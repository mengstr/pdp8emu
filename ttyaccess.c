/* File: ttyaccess.c
   Author: Douglas Jones, Dept. of Comp. Sci., U. of Iowa, Iowa City, IA 52242.
   Modified by: Sander van Malssen, svm@kozmix.hacktic.nl.
	added fixes so it uses termios.h rather than sgtty.h
   Date: Aug. 15, 1995
   Updates: Mats Engstrom, April 2020
   Language: C (UNIX)
   Purpose: Code to take over the tty and put it in raw mode
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <termios.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <string.h>

#define ignore_value(x) (__extension__ ({ __typeof__ (x) __x = (x); (void) __x; }))

#define control(ch) (ch & 037)

static int sock;

/*************/
/* operation */
/*************/

/* This code takes over you keyboard input and display output files so
   that the emulator will be able to do single keypress operations.

   As written here, it uses the UNIX ioctl() service to put the keyboard
   into raw-noecho mode (thus turning off all UNIX input processing,
   including break character detection and more), and then it uses
   fcntl() to allow polling of the input line.  It would be better
   to use UNIX signals to grab the keyboard interrupt, but an attempt at
   this caused too many headaches.

   Unfortunately, UNIX doesn't guarantee an interrupt for every key,
   so the interrupt service routine has to get one or more characters
   each time it is called; UNIX then adds insult to injury by providing
   lousy tools to handle the resulting critical sections.
*/


static int create_udp_socket(int port) 
{
	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("could not create socket\n");
		return 1;
	}

	struct timeval read_timeout;
	read_timeout.tv_sec = 0;
	read_timeout.tv_usec = 1;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);

	if ((bind(sock, (struct sockaddr *)&server_address,
	          sizeof(server_address))) < 0) {
		printf("could not bind socket\n");
		return 1;
	}
	return 0;
}


/************************/
/* startup and shutdown */
/************************/

static struct termios oldstate; /* tty state prior to reset */

void ttyrestore(void);  /* forward declaration */

static void mykill(int arg)  /* called to kill process */
{
	ttyrestore();
	exit(-1);
}

void ttyraw(void) /* save tty state and convert to raw mode */
{
	/* take over the interactive terminal */
	/* get old TTY mode for restoration on exit */
	tcgetattr(STDIN_FILENO, &oldstate);
	
	/* put TTY in RAW mode; note: raw mode may be a bit drastic! */
	struct termios newstate;
	tcgetattr(STDIN_FILENO, &newstate);
	newstate.c_lflag &= ~ISIG;  /* don't enable signals */
	newstate.c_lflag &= ~ICANON;/* don't do canonical input */
	newstate.c_lflag &= ~ECHO;  /* don't echo */
	newstate.c_iflag &= ~INLCR; /* don't convert nl to cr */
	newstate.c_iflag &= ~IGNCR; /* don't ignore cr */
	newstate.c_iflag &= ~ICRNL; /* don't convert cr to nl */
	newstate.c_iflag &= ~IXON;  /* don't enable ^S/^Q on output */
	newstate.c_iflag &= ~IXOFF; /* don't enable ^S/^Q on input */
	newstate.c_oflag &= ~OPOST; /* don't enable output processing */
	newstate.c_cc[VMIN] = 0 ;
	newstate.c_cc[VTIME] = 1 ;
	tcsetattr(STDIN_FILENO, TCSANOW, &newstate);

	/* install handlers to catch attempts to kill process */
	signal( SIGTERM, mykill );

	/* Set keyboard to nonblocking */
	int flag;
	flag = fcntl( STDIN_FILENO, F_GETFL, 0 );
	fcntl( STDIN_FILENO, F_SETFL, flag | O_NDELAY );

	/* Create the UDP socket listening at port 2288 */
	create_udp_socket(2288);
}

void ttyrestore(void) /* return console to user */
{
	tcsetattr(STDIN_FILENO, TCSANOW, &oldstate);
}


/*********************/
/* user I/O routines */
/*********************/

void (* ttybreak) () = NULL; /* set by user, called when 5 consec ^C seen */

void ttyputc(char ch) /* put character to console */
{
	char buf = ch;
	ignore_value(write( STDOUT_FILENO, &buf, 1 ));
}


static int breakcount = 0; /* count of consecutive ^C chars while polling */

/* following definitions are for auxiliary path into TTY input stream */
#define stufflen 1024
static int stuffhead = 0; /* head pointer for stuffing queue */
static int stufftail = 0; /* tail pointer for stuffing queue */
static char stuffqueue[stufflen];

void ttystuff(char ch) /* stuff a char into input stream from auxiliary source */
{
	int newtail = (stufftail + 1) % stufflen;
	if (newtail != stuffhead) { /* discards excess input */
		stuffqueue[stufftail] = ch;
		stufftail = newtail;
	}
}

static void poll_udp()  /* poll for characters coming in at the UDP socket */
{
	struct sockaddr_in client_address;
	socklen_t client_address_len = 0;
	int len;
	int i;

	char buffer[256];
	len = recvfrom(sock, buffer, sizeof(buffer), 0,
				(struct sockaddr *)&client_address,
				&client_address_len);

	for (i=0; i<len; i++) {
		ttystuff(buffer[i]);
	}
}


int ttypoll(void) /* poll for a character from the console */
{
	int count;
	char buf;
	
	poll_udp();

	/* try to fill input buffer with one char */
	if (stuffhead != stufftail) {
		count = 1;
		buf = stuffqueue[stuffhead];
		stuffhead = (stuffhead + 1) % stufflen;
	} else {
		count = read(STDIN_FILENO, &buf, 1 );
	}

	if (count <= 0) {
		return -1;
	} else {
		if (buf == control('c')) { /* detect 5 control C in a row */
			breakcount++;
			if ((breakcount >= 5) && (ttybreak != NULL)) {
				ttybreak();
				breakcount = 0;
			}
		} else {
			/* This should be dealt with in ttyraw(),
			   but many UNIX TTY interfaces refuse to do it!
			if (buf == '\n')
				buf = '\r'; */
			breakcount = 0;
		}
		return buf;
	}
}

int ttygetc(void) /* blocking 7 bit read from console */
{
	char buf;
	int len;

	for (;;) {
		poll_udp();

		if (stuffhead != stufftail) {
			buf = stuffqueue[stuffhead];
			stuffhead = (stuffhead + 1) % stufflen;
			break;
		} else {
			len=read(STDIN_FILENO, &buf, 1 );
			if (len>0) break;
		}
	}

	breakcount = 0;
	return buf & 0177;
}

void ttyputs(char * buf) /* put string to console */
{
	int count;
	for (count = 0; buf[count] != '\0'; count++) {;};
	count = write(STDOUT_FILENO, buf, count );
	fsync( STDOUT_FILENO );
}

void ttygets(char * buf, int len) /* get string from console */
{
	int i = 0;
	int ch;
	do {
		ch = ttygetc();
		if (ch == '\b') {
			if (i > 0) {
				ttyputs( "\b \b" );
				i--;
			}
		} else if (ch >= ' ') {
			if (i < (len - 1)) {
				ttyputc(ch);
				buf[i] = ch;
				i++;
			}
		}
	} while ((ch != '\r') && (ch != '\n'));
	ttyputs( "\r\n" );
	if (i < len) {
		buf[i] = '\0';
	} else {
		buf[len - 1] = '\0';
	}
}
