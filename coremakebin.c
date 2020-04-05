/* File: coremakebin.c
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

void bin(void)
/* bin loader */
{
	int md, c;
	int ma = 0;
	int fld=0;
	int restore=0;
	int stop_at_header=0;

	for (;;) {
		if ((c = getchar()) == EOF) return;
		if ((c&0300) == 0300) {
			// printf("Field\n");
			fld=c&07;
			continue;
		}
		if ((c&0300) == 0200) {
			// printf("Header\n");
			if (stop_at_header==1) break;
			continue;
		}
		if ((c&0300) == 0100) {
			ma=c&077;
			if ((c = getchar()) == EOF) return;
			ma=(ma<<6)+c;
			// printf("Address=%04o \n", ma);
			continue;
		}
		if ((c&0300) == 0000) {
			md=c&077;
			if ((c = getchar()) == EOF) return;
			md=(md<<6)+c;
			if (fld==0) {
				// printf("Memory[%04o]=%04o \n", ma,md);
				restore=memory[ma];
				memory[ma]=md;
			}
			ma++;
			stop_at_header=1;
			continue;
		}
	}
	memory[ma]=restore;
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
	bin();
	dumpcore();
}
