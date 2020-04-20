/* File: ttyaccess.c
   Author: Mats Engstrom, April 2020
   Language: C (UNIX)
   Purpose: Handle character-by-character input from stdin and udp using pthreads
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/select.h>
#include <stdlib.h>
#include  <string.h>
#include <sgtty.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include "ttyaccess.h"

struct threaddata_struct {
    int termpipe;
    int udp_socket;
};

static pthread_t thread;
static int pipes[2];
static struct threaddata_struct td;

#define RBLEN 1024
static volatile int rbhead = 0; // head pointer for ring buffer queue 
static volatile int rbtail = 0; // tail pointer for ring buffer queue 
static volatile char rbqueue[RBLEN];
static pthread_mutex_t rblock;      // Mutex for locking the ring buffer

static char breakcnt;




//
// Store character into ringbuffer
//
static void queue_to_rb(char ch)  {
	int newtail = (rbtail + 1) % RBLEN;
	if (newtail != rbhead) {
		rbqueue[rbtail] = ch;
		rbtail = newtail;
	}
	if (ch==3) breakcnt++;
	else breakcnt=0;
}


//
// Get a character from ringbuffer
// returns -1 if empty, else the character
//
int get_from_rb(void)  {
    int ch=-1;
    pthread_mutex_lock(&rblock);
	if (rbhead != rbtail) {
        ch = rbqueue[rbhead];
        rbhead = (rbhead + 1) % RBLEN;
    }
    pthread_mutex_unlock(&rblock);
    return ch;
}


//
// Return number of characters available in the ring buffer
//
int count_rb(void)  {
    int cnt;
    pthread_mutex_lock(&rblock);
	cnt=abs(rbhead-rbtail);
    pthread_mutex_unlock(&rblock);
    return cnt;
}


//
// Set stdin to either "raw" or normal mode
// mode=true for raw, false=normal
//
static void set_stdin_raw(int mode) {
	struct sgttyb sg;
	ioctl(STDIN_FILENO, TIOCGETP, &sg);
    if (mode) {
        sg.sg_flags |= CBREAK;
        sg.sg_flags &= ~ECHO;
    } else {
        sg.sg_flags &= ~CBREAK;
        sg.sg_flags |= ECHO;
    }
	ioctl(STDIN_FILENO, TIOCSETN, &sg);
}


//
// Create and bind to a listening UDP socket
//
static int create_udp_socket(int port) {
	struct sockaddr_in server_address;
    int sock;
    int r;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    sock = socket(PF_INET, SOCK_DGRAM, 0);
    for (int i=0; i<5; i++) {
        r=bind(sock, (struct sockaddr *)&server_address, sizeof(server_address));
        if (r==0) break;
        sleep(1);
    }
    if (r < 0) {
		printf("Could not bind socket to UDP port %d\n",port);
		exit(1);
	}
	return sock;
}

//
// Thread that waits for keypresses or udp messages and puts them into the ring buffer
//
void *keyin_thread(void *td) {
    char buffer[256];
    int len;
    fd_set rfds;

    int termpipe = ((struct threaddata_struct *)td)->termpipe;
    int udp_socket = ((struct threaddata_struct *)td)->udp_socket;

    for (;;) {
        FD_ZERO(&rfds);
        FD_SET(STDIN_FILENO, &rfds);
        FD_SET(termpipe, &rfds);
        FD_SET(udp_socket, &rfds);

        while (select(udp_socket+1, &rfds, NULL, NULL, NULL) == 0);

        if (FD_ISSET(termpipe, &rfds)) {
            close(termpipe);
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &rfds)) {
            len=read(STDIN_FILENO, buffer, sizeof(buffer));
            pthread_mutex_lock(&rblock);
            for (int i=0; i<len; i++) queue_to_rb(buffer[i]);
            pthread_mutex_unlock(&rblock);
        }

        if (FD_ISSET(udp_socket, &rfds)) {
            len=recv(udp_socket, buffer, sizeof(buffer),0);
            pthread_mutex_lock(&rblock);
            for (int i=0; i<len; i++) queue_to_rb(buffer[i]);
            pthread_mutex_unlock(&rblock);
        }
    }
    pthread_exit(NULL);
}


//
// Traps ctrl-c and send it to the ring buffer for normal processing
//
static void mykill(int arg) {
	queue_to_rb(3);
}


//
//
//
void comms_init(void) {
    pthread_mutex_init(&rblock, NULL);
    pipe(pipes);
    td.termpipe=pipes[0];
    td.udp_socket=create_udp_socket(2288);
    set_stdin_raw(1);
	breakcnt=0;
	signal(SIGINT, mykill);
    pthread_create(&thread, NULL, keyin_thread, &td);
}


//
//
//
void comms_cleanup(void) {
    set_stdin_raw(0);
    close(pipes[1]);
    printf("Closing socket %d\r\n",td.udp_socket);
    close(td.udp_socket);
    pthread_mutex_destroy(&rblock);
    // pthread_exit(NULL);
}


//
//  blocking 7 bit read from console 
//
int ttygetc(void) {
	int ch;
	do {
		ch=get_from_rb();
        if (ch<0) {
        	if (breakcnt>4) ttybreak();
            usleep(100);
        }
    } while (ch<0);
	return ch;
}

//
// poll for a character from the console
//
int ttypoll(void) {
    if (breakcnt>4) ttybreak();
	return get_from_rb();
}


//
//
//
void ttygets(char * buf, int len) {
	int i = 0;
	int ch;
	char c;
	do {
		ch = ttygetc();
		if (ch == '\b' || ch==127) {
			if (i > 0) {
				write(STDOUT_FILENO, "\b \b", 3);
				i--;
			}
		} else if (ch >= ' ') {
			if (i < (len - 1)) {
				c=ch;
				write(STDOUT_FILENO, &c, 1);
				buf[i] = ch;
				i++;
			}
		}
	} while ((ch != '\r') && (ch != '\n'));
	write(STDOUT_FILENO, "\r\n", 2);
	if (i < len) {
		buf[i] = '\0';
	} else {
		buf[len - 1] = '\0';
	}
}
