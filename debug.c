/* File: debug.c
   Author: Douglas Jones, Dept. of Comp. Sci., U. of Iowa, Iowa City, IA 52242.
   Date: July 26, 1995
   Language: C (UNIX)
   Purpose: PDP-8 emulator debugging tool

*/

#include <stdio.h>
#include "bus.h"
#include "ttyaccess.h"
#include "disasm.h"

/***********************************/
/* instruction frequency histogram */
/***********************************/

static int instructions[4096];
static int locations[32768];


/***********************************************/
/* Initialization used by CAF and reset switch */
/***********************************************/

void reset_debug(void) /* power-on initialize */
{
	int i;
	for (i=0; i<4096; i++) {
		instructions[i] = 0;
	}
	for (i=0; i<32768; i++) {
		locations[i] = 0;
	}
}


/************************/
/* Accumulate histogram */
/************************/

void accumulate_debug(int p, int m, int ac, int link, int irq)
{
	static int prevp[2];
	static long int cnt=0;

	instructions[m]++;
	locations[p]++;

	if (p==prevp[0] || p==prevp[1]) {
		cnt++;
	} else {
			if (cnt>0) {
				fprintf(stderr,"      [ %ld repeated instructions removed ]\r\n",cnt);
				cnt=0;
			}
			fprintf(stderr,"%o.%04o (%d) %05o:%04o  %s\r\n",(link?1:0),ac,irq,p,m,ops[m]);
			prevp[0]=prevp[1];
			prevp[1]=p;
			fflush(stderr);
	}
}


/************************/
/* Interface to console */
/************************/

void output_debug(void) /* show histogram */
{
	int i;
	char buf[80];

	sprintf( buf, "\r\nCumulative information since last report\r\n");
	ttyputs( buf );
	sprintf( buf, "\r\nInstructions that were fetched\r\n");
	ttyputs( buf );
	sprintf( buf, " Instruction: Frequency\r\n");
	ttyputs( buf );
	for (i=0; i<4096; i++) {
		if (instructions[i] > 0) {
			sprintf( buf, "	%04o: /%6d\r\n", i, instructions[i]);
			ttyputs( buf );
		}
	}

	sprintf( buf, "\r\nLocations that were fetched from\r\n");
	ttyputs( buf );
	sprintf( buf, "     Address: Frequency\r\n");
	ttyputs( buf );
	for (i=0; i<32768; i++) {
		if (locations[i] > 0) {
			sprintf( buf, "	%04o: /%6d\r\n", i, locations[i]);
			ttyputs( buf );
		}
	}
	reset_debug();
}
