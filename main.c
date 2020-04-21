/* File: kk8e.c
   Author: Douglas Jones, Dept. of Comp. Sci., U. of Iowa, Iowa City, IA 52242.
   Date: Mar. 6, 1996
   Updates: Mats Engstrom, April 2020
   Language: C (UNIX)
   Purpose: DEC PDP-8/e emulator

   Based on the PDP-8/E Small Computer Handbook,
   Digital Equipment Corporation, 1971, 1973,
   and the PDP-8/E/F/M Maintenance Manual, volumes 1 and 2,
   Digital Equipment Corporation, 1975.
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <ctype.h>

#include "bus.h"
#include "realtime.h"
#include "ttyaccess.h"
#include "debug.h"
#include "disasm.h"
#include "hstape-pc8e.h"
#include "floppy-rx8e.h"
#include "card-cr8f.h"
#include "frontpanel-kc8m.h"
#include "rtc-dk8e.h"
#include "tty-kl8e.h"
#include "mem-km8e.h"

char corename[NAME_LENGTH]; /* name of core image file, if any */
char * progname; /* name of program itself (argv[0]) */
int trace; /* true if disassembly/trace is output while running */
unsigned short memory[MAXMEM];
int bpInstCnt;
int bp[MAX_BREAKPOINTS];
int bp_type[MAX_BREAKPOINTS];   // 0=disabled, 1=address, 2=opcode
int watch[MAX_WATCHES];
long long opcnt;


/*******************************/
/* Generally visible registers */
/*******************************/

/* All of the following are visible outside the CPU in some context or
   another, either to some I/O device or to the front panel.
*/
int ac;  /* the accumulator, 12 bits */
int pc;  /* the program counter, 12 bits */
int mq;  /* the multiplier quotient, 12 bits */
int sr;  /* the switch register */
int cpma;/* the memory address register */
int mb;  /* the memory buffer register */

int lnk;/* the link bit, 1 bit, in position 010000 of the word */
int run; /* the run mode according to RUNMODE_xxxx */

int enab;/* interrupt enable bit, 0 = disable, 1=enable */
int enab_rtf; /* secodary enable needed for RTF deferred enable */
int irq; /* the interrupt request line, 0 = no request, >0 = request */

int sw;  /* the switch, 1 bit */

/* Note that any positive value of irq indicates a request!  Requests are
   posted by incrementing irq, and withdrawn by decrementing irq.
*/

/* 3 bit fields stored 12 places left so they can be ORed onto 12 addresses.
*/
int ifr; /* the instruction field register */
int dfr; /* the data field register */
int ib; /* the instruction field buffer (copy to if on branch, jsr) */

/* 7 bits, exactly as documented in the small computer handbook
*/
int sf; /* the save field register (save ir, if, df on interrupt) */

/* 1 bit, where ub is copied to uf on branch, jsr
*/
int uf; /* the user mode flag */
int ub; /* the user mode buffer */

/* 1 bit, reset on branch, jsr
*/
int km8e_uif; /* user interrupt flag (local to KM8E but used in KK8E) */



/************************************************************/
/* Declarations of machine components not included in bus.h */
/************************************************************/

/* Machine control bits */

int enab_del; /* secondary enable flipflop, used to delay enable operations */


/******************/
/* Initialization */
/******************/

/* Both the reset key on the console and the CAF instruction call this */
void clearflags(void)
{
	/* device specific effects of the reset operation */
	kc8init(); /* front panel */
	reset_debug();
// KE8E	ke8einit(); /* eae */
	km8einit(); /* mmu */
	dk8einit(); /* real-time clock */
	kl8einit(); /* console TTY */
	pc8einit(); /* paper tape reader punch */
	cr8finit(); /* card reader */
	rx8einit(); /* diskette drive */
	lnk = 000000;
	ac = 00000;
	irq = 0;
	enab = 0;
	enab_del = 0;
}


/* The following routine is used to make a copy of a string into a buffer *
 * that will be used as a file name.  This routine checks to make sure    *
 * that the file name fits in the buffer, and it prevents file names with *
 * embedded blanks by stopping the copy at the first blank.  It is safe   *
 * to call this with the same parameter for both arguments.               */

