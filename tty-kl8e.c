/* File: kl8e.c
   Author: Douglas Jones, Dept. of Comp. Sci., U. of Iowa, Iowa City, IA 52242.
   Date: Feb. 29, 1996
   Updates: Mats Engstrom, April 2020
   Language: C (UNIX)
   Purpose: DEC KL8/E console teletype emulator, for the console device only!
            Emulators for other asynchronous interfaces must be constructed
            separately.

   Based on the description in the PDP-8/E Small Computer Handbook,
   Digital Equipment Corporation, 1971.

   Uses nonblocking polling to grab characters from the keyboard.  This is
   a bit too synchronous a style of reading, but until ttyaccess.c can be
   made to use interrupts, it will do.  As a result, this should look to
   software as if it is reading from a synchronous data line interface
   that automatically discards pad characters.  Users who type too fast
   will note that their input-overruns lead to lost data.
*/

#include <stdio.h>
#include <unistd.h>
#include "realtime.h"
#include "bus.h"
#include "ttyaccess.h"

/*********************************************************************/
/* options:  The user may change the speed of the simulated teletype */
/* to any positive value.  DEC sold the KL8E in the following        */
/* standard versions with an RS232 interface:                        */
/*                                                                   */
/*   KL8E option:           A     B     C     D     E     F     G    */
/*   M8650 version:         -     YA    YA    YA    YA    YA    YA   */
/*   Transmit baud rate:   110   150   300   600  1200  1200  2400   */
/*   Receive baud rate:    110   150   300   600  1200   150   150   */
/*   Receive baud rate:    110   150   300   600  1200   150   150   */
/*                                                                   */
/* In fact, the M8650 YA board could be jumpered to handle 2400,     */
/* 4800 and 9600 baud on both input and output, but DEC didn't tell  */
/* people about this, at least not in the manual.                    */
/*********************************************************************/

/* translations from baud rate to time per character */

#define baud110  (100    * millisecond)
#define baud150  ( 66667 * microsecond)
#define baud300  ( 33333 * microsecond)
#define baud600  ( 16667 * microsecond)
#define baud1200 (  8333 * microsecond)
#define baud2400 (  4166 * microsecond)
#define baud4800 (  2083 * microsecond)
#define baud9600 (  1041 * microsecond)
#define baudfpga (   950 * microsecond)
#define baudmax  (     1 * microsecond)

/* select the baud rate here (note: if the emulator runs at 1/10 the speed
   of the real PDP-8, 1200 baud simulation will look like 110 to a person! */

#define print_time baudfpga
#define read_time baudfpga


/****************************************************************/
/* restrictions:  This version of the console has no low-speed  */
/* paper-tape reader or punch -- call it a KSR teletype instead */
/* of the usual ASR teletype.                                   */
/****************************************************************/


/*********************************************************/
/* Interface between device implementation and "console" */
/*********************************************************/

/* timers used to simulate delays between I/O initiation and completion */
static struct timer print_delay;
static struct timer read_delay;


/*************************************/
/* "officially visible" device state */
/*************************************/

static int keyboard_flag;
static int keyboard_buffer;
static int interrupt_enable;
static int print_flag;
static int print_buffer;


/*************************/
/* Device Implementation */
/*************************/

static void keyboard_event(int p) /* called to poll for keyboard input */
{
	int poll;

	if ((poll = ttypoll()) >= 0) { /* a key was pressed */
		/* Follow the DEC convention of setting the high bit */
		keyboard_buffer = poll | 0200;

		/* report the keypress to the rest of the emulator */
		if (keyboard_flag != 1) { /* no overrun condition */
			keyboard_flag = 1;
			if (interrupt_enable == 1) {
				irq = irq + 1;
			}
		}
	}

	schedule( &read_delay, read_time, keyboard_event, 0 );
}

static void read_character(void)
{ /* called to enable keyboard_event when reading from tape */
	/* right now, tape is not supported, so this does nothing */
}

static void print_event(int p)
{ /* called from timer when a byte has been successfully printed */
	/* this code allows for the DEC convention of setting the high bit */
	unsigned char ch=print_buffer & 0177;
	write(STDOUT_FILENO, &ch, 1);
	print_flag = 1;
	if (interrupt_enable == 1) {
		irq = irq + 1;
	}
}

static void print_character(void)
{ /* schedule the completion of a print "print_time" in the future */
	schedule( &print_delay, print_time, print_event, 0 );
}


/******************/
/* Initialization */
/******************/

void kl8epower(void) /* global initialize */
{
	/* set up timers used to delay I/O activity */
        init_timer(print_delay);
        init_timer(read_delay);

	/* the following makes the reader run forever (probably wrong) */
	schedule( &read_delay, read_time, keyboard_event, 0 );
}


void kl8einit(void) /* console reset */
{
	keyboard_flag = 0;
	print_flag = 0;
	interrupt_enable = 1;
}

/********************/
/* IOT Instructions */
/********************/

void kl8edev3(int op)
{
	switch (op) {
	case 00: /* KCF */
		if (interrupt_enable == 1) {
			irq = irq - keyboard_flag;
		}
		keyboard_flag = 0;
		break;
	case 01: /* KSF */
		if (keyboard_flag == 1) {
			pc = (pc + 1) & 07777;
		}
		break;
	case 02: /* KCC */
		if (interrupt_enable == 1) {
			irq = irq - keyboard_flag;
		}
		keyboard_flag = 0;
		ac = 00000;
		read_character();
		break;
	case 03: /* no operation! */
		break;
	case 04: /* KRS */
		ac = ac | keyboard_buffer;
		break;
	case 05: /* KIE != KSF KRS */
		if ((ac & 00001) == 0) { /* disable interrupts */
			if (interrupt_enable == 1) {
				interrupt_enable = 0;
				irq = irq - (keyboard_flag + print_flag);
			}
		} else { /* enable interrupts */
			if (interrupt_enable == 0) {
				interrupt_enable = 1;
				irq = irq + (keyboard_flag + print_flag);
			}
		}
		break;
	case 06: /* KRB = KCC KRS */
		if (interrupt_enable == 1) {
			irq = irq - keyboard_flag;
		}
		keyboard_flag = 0;
		ac = keyboard_buffer;
		read_character();
		break;
	case 07: /* no operation! */
		break;
	}
}

void kl8edev4(int op)
{
	switch (op) {
	case 00: /* TFL */
		if (print_flag == 0) {
			print_flag = 1;
			if (interrupt_enable == 1) {
				irq = irq + 1;
			}
		}
		break;
	case 01: /* TSF */
		if (print_flag == 1) {
			pc = (pc + 1) & 07777;
		}
		break;
	case 02: /* TCF */
		if (interrupt_enable == 1) {
			irq = irq - print_flag;
		}
		print_flag = 0;
		break;
	case 03: /* no operation! */
		break;
	case 04: /* TPC */
		print_buffer = ac & 00377;
		print_character();
		break;
	case 05: /* TSK != TSF TPC */
		if ((print_flag == 1) || (keyboard_flag == 1)) {
			pc = (pc + 1) & 07777;
		}
		break;
	case 06: /* TLS = TCF TPC */
		if (interrupt_enable == 1) {
			irq = irq - print_flag;
		}
		print_flag = 0;
		print_buffer = ac & 00377;
		print_character();
		break;
	case 07: /* no operation! */
		break;
	}
}
