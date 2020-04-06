/* File: kc8m.c
   Author: Douglas Jones, Dept. of Comp. Sci., U. of Iowa, Iowa City, IA 52242.
   Date: Feb. 29, 1996
   Updates: Mats Engstrom, April 2020
   Language: C (UNIX)
   Purpose: DEC PC8/M (blank and ugly tty based) control panel emulator.

   Based on the description in the PDP-8/E Maintenance manual,
   Digital Equipment Corporation, 1971, liberally interpreted.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "realtime.h"
#include "bus.h"
#include "ttyaccess.h"
#include "debug.h"
#include "kk8e.h"

#define control(ch) (ch & 037)

/*********************/
/* utility functions */
/*********************/

void ttyoctal(int num, int digits ,char * suffix)
{
	char buf[32];
	int temp = num;
	int i = digits;
	while (i > 0) {
		i--;
		buf[i] = (temp & 07) | '0';
		temp >>= 3;
	}
	i = digits;
	if (suffix != (char *)0) {
		int j = 0;
		while (suffix[j] != '\0') {
			buf[i++] = suffix[j++];
		}
	}
	buf[i] = '\0';
	ttyputs(buf);
}


/**********************************************/
/* Implementation of device control interface */
/**********************************************/

struct device_rec {
	int (* mount)(); /* hook to mount file on device */
	void (* dismount)(); /* hook to dismount mounted file */
	int unit; /* thing to pass as param to mount and dismount */
	char * name; /* device name */
	char * longname; /* descriptive device name */
	char * file; /* file attached to device */
	struct device_rec * next; /* next list element */
};
static struct device_rec * devices = NULL;

void register_device(
	int (* m)(), /* hook to mount file on device */
	void (* d)(), /* hook to dismount mounted file */
	int u, /* device unit */
	char * n, /* device name */
	char * l, /* descriptive device name */
	char * f /* file attached to device */
)
{
	struct device_rec * temp;
	temp = (struct device_rec *)malloc( sizeof( struct device_rec ) );
	temp -> next = devices;
	temp -> mount = m;
	temp -> dismount = d;
	temp -> unit = u;
	temp -> name = n;
	temp -> longname = l;
	temp -> file = f;
	devices = temp;
}

void close_devices(void)
{
	struct device_rec * temp = devices;
	while (temp != NULL) {
		(* (temp -> dismount))( temp -> unit );
		temp = temp -> next;
	}
}

void dump_devices(FILE *f)
{
	struct device_rec * temp = devices;
	while (temp != NULL) {
		if ((temp -> file)[0] != '\0') { /* device is mounted */
			fputs( "m ", f );
			fputs( temp -> name, f );
			fputs( " ", f );
			fputs( temp -> file, f );
			fputs( "\n", f );
		}
		temp = temp -> next;
	}
}

static void list_devices(void)
{
	struct device_rec * temp = devices;
	ttyputs( "\r\n" );
	while (temp != NULL) {
		ttyputs( temp -> name );
		ttyputs( temp -> longname );
		ttyputs( temp -> file );
		ttyputs( "\r\n" );
		temp = temp -> next;
	}
}

static struct device_rec * get_device(char * n)
{
	struct device_rec * temp = devices;
	while ((temp != NULL) && (strcmp(temp -> name, n) != 0)) {
		temp = temp -> next;
	}
	return temp;
}

void mount_device(char * n, char * f)
{
	/* quietly try to mount files on devices, used during startup */
	struct device_rec * d;
	if ((d = get_device(n)) != NULL) {
		int u = d -> unit;
		if ( ((d -> file)[0] == '\0') && (f[0] != '\0') ) {
			/* so long as a device is not mounted
			   and so long as a nonblank name was provided
			   try to do the mount */
			(void)(*(d -> mount))( u, f );
			/* ignore any error codes */
		}
	}
}

/********************************************/
/* Implementation of control panel function */
/********************************************/

static struct timer console_delay;

extern void (* ttybreak) (); /* hook to tty for keyboard overrun */

static char * help_message = "\
\r\n\
PDP-8/E emulator, commands are:\r\n\
\n\
 nnnnnG = goto location N  nnnnn/ = open memory location N\r\n\
 C = continue from halt    A = open accumulator\r\n\
 L = open link             S = open switch register\r\n\
 <cr> = close unchanged    nnnn<cr> = store N in open location, then close\r\n\
 the value in a location is printed when that location is opened.\r\n\
\n\
 D = list all devices\r\n\
 M = mount file on device\r\n\
 to dismount file, mount nothing on the device\r\n\
\n";

