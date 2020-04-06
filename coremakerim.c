/* File: coremakerim.c
   Updates: Mats Engstrom, April 2020
   Author: Douglas Jones, Dept. of Comp. Sci., U. of Iowa, Iowa City, IA 52242.
   Date: July 28, 1995
   Language: C (UNIX)
   Purpose: DEC PDP-8/e emulator core file creation program.
   Operation: read a .rim file from standard input and output a core image
	to the file named on the command line.
   Requires: utility.c -- the utility routines for the emulator.
*/

#include <stdio.h>
#include <stdlib.h>
#include "bus.h"
#include "utility.h"

char * progname; /* name of program itself (argv[0]) */
char corename[NAME_LENGTH]; /* name of core image file, if any */
int memory[MAXMEM];
int run;

void rim(void)
/* rim loader (logic reproduced as closely as possible from DEC documents) */
{
	int c, cc;
	int ma = 0;
	for (;;) {
		do {
			if ((c = getchar()) == EOF) {
				return;
			}
		} while (c > 0177);
		c = (c << 6);
		if ((cc = getchar()) == EOF) {
			return;
		}
		c = c | cc;
		if (c <= 07777) {
			memory[ma] = c;
		}
		ma = c & 07777;
	}
}

void dump_devices(FILE *f)
{
}

void mount_device(char * n, char * f)
{
}


/* main program */
int main(int argc, char *argv[])
{
	getargs(argc, argv);
        if (corename[0] == '\0') { /* no file specified */
		fprintf( stderr, "%s: no core file specified\n", argv[0] );
		exit(-1);
	}
	rim();
	dumpcore();
}
