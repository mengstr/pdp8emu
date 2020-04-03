/* File: coredump.c
   Author: Douglas Jones, Dept. of Comp. Sci., U. of Iowa, Iowa City, IA 52242.
   Date: July 26, 1995
   Language: C (UNIX)
   Purpose: DEC PDP-8/e emulator core file inspection program.
   Operation: read a core image file as specified on the command line and
	output a core image to standard output.
   Requires: utility.c -- the utility routines for the emulator.
*/

/* First, declare that this is a main program */
#define MAIN

#include <stdio.h>
#include "bus.h"


dump()
/* dump all of memory */
{
	int ma = 0;
	do {
		int i;
		printf( "%5.5o:", ma );
		for (i = 0; i < 8; i++) {
			printf( " %4.4o", memory[ma + i] );
		}
		printf( "\n" );

		/* now, try to find a memory address with a difference */
		do {
			ma = ma + 8;
			if (ma >= MAXMEM) { break; }
		} while (
			(memory[ma] == memory[ma-8]) &&
			(memory[ma+1] == memory[ma-7]) &&
			(memory[ma+2] == memory[ma-6]) &&
			(memory[ma+3] == memory[ma-5]) &&
			(memory[ma+4] == memory[ma-4]) &&
			(memory[ma+5] == memory[ma-3]) &&
			(memory[ma+6] == memory[ma-2]) &&
			(memory[ma+7] == memory[ma-1])
		);
	} while (ma < MAXMEM);
}

/* main program */
main(argc, argv)
int argc;
char *argv[];
{
	getargs(argc, argv);
        if (corename == NULL) { /* no file specified */
		fprintf( stderr, "%s: no core file specified\n", argv[0] );
		exit(-1);
	}
	readcore();
	dump();
}