void set_file_name(
	char * f, /* buffer for file name */
	char * s /* string from which name comes */
)
{
        int i = 0; /* index into f */
        int j = 0; /* index into s */
        while ((i < (NAME_LENGTH - 1)) && (isgraph(s[j]))) {
                f[i++] = s[j++];
        }
        f[i] = '\0';
}

static void closecore(int u)
{
	corename[0] = '\0';
}

static int opencore(int u, char * f)
{
	set_file_name( corename, f );
	return(1);
}


void powerup(int argc, char** argv)
/* called only once for power on */
{
	// TODO Handle arguments

	run = RUNMODE_STOPPED; /* by default, the machine comes up halted */

	/* initialize the real-time simulation mechanisms */
	init_timers();

	/* initialize all devices to their power-on state */
	register_device( opencore, closecore, 0, "CORE", "- saved image of main memory   ", corename );
	kc8power( argc, argv ); /* console */
	// KE8E	ke8epower(); /* eae */
	km8epower(); /* mmu */
	dk8epower(); /* real-time clock */
	kl8epower(); /* console TTY */
	pc8epower(); /* paper tape reader punch */
	cr8fpower(); /* card reader */
	rx8epower(); /* diskette drive */

	int fd=open(argv[1],O_RDONLY);
	if (fd<0) {
 		printf("Failed to open %s : %s\n",argv[1],strerror(errno));
		 exit(1);
	}
	read(fd,memory,sizeof(memory));
	close(fd);

	clearflags();
}

void powerdown(void)
/* called only once from the console to exit the emulator */
{
	comms_cleanup();
	close_devices();
	exit(0);
}


/************************/
/* Instruction decoding */
/************************/

/*
/			       _____ _____ _____ _____ 
/  instruction word format: = |_|_|_|_|_|_|_|_|_|_|_|_|
/ 			      | op  |i|z|     adr     |
/
/  Instructions will be decoded using a 5 bit opcode-mode combination,
/  so the following opcode definitions are shifted 2 places left
*/

#define opAND (0 << 2)
#define opTAD (1 << 2)
#define opISZ (2 << 2)
#define opDCA (3 << 2)
#define opJMS (4 << 2)
#define opJMP (5 << 2)
#define opIOT (6 << 2)
#define opOPR (7 << 2)

/* The following definitions give the addressing modes as a 2 bit field
*/

#define DIRECT 0
#define DEFER 2
#define ZERO 0
#define CURRENT 1

/* The following definitions give instruction times in terms of 200 ns ticks
*/
#define shortcycle 6
#define longcycle 7

/* The following definitions give widely used code for addressing
*/


#define PAGE_ZERO cpma = ((mb & 0177) | ifr)

#define CURRENT_PAGE cpma = ((mb & 0177) | (pc & 07600) | ifr)

#define DEFER_CYCLE {							\
	if ((cpma & 07770) == 00010) { /* indexed */			\
		cpma = ((memory[cpma] = ((memory[cpma] + 1) & 07777))) | dfr;\
		countdown -= longcycle;					\
	} else { /* normal */						\
		cpma = (memory[cpma]) | dfr;				\
		countdown -= shortcycle;				\
	}								\
}


void write_memory(int a, int v) {
	if (run==RUNMODE_RUNNING) {	
		if (bp_type[0]=='W' && a==bp[0]) run=RUNMODE_BP_W;
		else if (bp_type[1]=='W' && a==bp[1]) run=RUNMODE_BP_W;
		else if (bp_type[2]=='W' && a==bp[2]) run=RUNMODE_BP_W;
		else if (bp_type[3]=='W' && a==bp[3]) run=RUNMODE_BP_W;
	}
	memory[a]=v;
}


int read_memory(int a) {
	if (run==RUNMODE_RUNNING) {	
		if (bp_type[0]=='R' && a==bp[0]) run=RUNMODE_BP_R;
		else if (bp_type[1]=='R' && a==bp[1]) run=RUNMODE_BP_R;
		else if (bp_type[2]=='R' && a==bp[2]) run=RUNMODE_BP_R;
		else if (bp_type[3]=='R' && a==bp[3]) run=RUNMODE_BP_R;
	}
	return memory[a];
}

// char *getAll() {
// 	static char buf[100];
// 	sprintf(buf,"PC=%d:%04o DF=%d L=%d AC=%04o MQ=%04o IRQ=%d %04o",
// 		ifr,pc,dfr,lnk?1:0,ac,mq,irq,memory[pc&4095]
// 	);
// 	return buf;
// }

