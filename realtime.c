/* File: realtime.c
   Author: Douglas Jones, Dept. of Comp. Sci., U. of Iowa, Iowa City, IA 52242.
   Date: July 26, 1995
   Language: C (UNIX)
   Purpose: simulation core routines for real-time behavior of devices
            attached to instruction-set level computer emulators.
*/

#include "realtime.h"

long int countdown; /* the delay until the next timer expiration */

/************************************************/
/* Read the comments in the include file first! */
/************************************************/

/**************************************************************************/
/* Timers that have been scheduled are stored in a linked list, ordered   */
/* by their delay from the present.  The delay in each timer record after */
/* the first is the delay between the preceeding event and that event.    */
/* The delay of the first scheduled timer is not recorded in that timer,  */
/* but is stored in countdown, which is periodically decremented.         */
/**************************************************************************/

#define niltimer (struct timer *)0

static struct timer * head; /* points to the next timer in the queue */

void init_timers(void)  /* initialize timer subsystem */
{
	head = niltimer; /* there is no pending timer initially */
	countdown = 0x7FFFFFFF; /* so put off next firing a long time */
}

void schedule(  /* schedule timer t after a delay of d ticks */
	struct timer * t,
	long int d,
	void (* a)(),
	int p
)
{
	t->action = a;
	t->param = p;
	if (t->delay >= 0) { /* timer is already scheduled */
		/* don't mess up the queue, schedule becomes no-op */
	} else if (head == niltimer) { /* nothing is scheduled yet */
		countdown = d;
		t->delay = d;
		t->next = niltimer;
		head = t;
	} else { /* there is an existing schedule */
		if (countdown > d) {    /* t goes before old head */
			head->delay = countdown - d;
			countdown = d;    /* set timer until t */
			t->next = head;   /* link t ahead of old head */
			t->delay = d;     /* mark timer as queued */
			head = t;	  /* reset list head */
		} else { /* new event goes down into queue */
			struct timer * i;
			i = head;
			d = d - countdown; /* delay relative to head */
			while (i->next != niltimer) { /* scan list for place */
				if (d < i->next->delay) {
					i->next->delay = i->next->delay - d;
					break;
				}
				i = i->next;
				d = d - i->delay; /* adjust delay */
			}
			t->next = i->next; /* link new timer into queue */
			t->delay = d;
			i->next = t;
		}
	}
}

long int query_timer(struct timer * t)  /* find how much time remains on t */
{
	if (t->delay < 0) { /* timer is not in queue */
		return -1;
	} else {
		long int query = countdown;
		struct timer * i = head;
		while (i != t) { /* scan list for t */
			i = i->next;
			query = query + i->delay; /* accumulate delays */
		}
		return query;
	}
}

void fire_timer(void)  /* cause timer to fire at current time */
{
	if (head == niltimer) { /* no pending timer */
		/* put off next firing as long as possible */
		countdown = 0x7FFFFFFF;
	} else {
		void (* a)();
		int p;

		/* save action for use after dequeue */
		a = head->action;
		p = head->param;

		/* mark head timer as idle */
		head->delay = -1;

		/* schedule next event */
		head = head->next;
		if (head == niltimer) { /* no new timer */
			/* put off next firing as long as possible */
			countdown = 0x7FFFFFFF;
		} else {
			/* figure delay till next timer */
			countdown = countdown + head->delay;
			/* note: this could have been countdown = head->delay,
			   except that countdown could have gone slightly
			   negative, and we want to make delays add nicely
			   for such devices as line frequency clocks */
		}

		/* fire action on head timer */
		(* a)(p);
	}
}
