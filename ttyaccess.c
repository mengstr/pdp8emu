/* File: ttyaccess.c
   Author: Douglas Jones, Dept. of Comp. Sci., U. of Iowa, Iowa City, IA 52242.
   Modified by: Sander van Malssen, svm@kozmix.hacktic.nl.
	added fixes so it uses termios.h rather than sgtty.h
   Date: Aug. 15, 1995
   Language: C (UNIX)
   Purpose: Code to take over the tty and put it in raw mode
*/

/* WARNING:  This code is highly UNIX dependant!  Linux users may have to
   include a #define for TERMIOS to force use of the right include file.
   This define belongs in stdio.h, I think (a very small Linux bug).
   Alternately (?), the include file <sgtty.h> can be changed to
   <bsd/sgtty.h> and the code can be compiled with the bsd library */


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#ifdef TERMIOS
#include <termios.h>
#else
#include <sgtty.h>
#endif

#include <signal.h>

#define control(ch) (ch & 037)

#define keyboard 0
#define display 1


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


/************************/
/* startup and shutdown */
/************************/

#ifdef TERMIOS
static struct termios oldstate; /* tty state prior to reset */
#else
static struct sgttyb oldstate; /* tty state prior to reset */
#endif

void ttyrestore(void);  /* forward declaration */

static void mykill(int arg)  /* called to kill process */
{
	ttyrestore();
	exit(-1);
}

void ttyraw(void) /* save tty state and convert to raw mode */
{
	/* take over the interactive terminal */
	{ /* get old TTY mode for restoration on exit */
#ifdef TERMIOS
		ioctl( keyboard, TCGETS, &oldstate );
#else
		ioctl( keyboard, TIOCGETP, &oldstate );
#endif
	}
	{ /* put TTY in RAW mode; note: raw mode may be a bit drastic! */
#ifdef TERMIOS
		struct termios newstate;
		ioctl( keyboard, TCGETS, &newstate );
		newstate.c_lflag &= ~ISIG;  /* don't enable signals */
		newstate.c_lflag &= ~ICANON;/* don't do canonical input */
		newstate.c_lflag &= ~ECHO;  /* don't echo */
		newstate.c_iflag &= ~INLCR; /* don't convert nl to cr */
		newstate.c_iflag &= ~IGNCR; /* don't ignore cr */
		newstate.c_iflag &= ~ICRNL; /* don't convert cr to nl */
		newstate.c_iflag &= ~IUCLC; /* don't map upper to lower */
		newstate.c_iflag &= ~IXON;  /* don't enable ^S/^Q on output */
		newstate.c_iflag &= ~IXOFF; /* don't enable ^S/^Q on input */
		newstate.c_oflag &= ~OPOST; /* don't enable output processing */
#ifdef        VMIN
		newstate.c_cc[VMIN] = 0 ;
		newstate.c_cc[VTIME] = 1 ;
#endif
		/* note:  on some UNIX systems, no amount of urging seems
		   to make it insist on converting cr to nl */
		ioctl( keyboard, TCSETS, &newstate );
#else
		struct sgttyb newstate;
		ioctl( keyboard, TIOCGETP, &newstate );
		newstate.sg_flags |= RAW;
		newstate.sg_flags &= ~ECHO;
		ioctl( keyboard, TIOCSETP, &newstate );
#endif
	}
	{ /* install handlers to catch attempts to kill process */
		/* should probably catch other fatal signals too */
		signal( SIGTERM, mykill );
	}
}

void ttyrestore(void) /* return console to user */
{
#ifdef TERMIOS
	ioctl( keyboard, TCSETS, &oldstate );
#else
	ioctl( keyboard, TIOCSETP, &oldstate );
#endif
}


/*********************/
/* user I/O routines */
/*********************/

void (* ttybreak) () = NULL; /* set by user, called when 5 consec ^C seen */

void ttyputc(char ch) /* put character to console */
{
	int count;
	char buf = ch;
	count = write( display, &buf, 1 );
}

#define BLOCKING 0
#define NONBLOCK 1
static int mode = BLOCKING; /* most recent tty read mode */

static int breakcount = 0; /* count of consecutive ^C chars while polling */

/* following definitions are for auxiliary path into TTY input stream */
#define stufflen 16
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

int ttypoll(void) /* poll for a character from the console */
{
	int count;
	char buf;

	/* try to fill input buffer with one char */
	if (stuffhead != stufftail) {
		count = 1;
		buf = stuffqueue[stuffhead];
		stuffhead = (stuffhead + 1) % stufflen;
	} else {
		if (mode == BLOCKING) { /* make nonblocking */
			int flag;
			flag = fcntl( keyboard, F_GETFL, 0 );
#ifdef TERMIOS
			fcntl( keyboard, F_SETFL, flag | O_NDELAY );
#else
			fcntl( keyboard, F_SETFL, flag | FNDELAY );
#endif
			mode = NONBLOCK;
		}

		count = read( keyboard, &buf, 1 );
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

	if (stuffhead != stufftail) {
		buf = stuffqueue[stuffhead];
		stuffhead = (stuffhead + 1) % stufflen;
	} else {
		if (mode != BLOCKING) { /* make nonblocking */
			int flag;
			flag = fcntl( keyboard, F_GETFL, 0 );
#ifdef TERMIOS
			fcntl( keyboard, F_SETFL, flag & ~O_NDELAY );
#else
			fcntl( keyboard, F_SETFL, flag & ~FNDELAY );
#endif
			mode = BLOCKING;
		}
		read( keyboard, &buf, 1 );
	}

	breakcount = 0;
	return buf & 0177;
}

void ttyputs(char * buf) /* put string to console */
{
	int count;
	for (count = 0; buf[count] != '\0'; count++) {;};
	count = write( display, buf, count );
	fsync( display );
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