/* Emulate the fetch/execute cycle */
int main(int argc, char **argv)
{
	comms_init();
	powerup(argc,argv);

	run=RUNMODE_STOPPED;
	for (;;) {
		if (run <= RUNMODE_STOPPED) {
			console();
			opcnt=0;
			continue;
		}

		opcnt++;

		/* setup to fetch from pc */
		cpma = pc | ifr;
		/* I/O and console activity happens with CPMA holding PC */
		while (countdown <= 0) { /* handle pending device activity */
			fire_timer();
		}

		/* If an interrupt was requested, PC will change! */
		if ((irq > 0) && (enab_del != 0) && (enab_rtf != 0)) {
			/* an interrupt occurs */
			memory[0] = pc;
			pc = 1;
			sf = (ifr >> 9) | (dfr >> 12) | (uf << 6);
			ifr = 0;
			ib = 0;
			dfr = 0;
			uf = 0;
			ub = 0;
			cpma = pc | ifr;
			countdown -= longcycle;
			enab = 0;
		}

		/* this line handles 1 instr delay of interrupt enable */
		enab_del = enab;

		/* the actual instruction fetch is here */
		mb = memory[cpma];

		if (bpInstCnt==0) {run=RUNMODE_CNT; continue;}
		if (bpInstCnt>0) bpInstCnt--;
		if (run==RUNMODE_RUNNING) {	
			if (bp_type[0]=='O' && mb==bp[0]) {run=RUNMODE_BP_O; continue;}
			if (bp_type[1]=='O' && mb==bp[1]) {run=RUNMODE_BP_O; continue;}
			if (bp_type[2]=='O' && mb==bp[2]) {run=RUNMODE_BP_O; continue;}
			if (bp_type[3]=='O' && mb==bp[3]) {run=RUNMODE_BP_O; continue;}
			if (bp_type[0]=='E' && cpma==bp[0]) {run=RUNMODE_BP_E; continue;}
			if (bp_type[1]=='E' && cpma==bp[1]) {run=RUNMODE_BP_E; continue;}
			if (bp_type[2]=='E' && cpma==bp[2]) {run=RUNMODE_BP_E; continue;}
			if (bp_type[3]=='E' && cpma==bp[3]) {run=RUNMODE_BP_E; continue;}
		}
		accumulate_debug(cpma,mb,ac,lnk,irq);
		countdown -= shortcycle;

		switch (mb >> 7) { /* note that we decode i and z here */


		case opAND | DIRECT | ZERO:
			PAGE_ZERO;
			pc = (pc + 1) & 07777;
			ac = ac & read_memory(cpma);
			countdown -= longcycle;
			break;

		case opAND | DIRECT | CURRENT:
			CURRENT_PAGE;
			pc = (pc + 1) & 07777;
			ac = ac & read_memory(cpma);
			countdown -= longcycle;
			break;

		case opAND | DEFER | ZERO:
			PAGE_ZERO;
			pc = (pc + 1) & 07777;
			DEFER_CYCLE;
			ac = ac & read_memory(cpma);
			countdown -= longcycle;
			break;

		case opAND | DEFER | CURRENT:
			CURRENT_PAGE;
			pc = (pc + 1) & 07777;
			DEFER_CYCLE;
			ac = ac & read_memory(cpma);
			countdown -= longcycle;
			break;


		case opTAD | DIRECT | ZERO:
			PAGE_ZERO;
			pc = (pc + 1) & 07777;
			ac = (ac | lnk) + read_memory(cpma);
			lnk = ac & 010000;
			ac = ac & 007777;
			countdown -= longcycle;
			break;

		case opTAD | DIRECT | CURRENT:
			CURRENT_PAGE;
			pc = (pc + 1) & 07777;
			ac = (ac | lnk) + read_memory(cpma);
			lnk = ac & 010000;
			ac = ac & 007777;
			countdown -= longcycle;
			break;

		case opTAD | DEFER | ZERO:
			PAGE_ZERO;
			pc = (pc + 1) & 07777;
			DEFER_CYCLE;
			ac = (ac | lnk) + read_memory(cpma);
			lnk = ac & 010000;
			ac = ac & 007777;
			countdown -= longcycle;
			break;

		case opTAD | DEFER | CURRENT:
			CURRENT_PAGE;
			pc = (pc + 1) & 07777;
			DEFER_CYCLE;
			ac = (ac | lnk) + read_memory(cpma);
			lnk = ac & 010000;
			ac = ac & 007777;
			countdown -= longcycle;
			break;


		case opISZ | DIRECT | ZERO:
			PAGE_ZERO;
			pc = (pc + 1) & 07777;
			mb = memory[cpma] = ((read_memory(cpma) + 1) & 07777);
			if (mb == 0) {
				pc = (pc + 1) & 07777;
			}
			countdown -= longcycle;
			break;

		case opISZ | DIRECT | CURRENT:
			CURRENT_PAGE;
			pc = (pc + 1) & 07777;
			mb = memory[cpma] = ((read_memory(cpma) + 1) & 07777);
			if (mb == 0) {
				pc = (pc + 1) & 07777;
			}
			countdown -= longcycle;
			break;

		case opISZ | DEFER | ZERO:
			PAGE_ZERO;
			pc = (pc + 1) & 07777;
			DEFER_CYCLE;
			mb = memory[cpma] = ((read_memory(cpma) + 1) & 07777);
			if (mb == 0) {
				pc = (pc + 1) & 07777;
			}
			countdown -= longcycle;
			break;

		case opISZ | DEFER | CURRENT:
			CURRENT_PAGE;
			pc = (pc + 1) & 07777;
			DEFER_CYCLE;
			mb = memory[cpma] = ((read_memory(cpma) + 1) & 07777);
			if (mb == 0) {
				pc = (pc + 1) & 07777;
			}
			countdown -= longcycle;
			break;


		case opDCA | DIRECT | ZERO:
			PAGE_ZERO;
			pc = (pc + 1) & 07777;
			write_memory(cpma, ac);
			ac = 00000;
			countdown -= longcycle;
			break;

		case opDCA | DIRECT | CURRENT:
			CURRENT_PAGE;
			pc = (pc + 1) & 07777;
			write_memory(cpma, ac);
			ac = 00000;
			countdown -= longcycle;
			break;

		case opDCA | DEFER | ZERO:
			PAGE_ZERO;
			pc = (pc + 1) & 07777;
			DEFER_CYCLE;
			write_memory(cpma, ac);
			ac = 00000;
			countdown -= longcycle;
			break;

		case opDCA | DEFER | CURRENT:
			CURRENT_PAGE;
			pc = (pc + 1) & 07777;
			DEFER_CYCLE;
			write_memory(cpma, ac);
			ac = 00000;
			countdown -= longcycle;
			break;

/* force indirect branching to use the instruction field */
#define dfr ifr

		case opJMS | DIRECT | ZERO:
			PAGE_ZERO;
			ifr = ib;
			uf = ub;
			enab_rtf = 1;
			write_memory(cpma, (pc + 1) & 07777);
			countdown -= longcycle;
			pc = (cpma + 1) & 07777;
			break;

		case opJMS | DIRECT | CURRENT:
			CURRENT_PAGE;
			ifr = ib;
			uf = ub;
			cpma = (cpma & 07777) | ifr;
			enab_rtf = 1;
			write_memory(cpma, (pc + 1) & 07777);
			countdown -= longcycle;
			pc = (cpma + 1) & 07777;
			break;

		case opJMS | DEFER | ZERO:
			PAGE_ZERO;
			DEFER_CYCLE;
			ifr = ib;
			uf = ub;
			cpma = (cpma & 07777) | ifr;
			enab_rtf = 1;
			write_memory(cpma, (pc + 1) & 07777);
			countdown -= longcycle;
			pc = (cpma + 1) & 07777;
			break;

		case opJMS | DEFER | CURRENT:
			CURRENT_PAGE;
			DEFER_CYCLE;
			ifr = ib;
			uf = ub;
			cpma = (cpma & 07777) | ifr;
			enab_rtf = 1;
			write_memory(cpma, (pc + 1) & 07777);
			countdown -= longcycle;
			pc = (cpma + 1) & 07777;
			break;


		case opJMP | DIRECT | ZERO:
			PAGE_ZERO;
			ifr = ib;
			uf = ub;
			cpma = (cpma & 07777) | ifr;
			enab_rtf = 1;
			pc = cpma & 07777;
			break;

		case opJMP | DIRECT | CURRENT:
			CURRENT_PAGE;
			ifr = ib;
			uf = ub;
			cpma = (cpma & 07777) | ifr;
			enab_rtf = 1;
			pc = cpma & 07777;
			break;

		case opJMP | DEFER | ZERO:
			PAGE_ZERO;
			DEFER_CYCLE;
			ifr = ib;
			uf = ub;
			cpma = (cpma & 07777) | ifr;
			enab_rtf = 1;
			pc = cpma & 07777;
			break;

		case opJMP | DEFER | CURRENT:
			CURRENT_PAGE;
			DEFER_CYCLE;
			ifr = ib;
			uf = ub;
			cpma = (cpma & 07777) | ifr;
			enab_rtf = 1;
			pc = cpma & 07777;
			break;

/* undo kluge to force branches to use instruction field */
#undef dfr

		case opIOT | DIRECT | ZERO:
		case opIOT | DIRECT | CURRENT:
		case opIOT | DEFER | ZERO:
		case opIOT | DEFER | CURRENT:

			pc = (pc + 1) & 07777;
			if (uf == 1) { /* illegal in user mode */
				irq = irq + 1;
				km8e_uif = 1;
				break; /* abort instruction */
			}

			switch ((mb >> 3) & 077) { /* decode device address */

			case 000:
				switch (mb & 07) { /* decode CPU IOTs */
				case 00: /* SKON */
					if (enab != 0) {
						pc = (pc + 1) & 07777;
					}
					enab = 0;
					enab_del = 0;
					break;
				case 01: /* ION */
					enab = 1;
					break;
				case 02: /* IOF */
					enab = 0;
					enab_del = 0;
					break;
				case 03: /* SRQ */
					if (irq > 0) {
						pc = (pc + 1) & 07777;
					}
					break;
				case 04: /* GTF */
					ac = (lnk >> 1)       /* bit 0 */
// KE8E				   | (gt?)	       /* bit 1 KE8E */
					   | ((irq > 0) << 9)  /* bit 2 */
					   | (0)	/*?*/  /* bit 3 */
					   | (enab << 7)       /* bit 4 */
					   | sf 	       /* bit 5-11 */
					;
					break;
				case 05: /* RTF */
					lnk = (ac<<1)& 010000; /* bit 0 */
// KE8E				gt = ?			/* bit 1 KE8E */
					/* nothing */		/* bit 2 */
					/* nothing */		/* bit 3 */
					enab = 1;		/* bit 4 */
					ub = (ac & 00100) >> 6; /* bit 5 */
					ib = (ac & 00070) << 9; /* bit 6-8 */
					dfr = (ac & 00007) << 12;/* bit 9-11 */// #endif
					/* disable interrupts until branch */
					enab_rtf = 0;
					break;
				case 06: /* SGT */
// KE8E				if (?) {
// KE8E					pc = (pc + 1) & 07777;
// KE8E				}
					break;
				case 07: /* CAF */
					clearflags();
					break;
				}
				break;

			// case 001:	// High Speed Tape
			// 	pc8edev1(mb & 07);
			// 	break;
			// case 002:	// High Speed Tape
			// 	pc8edev2(mb & 07);
			// 	break;

			case 003:	// TTY Keyboard
				kl8edev3(mb & 07);
				break;
			case 004:	// TTY Printer
				kl8edev4(mb & 07);
				break;

			// case 013:	// RTC
			// 	dk8edev(mb & 07);
			// 	break;

			// case 020:	// Memory Management
			// case 021:
			// case 022:
			// case 023:
			// case 024:
			// case 025:
			// case 026:
			// case 027:
			// 	km8edev(mb & 077);
			// 	break;

			// case 063:	// Card reader
			// 	cr8fdev3(mb & 07);
			// 	break;
			// case 067:	// Card reader
			// 	cr8fdev7(mb & 07);
			// 	break;

			// case 075:	// Floppy drive
			// 	rx8edev(mb & 07);
			// 	break;

			default: /* non existant device */
				break;
			}
			break;


		case opOPR | DIRECT | ZERO:	/* group 1, CLA = 0 */
		case opOPR | DIRECT | CURRENT:	/* group 1, CLA = 1 */

			pc = (pc + 1) & 07777;
			switch ((mb >> 4) & 017) { /* decode CLA ... CML here */

			case 000:	/* NOP */
				break;

			case 001:	/*             CML */
				lnk = lnk ^ 010000; 
				break;

			case 002:	/*         CMA     */
				ac = ac ^ 007777;
				break;

			case 003:	/*         CMA CML */
				ac = ac ^ 007777;
				lnk = lnk ^ 010000; 
				break;

			case 004:	/*     CLL         */
				lnk = 000000; 
				break;

			case 005:	/*     CLL     CML */
				lnk = 010000; 
				break;

			case 006:	/*     CLL CMA     */
				ac = ac ^ 007777;
				lnk = 000000; 
				break;

			case 007:	/*     CLL CMA CML */
				ac = ac ^ 007777;
				lnk = 010000; 
				break;

			case 010:	/* CLA             */
				ac = 00000;
				break;

			case 011:	/* CLA         CML */
				ac = 00000;
				lnk = lnk ^ 010000; 
				break;

			case 012:	/* CLA     CMA     */
				ac = 07777;
				break;

			case 013:	/* CLA     CMA CML */
				ac = 07777;
				lnk = lnk ^ 010000; 
				break;

			case 014:	/* CLA CLL         */
				ac = 00000;
				lnk = 000000;
				break;

			case 015:	/* CLA CLL     CML */
				ac = 00000;
				lnk = 010000;
				break;

			case 016:	/* CLA CLL CMA     */
				ac = 07777;
				lnk = 000000;
				break;

			case 017:	/* CLA CLL CMA CML */
				ac = 07777;
				lnk = 010000;
				break;
			}

			if (mb & 00001) { /* IAC */
				ac = (ac | lnk) + 1;
				lnk = ac & 010000;
				ac = ac & 007777;
			}

			switch ((mb >> 1) & 07) { /* decode RAR,RAL,TWO */

			case 00:	/* NOP */
				break;

			case 01:	/*         TWO -- BSW */
				ac = ((ac & 07700) >> 6)
				   | ((ac & 00077) << 6);
				break;

			case 02:	/*     RAL     */
				ac = (ac << 1) | (lnk >> 12);
				lnk = ac & 010000;
				ac = ac & 007777;
				break;

			case 03:	/*     RAL TWO */
				ac = (ac << 2) | ((ac | lnk) >> 11);
				lnk = ac & 010000;
				ac = ac & 007777;
				break;

			case 04:	/* RAR         */
				ac = ((ac | lnk) >> 1) | (ac << 12);
				lnk = ac & 010000;
				ac = ac & 007777;
				break;

			case 05:	/* RAR     TWO */
				ac = ((ac | lnk) >> 2) | (ac << 11);
				lnk = ac & 010000;
				ac = ac & 007777;
				break;

			case 06:	/* RAR RAL     */
				/* this uses a data path meant for AND */
				ac = ac & mb;
				break;

			case 07:	/* RAR RAL TWO */
				/* this uses an addressing data path */
				ac = ((pc - 1) & 07600) | (mb & 00177);
				break;

			}

			break;

		case opOPR | DEFER | ZERO:	/* group 2,3 CLA = 0 */
		case opOPR | DEFER | CURRENT:   /* group 2,3 CLA = 1 */

			if ((mb & 00001) == 0) { /* GROUP 2 */

				pc = (pc + 1) & 07777;

				switch ((mb & 00170) >> 3) { /* SMA ... REV */

				case 000: /* NOP */
					break;

				case 001: /*             REV */
					pc = (pc + 1) & 07777;
					break;

				case 002: /*         SNL     */
					if (lnk) {
						pc = (pc + 1) & 07777;
					}
					break;

				case 003: /*         SNL REV */
					if (lnk == 0) {
						pc = (pc + 1) & 07777;
					}
					break;

				case 004: /*     SZA         */
					if (ac == 0) {
						pc = (pc + 1) & 07777;
					}
					break;

				case 005: /*     SZA     REV */
					if (ac) {
						pc = (pc + 1) & 07777;
					}
					break;

				case 006: /*     SZA SNL     */
					if ((ac == 0) || lnk) {
						pc = (pc + 1) & 07777;
					}
					break;

				case 007: /*     SZA SNL REV */
					if (ac && (lnk == 0)) {
						pc = (pc + 1) & 07777;
					}
					break;

				case 010: /* SMA             */	
					if (ac & 04000) {
						pc = (pc + 1) & 07777;
					}
					break;

				case 011: /* SMA         REV */
					if ((ac & 04000) == 0) {
						pc = (pc + 1) & 07777;
					}
					break;

				case 012: /* SMA     SNL     */
					if ((ac | lnk) & 014000) {
						pc = (pc + 1) & 07777;
					}
					break;

				case 013: /* SMA     SNL REV */	
					if (((ac | lnk) & 014000) == 0) {
						pc = (pc + 1) & 07777;
					}
					break;

				case 014: /* SMA SZA         */
					if ((ac & 04000) || (ac == 0)) {
						pc = (pc + 1) & 07777;
					}
					break;

				case 015: /* SMA SZA     REV */
					if (((ac & 004000) == 0) && ac) {
						pc = (pc + 1) & 07777;
					}
					break;

				case 016: /* SMA SZA SNL     */
					if (((ac | lnk) & 014000)
					 || (ac == 0)) {
						pc = (pc + 1) & 07777;
					}
					break;

				case 017: /* SMA SZA SNL REV */	
					if ((((ac | lnk) & 014000) == 0)
					 && (ac)) {
						pc = (pc + 1) & 07777;
					}
					break;
				}

				if (mb & 00200) { /* CLA */
					ac = 00000;
				}

				if ((uf != 0) && ((mb & 00006) != 0)) {
					/* illegal in user mode */
					irq = irq + 1;
					km8e_uif = 1;
					break; /* abort instruction */
				}

				if (mb & 00004) { /* OSR */
					ac = ac | sr;
				}

				if (mb & 00002) { /* HLT */
					run=RUNMODE_HLT;
					countdown = 0;
				}

			} else { /* GROUP 3 */
				pc = (pc + 1) & 07777;

				if (mb & 00200) { /* CLA */
					ac = 00000;
				}

				if ((mb & 00120) == 00100) { /* MQA */
					ac = mq | ac;
				} else if ((mb & 00120) == 00020) { /* MQL */
					mq = ac;
					ac = 00000;
				} else if ((mb & 00120) == 00120) { /*MQA,MQL*/
					int temp;
					temp = mq;
					mq = ac;
					ac = temp;
				}
// KE8E			if (EAEmode == 0) { /* mode A */
// KE8E				if (mb & 00040) { /* mode A SCA */
// KE8E					ac |= sc;
// KE8E				}
// KE8E				switch ((mb & 00016) >> 1) {
// KE8E				case 00: /* NOP */
// KE8E					break;
// KE8E				case 01: /* SCL */
// KE8E					cpma = pc | ifr;
// KE8E					mb = memory[cpma];
// KE8E					pc = (pc + 1) & 07777;
// KE8E					sc = (~mb) & 00037;
// KE8E					break;
// KE8E				case 02: /* MUY */
// KE8E					cpma = pc | ifr;
// KE8E					mb = memory[cpma];
// KE8E					pc = (pc + 1) & 07777;
// KE8E					{
// KE8E						long int prod = mp * mb;
// KE8E						mq = prod & 07777;
// KE8E						ac = (prod>>12) & 07777;
// KE8E						lnk = 000000;
// KE8E					}
// KE8E					sc = 013;
// KE8E					break;
// KE8E				case 03: /* DVI */
// KE8E					cpma = pc | ifr;
// KE8E					mb = memory[cpma];
// KE8E					pc = (pc + 1) & 07777;
// KE8E					if (ac < mb) { /* no overflow */
// KE8E						long int idend;
// KE8E						idend = (ac << 12) | mq;
// KE8E						mq = idend / mb;
// KE8E						ac = idend % mb;
// KE8E						lnk = 000000;
// KE8E					} else { /* overflow */
// KE8E						/* --- mq = ?? --- */
// KE8E						ac = ac - mb;
// KE8E						/* shift ac-mq-lnk */
// KE8E						lnk = 010000;
// KE8E						/* --- shift ?? --- */
// KE8E					}
// KE8E					sc = 014;
// KE8E					break;
// KE8E				case 04: /* NMI or SWAB */
// KE8E					if ((mb & 00060) == 020) {
// KE8E					    /* SWAB */
// KE8E					    EAEmode = 1
// KE8E					} else {
// KE8E					    /* NMI */
// KE8E					    long int shift, news;
// KE8E					    shift = (lnk | ac)<<12;
// KE8E					    shift |= mq;
// KE8E					    sc = 0;
// KE8E					    do (;;) {
// KE8E						news = shift << 1;
// KE8E					        if(!(news & 027777777))
// KE8E						    break;
// KE8E						if ( (news^shift)
// KE8E						   & 040000000 ) break;
// KE8E						shift = news;
// KE8E						sc ++;
// KE8E					    }
// KE8E					    mq = shift & 07777;
// KE8E					    shift >>= 12;
// KE8E					    ac = shift & 07777;
// KE8E					    lnk = shift & 010000;
// KE8E					}
// KE8E					break;
// KE8E				case 05: /* SHL */
// KE8E					cpma = pc | ifr;
// KE8E					mb = memory[cpma];
// KE8E					pc = (pc + 1) & 07777;
// KE8E					sc = (~mb) & 00037;
// KE8E					}
// KE8E					    long int shift;
// KE8E					    shift = (lnk | ac)<<12;
// KE8E					    shift |= mq;
// KE8E					    do (;;) {
// KE8E						shift <<= 1;
// KE8E						sc ++;sc &= 037;
// KE8E						if (sc == 0) break;
// KE8E					    }
// KE8E					    mq = shift & 07777;
// KE8E					    shift >>= 12;
// KE8E					    ac = shift & 07777;
// KE8E					    lnk = shift & 010000;
// KE8E					}
// KE8E					break;
// KE8E				case 06: /* ASR */
// KE8E					cpma = pc | ifr;
// KE8E					mb = memory[cpma];
// KE8E					pc = (pc + 1) & 07777;
// KE8E					sc = (~mb) & 00037;
// KE8E					}
// KE8E					    long int shift;
// KE8E					    shift = (ac<<12)|mq;
// KE8E					    lnk = (ac<<1)&010000;
// KE8E					    do (;;) {
// KE8E						shift=(lnk|shift)>>1;
// KE8E						sc ++;sc &= 037;
// KE8E						if (sc == 0) break;
// KE8E					    }
// KE8E					    mq = shift & 07777;
// KE8E					    shift >>= 12;
// KE8E					    ac = shift & 07777;
// KE8E					}
// KE8E					break;
// KE8E				case 07: /* LSR */
// KE8E					cpma = pc | ifr;
// KE8E					mb = memory[cpma];
// KE8E					pc = (pc + 1) & 07777;
// KE8E					sc = (~mb) & 00037;
// KE8E					}
// KE8E					    long int shift;
// KE8E					    shift = (ac<<12)|mq;
// KE8E					    do (;;) {
// KE8E						shift >>= 1;
// KE8E						sc ++;sc &= 037;
// KE8E						if (sc == 0) break;
// KE8E					    }
// KE8E					    mq = shift & 07777;
// KE8E					    shift >>= 12;
// KE8E					    ac = shift & 07777;
// KE8E					    lnk = 0;
// KE8E					}
// KE8E					break;
// KE8E				}
// KE8E			} else { /* mode B */
// KE8E			    if ((mb & 00040) == 0) { /* CLASS 1 */
// KE8E				switch ((mb & 00016) >> 1) {
// KE8E				case 00: /* NOP */
// KE8E					break;
// KE8E				case 01: /* ACS */
// KE8E					sc = ac & 037;
// KE8E					break;
// KE8E				case 02: /* */
// KE8E					break;
// KE8E				case 03: /* */
// KE8E					break;
// KE8E				case 04: /* */
// KE8E					break;
// KE8E				case 05: /* SAM */
// KE8E					ac = mq - ac;
// KE8E					break;
// KE8E				case 06: /* */
// KE8E					break;
// KE8E				case 07: /* */
// KE8E					break;
// KE8E				}
// KE8E			    } else { /* CLASS 2 */
// KE8E				switch ((mb & 00016) >> 1) {
// KE8E				case 00: /* SCA */
// KE8E					ac = ac | sc;
// KE8E					break;
// KE8E				case 01: /* */
// KE8E					break;
// KE8E				case 02: /* */
// KE8E					break;
// KE8E				case 03: /* */
// KE8E					break;
// KE8E				case 04: /* */
// KE8E					break;
// KE8E				case 05: /* */
// KE8E					break;
// KE8E				case 06: /* */
// KE8E					break;
// KE8E				case 07: /* */
// KE8E					break;
// KE8E				}
// KE8E			}
 			}
 		}
	if (run==RUNMODE_STARTING) run=RUNMODE_RUNNING;
 	}
 }
 