static void console_event(void)
/* all console functions happen here! */
{
	int number = -1; /* number from command line */
	int loc = 0; /* current memory location (negative => special loc) */
	char ch; /* input character */

	ttyoctal( ifr >> 12, 1, "" );
	ttyoctal( pc, 4, " (" );
	ttyoctal( ac, 4, "\r\n" );
	do {
		do { /* get next character but ignore break characters */
			ch = ttygetc();
		} while (ch == control('c'));
		ttyputc( ch );
		if ((ch >= '0')&&(ch <= '7')) {
			if (number < 0) {
				number = 0;
			}
			number = (number << 3) | (ch & 07);
			if (number >= MAXMEM) {
				number = -1;
				ttyputs( "?\r\n" );
			}
		} else switch (ch) {
		case '/': /* open number*/
			if (number >= 0) {
				loc = number;
				number = -1;
			}
			ttyoctal( memory[loc], 4, " " );
			break;
		case 'S':
		case 's': /* open switch register */
			loc = -3;
			number = -1;
			ttyoctal( sr, 4, " " );
			break;
		case 'A':
		case 'a': /* open accumulator */
			loc = -2;
			number = -1;
			ttyoctal( ac, 4, " " );
			break;
		case 'L':
		case 'l': /* open link */
			loc = -1;
			number = -1;
			ttyoctal( (link >> 12), 1, " " );
			break;
		case '\r': /* set open location and close */
		case '\n': /* synonym */
			if (number >= 0) {
				if (loc >= 0) {
					memory[loc] = number & 07777;
				} else if (loc == -3) {
					sr = number & 07777;
				} else if (loc == -2) {
					ac = number & 07777;
				} else if (loc == -1) {
					link = (number & 01) << 12;
				}
				number = -1;
			}
			if (ch == '\r') {
				ttyputc( '\n' );
			} else {
				ttyputc( '\r' );
			}
			break;
		case 'G':
		case 'g': /* Go */
			clearflags();
			if (number >= 0) {
				pc = number & 07777;
				ifr = number & 070000;
			} else {
				pc = 0;
				ifr = 0;
			}
			ib = ifr;
			dfr = ifr;
			cpma = pc | ifr;
			ttyputs( "\r\n" );
			run = 1;
			break;
		case 'C':
		case 'c': /* Continue */
			ttyputs( "\r\n" );
			run = 1;
			break;
		case 'Q':
		case 'q': /* Quit */
			ttyputs( "\r\n" );
			powerdown();
			/* the above never returns */
			break;
		case 'D':
		case 'd': /* List Devices */
			list_devices();
			break;
		case 'M':
		case 'm': /* Mount file on device */
			{
				struct device_rec * d;
				char n[5];
				char f[80];
				ttyputs( "\r\n device: " );
				ttygets( n, 5 );
				if ((d = get_device(n)) != NULL) {
					int u = d -> unit;
					ttyputs( " file: " );
					ttygets( f, 80 );
					if (f[0] != '\0') {
						if (!(*(d -> mount))( u, f )) {
							ttyputs( "?\r\n" );
						}
					} else {
						(* (d -> dismount))( u );
					}
				} else {
					ttyputs( "?\r\n" );
				}
			}
			break;
		case '?':
			ttyputs( help_message );
			number = -1;
			break;
		case 'H':
		case 'h':
			ttyputs( "\r\n" );
			output_debug();
			break;
		default:
			ttyputs( "?\r\n" );
			number = -1;
		}
	} while ( run == 0 );

	/* note that console mode is still raw on exit */
}

/**********************************************************/
/* Interface between cpu implementation and control panel */
/**********************************************************/

static void hitbreak(void) /* called by keyboard server to get attention */
{
	run = 0;
	schedule( &console_delay, 0L, console_event, 0 );
}

void kc8power(int argc, char **argv) /* power-on initialize */
{
	init_timer( console_delay );
	ttybreak = hitbreak;
	ttyputs( "PDP-8 Emulator, type ? for help.\r\n" );
}

void kc8init(void) /* console reset */
{
	/* nothing to do here */
}

void kc8halt(void) /* respond to halt instruction */
{
	/* force console event! */
	schedule( &console_delay, 0L, console_event, 0 );
}

