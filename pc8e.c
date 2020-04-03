/* File: pc8e.c
   Author: Douglas Jones, Dept. of Comp. Sci., U. of Iowa, Iowa City, IA 52242.
   Date: Feb. 29, 1996
   Language: C (UNIX)
   Purpose: DEC PC8/E high speed paper tape reader-punch emulator,

   Based on the description in the PDP-8/E Small Computer Handbook,
   Digital Equipment Corporation, 1971.

*/

#include <stdio.h>
#include "realtime.h"
#include "bus.h"
#include "utility.h"
#include "kc8m.h"

/***********/
/* options */
/***********/

/* The advertised speed of the punch is 50 chars/sec. */
#define punch_time (20 * millisecond)

/* The advertised speed of the reader is 300 chars/sec. */
#define reader_time (3333 * microsecond)


/*********************************************************/
/* Interface between device implementation and "console" */
/*********************************************************/

/* files used to simulate the device */
static FILE *punch_stream;
static char punchname[NAME_LENGTH];
static FILE *reader_stream;
static char readername[NAME_LENGTH];

/* timers used to simulate delays between I/O initiation and completion */
static struct timer punch_delay;
static struct timer reader_delay;

static void readerclose(int u)
{
	if (reader_stream != NULL) {
		fclose( reader_stream );
		reader_stream = NULL;
		readername[0] = '\0';
	}
}

static int readeropen(int u, char * f)
{
	readerclose(u);
	set_file_name( readername, f );
	if ((reader_stream = fopen( readername, "r" )) == NULL) {
		readername[0] = '\0';
	}
	return (reader_stream != NULL);
}

static void punchclose(int u)
{
	if (punch_stream != NULL) {
		fclose( punch_stream );
		punch_stream = NULL;
		punchname[0] = '\0';
	}
}

static int punchopen(int u, char * f)
{
	punchclose(u);
	set_file_name( punchname, f );
	if ((punch_stream = fopen( punchname, "w" )) == NULL) {
		punchname[0] = '\0';
	}
	return (punch_stream != NULL);
}

void pc8epower(void) /* power-on initialize */
{
	punch_stream = NULL;
	reader_stream = NULL;
	punchname[0] = '\0';
	readername[0] = '\0';
	init_timer( punch_delay );
	init_timer( reader_delay );
	register_device( readeropen, readerclose, 0,
			 "PTR", "-- high-speed paper-tape reader ",
			 readername );
	register_device( punchopen, punchclose, 0,
			 "PTP", "-- high-speed paper-tape punch  ",
			 punchname );
}


/*************************************/
/* "officially visible" device state */
/*************************************/

static int punch_flag;
static int punch_buffer;
static int reader_flag;
static int reader_buffer;
static int interrupt_enable;

/*************************/
/* Device implementation */
/*************************/

static void reader_event(int p)
{ /* called from timer when a byte has been successfully read */
	if (reader_stream != NULL) {
		reader_buffer = getc(reader_stream);
		if (reader_buffer == EOF) {
			/* simulate the tape running out in the reader */
			readerclose(0);
			reader_buffer = 0377;
		};
	} else {
		/* simulate reading with no tape mounted */
		reader_buffer = 0377; /* all holes */
	}
	if (interrupt_enable == 1) {
		irq = (irq - reader_flag) + 1;
	}
	reader_flag = 1; /* signal that read is complete */
}

static void read_character(void)
{ /* schedule the completion of a read "reader_time" in the future */
	schedule( &reader_delay, reader_time, reader_event, 0 );
}


static void punch_event(int p)
{ /* called from timer when a byte has been successfully printed */
	if (punch_stream != NULL) {
		putc(punch_buffer, punch_stream);
	} else {
		/* simulate punching with no tape in punch */
	}
	if (interrupt_enable == 1) {
		irq = (irq - punch_flag) + 1;
	}
	punch_flag = 1;
}

static void punch_character(void)
{ /* schedule the completion of a punch "punch_time" in the future */
	schedule( &punch_delay, punch_time, punch_event, 0 );
}



/***********************************************/
/* Initialization used by CAF and reset switch */
/***********************************************/

void pc8einit(void) /* console reset */
{
	punch_flag = 0;
	reader_flag = 0;
	interrupt_enable = 1;
	/* assume that cpu clears irq for us */
}

/********************/
/* IOT Instructions */
/********************/

void pc8edev1(int op)
{
	switch (op) {
	case 00: /* RPE */
		if (interrupt_enable == 0) {
			interrupt_enable = 1;
			irq = irq + reader_flag + punch_flag;
		}
		break;
	case 01: /* RSF */
		if (reader_flag == 1) {
			pc = (pc + 1) & 07777;
		}
		break;
	case 02: /* RRB */
		ac = ac | reader_buffer;
		reader_flag = 0;
		break;
	case 03: /* no operation! */
		break;
	case 04: /* RFC */
		if (interrupt_enable == 1) {
			irq = irq - reader_flag;
		}
		reader_flag = 0;
		read_character();
		break;
	case 05: /* no operation! */
		break;
	case 06: /* RRB RFC */
		ac = ac | reader_buffer;
		if (interrupt_enable == 1) {
			irq = irq - reader_flag;
		}
		reader_flag = 0;
		read_character();
		break;
	case 07: /* no operation! */
		break;
	}
}

void pc8edev2(int op)
{
	switch (op) {
	case 00: /* PCE */
		if (interrupt_enable == 1) {
			interrupt_enable = 0;
			irq = irq - (reader_flag + punch_flag);
		}
		break;
	case 01: /* PSF */
		if (punch_flag == 1) {
			pc = (pc + 1) & 07777;
		}
		break;
	case 02: /* PCF */
		if (interrupt_enable == 1) {
			irq = irq - punch_flag;
		}
		punch_flag = 0;
		break;
	case 03: /* no operation! */
		break;
	case 04: /* PPC */
		punch_buffer = ac & 00377;
		punch_character();
		break;
	case 05: /* no operation! */
		break;
	case 06: /* PCF PPC */
		if (interrupt_enable == 1) {
			irq = irq - punch_flag;
		}
		punch_flag = 0;
		punch_buffer = ac & 00377;
		punch_character();
		break;
	case 07: /* no operation! */
		break;
	}
}
