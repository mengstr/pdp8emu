/* File: utility.c
   Author: Douglas Jones, Dept. of Comp. Sci., U. of Iowa, Iowa City, IA 52242.
   Date: July 31, 1995
   Language: C (UNIX)
   Purpose: DEC PDP-8/e emulator utility routines
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "bus.h"


/* The following routine is used to make a copy of a string into a buffer *
 * that will be used as a file name.  This routine checks to make sure    *
 * that the file name fits in the buffer, and it prevents file names with *
 * embedded blanks by stopping the copy at the first blank.  It is safe   *
 * to call this with the same parameter for both arguments.               */

void set_file_name(
	char * f, /* buffer for file name */
	char * s /* string from which name comes */
)
{
        int i = 0; /* index into f */
        int j = 0; /* index into s */
        while ((i < (NAME_LENGTH - 1)) && (isgraph(s[j]))) {
                f[i++] = s[j++];
        }
        f[i] = '\0';
}


/* command line argument processing */

void getargs(int argc, char *argv[])
{
        int i;

	/* initialize argument flags etc to defaults */
	corename[0] = '\0';
	progname = argv[0];

	/* find real values of argument flags etc */
        for (i=1; i < argc; i++) {
		if (argv[i][0] == '-') {
			fprintf( stderr, "%s: unexpected option %s\n",
				 progname, argv[i] );
                        exit(-1);
                } else if (corename[0] != '\0') {
                        fprintf( stderr, "%s: too many core files specified\n",
                                 progname );
                        exit(-1);
                } else {
			set_file_name( corename, &argv[i][0] );
		}
        } /* end for loop */
}


/* The following two routines read and write core image files using a     *
 * simple variant of Charles Lasner's .IPL file format.                   *
 *                                                                        *
 * A core image file begins with an "auto-boot" header so that the host   *
 * operating system can run the file, loading the PDP-8 interpreter to    *
 * interpret its contents.  Under UNIX, this header is a line reading     *
 * "#!<i>", where "<i>" is replaced by the absolute file name of the      *
 * PDP-8 emulator.                                                        *
 *                                                                        *
 * In the remainder of the file, spaces and control characters are        *
 * ignored.  Immediately following the header, there may be any number    *
 * of lower-case ASCII characters, used as a comment.  These are ignored  *
 * in Lasner's standard, but here, if the comment begins with the string  *
 * "run", the machine is powered up in the running state.                 *
 * Following these are upper case characters (any ASCII character after   *
 * space and before the lower case characters), each representing one     *
 * 6 bit byte of the core image.  The two bytes of each 12 bit word are   *
 * recorded most significant bit first, and words are recorded starting   *
 * with word zero of memory.  The load data ends with the end of file,    *
 * a lower case letter, or the end of the emulator's memory, which ever   *
 * is encountered first.                                                  */

void readcore(void)
{
	FILE *f;
	int  a; /* memory address to load */
	int  c,b; /* recent characters */
	int  d; /* counter used to pair up load bytes b and c */

	if ((f = fopen(corename, "r")) == NULL) {
		fprintf( stderr, "%s: cannot read core image from %s\n",
                         progname, corename );
		corename[0] = '\0';
	} else {
		if ((c = getc(f)) == '#') { /* skip emulator prefix */;
			do { /* skip until eol or eof */
				c = getc(f);
			} while ((c != EOF) && (c != '\n'));
		}

		c = getc(f);
		while ((c != EOF) && (( c <= ' ') || (c >= 'a'))) {
			/* skip lower case ASCII leader */
			c = getc(f);
		}

		/* start of load data */
		d = 1;
		a = 0;
		for (;;) {
			b = c;
			do { /* get the next data character */
				c = getc(f);
			} while ((c != EOF) && (c <= ' '));
			if (c == EOF) break;
			if (c >= 'a') break;
			d++;
			if (d == 2) { /* we have a digraph */
				memory[a] = ((b - '!')<<6) | (c - '!');
				a++;
				d = 0;
			}
			if (a >= MAXMEM) break;
		}
		/* comes here on (c>=a) or (c==EOF) or (a>=MAXMEM) */
		while ((c != EOF) && (c < 'a')) { /* try to find epilogue */
			c = getc(f);
		}
		if (c != EOF) { /* try to read epilogue */
			char n[5]; /* a device name */
			char fn[NAME_LENGTH]; /* a file name */
			int i;

			/* the following mirrors dump_devices */

			while (c == 'm') { /* try to mount a file */
				c = getc(f);
				if (c != ' ') break;
				i = 0; /* setup to read device name */
				c = getc(f);
				while (isgraph(c)) { /* get device name */
					n[i++] = c;
					c = getc(f);
					if (i >= 4) break;
				}
				n[i] = '\0';
				if (c != ' ') break;
				i = 0; /* setup to read name of file*/
				c = getc(f);
				while (isgraph(c)) { /* get file name */
					fn[i++] = c;
					c = getc(f);
					if (i >= (NAME_LENGTH-1)) break;
				}
				fn[i] = '\0';
				if (c != '\n') break;
				c = getc(f);
				mount_device( n, fn );
			}
			if (c == 'r') { /* emulator running when depowered */
				run = 1;
			}
		}
		fclose(f);
	}
}

void dumpcore(void)
{
	FILE *f;
	int a, max; /* memory address */
	int d; /* count of characters on this line */

	if ((f = fopen(corename, "w")) == NULL) {
		fprintf( stderr, "%s: cannot write core image to %s\n",
                         progname, corename );
		corename[0] = '\0';
	} else {
		/* first, output emulator prefix */
		fputs( "#!", f );
		fputs( PDP8NAME, f );
		putc( '\n', f );

		d = 0;
		max = MAXMEM-1;
		while (memory[max] == 0) { /* avoid output of unused fields */
			max--;
		}
		for (a = 0; a < max; a++) { /* dump nonzero memory */
			putc( (memory[a] >> 6) + '!', f );
			putc( (memory[a] & 077) + '!', f );
			d++;
			if (d >= 32) {
				putc( '\n', f );
				d = 0;
			}
                }
		if (d != 0) {
			putc( '\n', f );
		}
                dump_devices( f );
                if (run == 1) {
                        fputs( "r\n", f );
		}
		fputs( "end\n", f );
		fclose(f);
	}
}
