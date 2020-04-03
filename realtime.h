/* realtime.h
   Author: Douglas Jones, Dept. of Comp. Sci., U. of Iowa, Iowa City, IA 52242.
   Date: Nov. 13, 1995
   Language: C (UNIX)
   Purpose: interface to simulation core routines for real-time behavior
            of devices attached to instruction-set level computer emulators.
*/

/* The following trick puts extern on definitions if not in the main program */
#ifdef MAIN
#define EXTERN
#else
#define EXTERN extern
#endif


/**********************************************/
/* Times are measure in 200 nanosecond ticks. */
/**********************************************/

#define microsecond 5l
#define millisecond 5000l
#define second 5000000l

/**************************************************************************/
/* Using 32 bit signed integers, the maximum time is 2**31 - 1 ticks,     */
/* the conversion to seconds is (ns/tick)(sec/ns)(ticks) = sec,           */
/* so, a 32 bit clock can hold (200)(10**-9)(2**31-1) sec = 7.158 min     */
/*                                                                        */
/* This is not much running time, so scheduled events are always recorded */
/* as delays from the present, and no peripheral activity may be set to   */
/* happen more than 7 minutes into the future (this is plenty long for    */
/* such things as rewinding a reel of tape, one of the slowest actions.   */
/**************************************************************************/

/**************************************************************************/
/* For each device for which there may be asynchronous activity, a timer  */
/* needs to be allocated.  This timer is set whenever the activity is     */
/* caused, and when the timer expires, it calls the appropriate service   */
/* to simulate the activity in question                                   */
/**************************************************************************/

struct timer {
	long int delay;      /* the delay until the timer is to fire */
	void (* action)();   /* the function to call when the timer fires */
	int param;           /* parameter to action */
	struct timer * next; /* the next timer to worry about after this */
};

/* each timer must be initialized before the first time it is scheduled */
#define init_timer(t) t.delay = -1

void init_timers(); /* initialize entire timer subsystem */

void schedule(  /* schedule timer t after a delay of d ticks */
	struct timer * t,
	long int d,
	void (* a)(),
	int p
);

void fire_timer(); /* cause timer to fire at current time */

EXTERN long int countdown; /* the delay until the next timer expiration */

/**************************************************************************/
/* It is up to the emulator to decrement countdown appropriately as each  */
/* machine instruction is interpreted, thus recording the passage of time */
/* The emulator is also responsible for calling fire_timer if countdown   */
/* ever reaches zero.                                                     */
/**************************************************************************/
