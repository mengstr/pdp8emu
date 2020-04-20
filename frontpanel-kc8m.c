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
#include <sys/errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <strings.h>
#include "realtime.h"
#include "bus.h"
#include "ttyaccess.h"
#include "debug.h"
#include "kk8e.h"
#include "disasm.h"


#define control(ch) (ch & 037)

/*********************/
/* utility functions */
/*********************/

//
// 123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
// PC=_:____  DF=_ L=_ AC=____ MQ=____ AI=____ ____ ____ ____ ____ ____ ____ ____ 
//
char *getAllRegs() {
	static char buf[100];
	sprintf(buf,"PC=%d:%04o DF=%d L=%d AC=%04o MQ=%04o AI=%04o %04o %04o %04o %04o %04o %04o %04o",
		ifr,pc,dfr,lnk?1:0,ac,mq,memory[010],memory[011],memory[012],memory[013],memory[014],memory[015],memory[016],memory[017]
	);
	return buf;
}

//
// 123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
// ____=____ ____=____ ____=____ ____=____ ____=____ ____=____ ____=____ ____=____ 
//
char *getAllWatched() {
	static char buf[100];
	static char tmps[100];
	buf[0]=0;
	for (int i=0; i<MAX_WATCHES; i++) {
		if (watch[i]>=0) {
			sprintf(tmps,"%04o=%04o ",watch[i],memory[watch[i]]);
			strcat(buf,tmps);
		}
	}
	return buf;
}


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
	printf("%s",buf);
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
	printf("\r\n");
	while (temp != NULL) {
		printf("%s%s%s\r\n",temp -> name, temp -> longname, temp -> file );
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

//  D = list all devices
//  M = mount file on device


static char *help_message = "\
g[addr]        - Run starting at current PC or at the given address.\r\n\
\r\n\
t[cnt][,addr]  - Trace one (or 'cnt') instructions starting at current PC\r\n\
                 or at the given address.\r\n\
\r\n\
s<value><reg>  - Set register PC/LINK/AC/MQ/SWITCH/IFLD/DFLD to value.\r\n\
\r\n\
d<addr>[,len]  - Dump 'len' (or 16 bytes if not specifed) of memory starting\r\n\
                 at given address.\r\n\
\r\n\
D<addr>[,len]  - Disassemble 'len' (or 16 bytes if not given) of memory \r\n\
                 starting at the given address.\r\n\
\r\n\
m<addr>,[data] - Modify memory content at address with data. Prompt if no data\r\n\
                 is given. Enter dot (.) to exit mode or Enter to accept the\r\n\
                 current value.\r\n\
\r\n\
b[value][type] - Set/Clear breakpoint at or for the specicied value for the\r\n\
                 given type (E/W/R for execute, write or read at address,\r\n\
                 or O for executing the given opcode). If no type is\r\n\
                 given then the breakpoint is removed. If no value is given\r\n\
                 all breakpoints are shown.\r\n\
\r\n\
r              - Clear(reset) registers and flags.\r\n\
\r\n\
l              - List devices.\r\n\
\r\n\
q              - Quit emulator and exit to operating system.\r\n\
\r\n\
#              - Comment, can also use ;\r\n\
\r\n\
";

static int isOctal(char ch) {
	return (ch>='0' && ch<='7');
}


static char parse_nums(char *p, int *num1, int *num2) {
	*num1=-1;
	*num2=-1;
	char ch=0;

	while (!isOctal(*p)) {	// Skip leading non-octals
		if (!(*p)) return ch;	// Return at end-of string
		p++;
	}

	while(isOctal(*p)) {	// Collect num1 while octal numbers
		if (*num1==-1) *num1=0;	// Zero num at first digit
		*num1=(*num1)*8+(*p-'0');
		p++;
	}
	if (!(*p)) return ch;		// Return at end-of string

	while (!isOctal(*p)) {	// Skip middle non-octals
		if (!(*p)) return ch;	// Return of end-of string
		if (ch==0 && (*p)!=' ') ch=*p; // Store first non-space non-octal
		p++;
	}

	while(isOctal(*p)) {	// Collect num2 while octal numbers
		if (*num2==-1) *num2=0;	// Zero num at first digit
		*num2=(*num2)*8+(*p-'0');
		p++;
	}
	return ch;
}

char* toThousandsString(long long val) {
    static char result[ 128 ];
    snprintf(result, sizeof(result), "%lld", val);
    int i = strlen(result) - 1;
    int i2 = i + (i / 3);
    int c = 0;
    result[i2 + 1] = 0;
    for( ; i != 0; i-- ) {
        result[i2--] = result[i];
        c++;
        if( c % 3 == 0 )
            result[i2--] = ' ';
    }
    return result;
}

void console(void) {
	char ch;
	char *p;
	int i,cnt;
	char cmd[100];
	int num1, num2;

	// Spend 1/10 of a second here making sure the tty buffer is processed
	for (i=0; i<100; i++) {
		fire_timer();
		usleep(1000);
	}

	// printf("\r\n %s executed\r\n",toThousandsString(opcnt));

	printf("\r\n%s\r\n",getAllRegs());
	p=getAllWatched();
	if (strlen(p)>0) printf("%s\r\n",p);

	while (run <= RUNMODE_STOPPED) {
		ttygets(cmd, 100);
		ch=parse_nums(cmd, &num1, &num2);
		switch (cmd[0]) {

			case '?': // Help
				fprintf(stdout,"%s", help_message);
				break;

			case '#': // comment
			case '/': // comment
				break;

			case 'q': // Quit
				printf( "\r\nQuitting\r\n" ); fflush(stdout); usleep(100000);
				powerdown();
				break;

			case 'c': // Clear
				printf( "\r\nFlags and registers cleared\r\n" );
				ifr = 0;
	 			ib = ifr;
				dfr = ifr;
				cpma = pc | ifr;
				mq=0;
				sr=0;
				clearflags();
				break;

			case 's': // Set register value
				if (num1==-1) break;
				if (ch=='P' || ch=='p') pc=num1;
				if (ch=='L' || ch=='l') lnk=010000*(num1&1);
				if (ch=='A' || ch=='a') ac=num1;
				if (ch=='M' || ch=='m') mq=num1;
				if (ch=='S' || ch=='s') sw=num1;
				if (ch=='I' || ch=='i') ifr=num1&7;
				if (ch=='D' || ch=='d') dfr=num1&7;
				printf("\r\n%s\r\n",getAllRegs());
				break;

			case 'm': // Set data in memory
				if (num1==-1) break;
				if (num2>=0) {
					memory[num1]=num2;
					break;
				}
				for (;;) {
					printf("%04o: %04o  New:",num1,memory[num1]);
					fflush(stdout);
					ttygets(cmd, 100);
					fflush(stdout);
					if (cmd[0]==0) {
						num1++;
						continue;
					}
					if (!isOctal(cmd[0])) break;
					ch=parse_nums(cmd, &num2, &num2);
					memory[num1]=num2;
					num1++;
				}
				break;

			case 'g': // Go/Run
				if (num1!=-1) pc=num1;
				cpma=pc;
				run=RUNMODE_STARTING;	
				break;

			case 'd': // Dump memory
				if (num1==-1) break;
				if (num2==-1) num2=16;
				while (num2>0) {
					printf("%04o: ",num1-(num1%8));
					for (i=0; i<num1%8; i++) printf(".... ");
					cnt=8-num1%8;
					for (i=0; i<cnt; i++) {
						printf("%04o ",memory[num1]);
						num1++;
						num2--;
						if (num2==0) break;
					}
					printf("\r\n");
				}
				break;

			case 'D': // Disassemble from memory
				if (num1==-1) break;
				if (num2==-1) num2=16;
				while (num2>0) {
					printf("%04o: %04o - %s\r\n",num1,memory[num1],ops[memory[num1]]);
					num1++;
					num2--;
				}
				break;

			case 'b': // Breakpoints
				if (num1==-1 && num2==-1) {
					for (i=0; i<MAX_BREAKPOINTS; i++) {
						if (bp_type[i]=='E') printf("BP at execution at %04o\r\n",bp[i]);
						if (bp_type[i]=='R') printf("BP at read of %04o\r\n",bp[i]);
						if (bp_type[i]=='W') printf("BP at write of %04o\r\n",bp[i]);
						if (bp_type[i]=='O') printf("BP at opcode %04o\r\n",bp[i]);
					}
					break;
				}
				if (num1!=-1 && ch==0) {
					for (i=0; i<MAX_BREAKPOINTS; i++) {
						if (bp[i]==num1) {
							if (bp_type[i]=='E') printf("Removed BP of execution type\r\n");
							if (bp_type[i]=='R') printf("Removed BP of read of type\r\n");
							if (bp_type[i]=='W') printf("Removed BP of write of type\r\n");
							if (bp_type[i]=='O') printf("Removed BP of opcode type\r\n");
							bp_type[i]=0;
						}
					}
					break;
				}
				if (num1!=-1 && ch>0) {
					for (i=0; i<MAX_BREAKPOINTS; i++) {
						if (bp_type[i]==0) {
							if (ch=='E'|ch=='e') {
								bp_type[i]='E';
								bp[i]=num1;
								printf("Added BP for execution at address %04o\r\n",num1);
								i=MAX_BREAKPOINTS;
							}
							if (ch=='R'|ch=='r') {
								bp_type[i]='R';
								bp[i]=num1;
								printf("Added BP for read of address %04o\r\n",num1);
								i=MAX_BREAKPOINTS;
							}
							if (ch=='W'|ch=='w') {
								bp_type[i]='W';
								bp[i]=num1;
								printf("Added BP for write of address %04o\r\n",num1);
								i=MAX_BREAKPOINTS;
							}
							if (ch=='O'|ch=='o') {
								bp_type[i]='O';
								bp[i]=num1;
								printf("Added BP for opcode  %04o\r\n",num1);
								i=MAX_BREAKPOINTS;
							}
						}
					}
					break;
				}
			case 'l': // List devices
				list_devices();	
				break;
		}
	}
}


// 		case 'G':
// 		case 'g': /* Go */
// 		case 'T':
// 		case 't': /* Trace */
// 			if (ch=='T' || ch=='t') {
// 				trace=1;
// 			} else {
// 				trace=0;
// 			}
// 			clearflags();
// 			if (number >= 0) {
// 				pc = number & 07777;
// 				ifr = number & 070000;
// 			} else {
// 				pc = 0;
// 				ifr = 0;
// 			}
// 			ib = ifr;
// 			dfr = ifr;
// 			cpma = pc | ifr;
// 			ttyputs( "\r\n" );
// 			run = 1;
// 			break;
// 		case 'D':
// 		case 'd': /* List Devices */
// 			list_devices();
// 			break;
// 		case 'M':
// 		case 'm': /* Mount file on device */
// 			{
// 				struct device_rec * d;
// 				char n[5];
// 				char f[80];
// 				ttyputs( "\r\n device: " );
// 				ttygets( n, 5 );
// 				if ((d = get_device(n)) != NULL) {
// 					int u = d -> unit;
// 					ttyputs( " file: " );
// 					ttygets( f, 80 );
// 					if (f[0] != '\0') {
// 						if (!(*(d -> mount))( u, f )) {
// 							ttyputs( "?\r\n" );
// 						}
// 					} else {
// 						(* (d -> dismount))( u );
// 					}
// 				} else {
// 					ttyputs( "?\r\n" );
// 				}
// 			}
// 			break;


/**********************************************************/
/* Interface between cpu implementation and control panel */
/**********************************************************/

static void hitbreak(void) /* called by keyboard server to get attention */
{
	run=RUNMODE_BREAK;
}

void kc8power(int argc, char **argv) /* power-on initialize */
{
	init_timer(console_delay);
	ttybreak = hitbreak;
	for (int i=0; i<MAX_WATCHES; i++) watch[i]=-1;
	printf( "PDP-8 Emulator, type ? for help.\r\n" );
}

void kc8init(void) /* console reset */
{
	/* nothing to do here */
}

void kc8halt(void) /* respond to halt instruction */
{
	/* nothing to do here */
}

