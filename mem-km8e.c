/* File: km8e.c
   Author: Douglas Jones, Dept. of Comp. Sci., U. of Iowa, Iowa City, IA 52242.
   Date: Jul 26, 1995
   Language: C (UNIX)
   Purpose: DEC KM8/E memory management functions, excepting those
            included in the KK8E cpu.  This version does not support
	    the time-share option.

   Based on the description in the PDP-8/E Small Computer Handbook,
   Digital Equipment Corporation, 1971.
*/

#include "bus.h"

/******************/
/* Initialization */
/******************/

void km8epower(void) /* global initialize */
{
	ifr = 0;
        ib = 0;
        dfr = 0;
	sf = 0;
	uf = 0;
	ub = 0;
}


void km8einit(void) /* console reset */
{
	km8e_uif = 0;
	/* reset to user mode eventually */
}

/********************/
/* IOT Instructions */
/********************/

void km8edev(int op)
{
	switch (op & 07) {
	case 00:
		break;
	case 01: /* CDF */
		dfr = (op & 0070) << 9;
		break;
	case 02: /* CIF */
		ib = (op & 0070) << 9;
		enab_rtf = 0; /* disable interrupt till branch */
		break;
	case 03: /* CDF CIF */
		ib = (op & 0070) << 9;
		dfr = ib;
		enab_rtf = 0; /* disable interrupt till branch */
		break;
	case 04: /* other */
		switch (op >> 3) {
		case 00: /* CINT */
			/* clear user interrupt flag */
			irq = irq - km8e_uif;
			km8e_uif = 0;
			break;
		case 01: /* RDF */
			ac = ac | (dfr >> 9);
			break;
		case 02: /* RIF */
			ac = ac | (ifr >> 9);
			break;
		case 03: /* RIB */
			ac = ac | sf;
			break;
		case 04: /* RMF */
			/* user state bit ? */
			ib = (sf & 070) << 9;
			dfr = (sf & 007) << 12;
			ub = (sf & 0100) >> 6;
			enab_rtf = 0; /* disable interrupt till branch */
			break;
		case 05: /* SINT */
			/* skip if user interrupt flag */
			if (km8e_uif == 1) {
				pc = (pc + 1) & 07777;
			}
			break;
		case 06: /* CUF */
			/* clear user flag */
			ub = 0;
			break;
		case 07: /* SUF */
			/* set user flag, */
			ub = 1;
			enab_rtf = 0; /* disable interrupt till branch */
			break;
		}
		break;
	case 05:
	case 06:
	case 07:
		/* no op? */
		break;
	}
}
