/* File: dk8e.c
   Author: Douglas Jones, Dept. of Comp. Sci., U. of Iowa, Iowa City, IA 52242.
   Date: July 26, 1995
   Language: C (UNIX)
   Purpose: DEC DK8/EA or /EC real time clock emulator,

   Based on the description in the PDP-8/E Small Computer Handbook,
   Digital Equipment Corporation, 1971.

*/

#include "realtime.h"
#include "bus.h"

/*****************************************************/
/* options:  The user may change the frequency of    */
/* simulated real time clock.  The speeds available  */
/* were as follows:                                  */
/*                                                   */
/*   DK8EA Line Frequency Clock                      */
/*     with 60 hz power, 120 ticks/second            */
/*     with 50 hz power, 100 ticks/second            */
/*                                                   */
/*   DK8EC Crystal Clock                             */
/*     1, 50, 500, 5000 ticks/second                 */
/*****************************************************/

#define ticks1 (1000 * millisecond)
#define ticks50 (20 * millisecond)
#define ticks100 (10 * millisecond)
#define ticks120 (8333 * microsecond)
#define ticks500 (2 * millisecond)
#define ticks5000 (200 * microsecond)

/* DK8/EA at 60hz */
#define tick_time = ( ticks120 / IOFUDGE )


/*********************************************************/
/* Interface between device implementation and "console" */
/*********************************************************/

/* timer used to gain control every tick */
static struct timer tick_delay;


/*************************************/
/* "officially visible" device state */
/*************************************/

static int tick_flag;
static int interrupt_enable;


/*************************/
/* Device implementation */
/*************************/

static void tick_event(p)
int p;
{ /* called from timer when a the real-time clock ticks */
	if (tick_flag == 0) {
		/* clock ticks are no-ops if the flag is already set */
		tick_flag = 1;
		if (interrupt_enable == 1) {
			irq = irq + 1;
		}
	}
	schedule( &tick_delay, tick_time, &tick_event, 0 );
}


/***********************************************/
/* Initialization used by CAF and reset switch */
/***********************************************/

dk8epower() /* power-on initialize */
{
	init_timer(tick_delay);
	schedule( &tick_delay, tick_time / 2, &tick_event, 0 );
}

dk8einit() /* console reset */
{
	tick_flag = 0;
	interrupt_enable = 1;
	/* assume that cpu clears irq for us */
}

/********************/
/* IOT Instructions */
/********************/

dk8edev(op)
int op;
{
	switch (op) {
	case 00: /* no operation */
		break;
	case 01: /* CLEI */
		if (interrupt_enable == 0) {
			interrupt_enable = 1;
			irq = irq + tick_flag;
		}
		break;
	case 02: /* CLDI */
		if (interrupt_enable == 1) {
			interrupt_enable = 0;
			irq = irq - tick_flag;
		}
		break;
	case 03: /* CLSK */
		if (tick_flag == 1) {
			pc = pc + 1;
			tick_flag = 0;
		}
		break;
	case 03: /* no operation! */
	case 04: /* no operation! */
	case 05: /* no operation! */
	case 06: /* no operation! */
	case 07: /* no operation! */
		break;
	}
}
