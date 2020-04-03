/* File: rx8e.c
   Author: Douglas Jones, Dept. of Comp. Sci., U. of Iowa, Iowa City, IA 52242.
   Date: Jul 26, 1995
   Language: C (UNIX)
   Purpose: DEC RX8 RX01 diskette interface

   Based on the description in the RX8/RX11 Floppy Disk System Maintenance Man.
   Digital Equipment Corporation, 1976.

   A simulated RX01 disk is a file containing the string "rx01" as the
   first 4 characters of the first 258 byte block.  The actual data
   sectors of the file occupy consecutive 128 byte blocks starting with
   byte 256.  Simulated RX01 diskettes should be created using rx01format.
*/

#include "realtime.h"
#include "bus.h"
#include <stdio.h>


/***********/
/* formats */
/***********/

/* RX01 physical diskette layout */
#define bytes_per_sector 128
#define min_sector 1
#define max_sector 26
#define sectors_per_track ((max_sector-min_sector)+1) /* 26 */
#define min_track 0
#define max_track 76
#define tracks_per_disk ((max_track-min_track)+1) /* 77 */
#define sectors_per_disk (tracks_per_disk * sectors_per_track) /* 2002 */
/* note that this implies that there are 256256 bytes per disk */

/* layout of file used to emulate diskette */
/* bytes 0 through 3 are "rx01" */
#define tag_delete 4
/* tag bytes 4 and up hold the bit-vector that marks deleted data sectors */
#define length_delete ((sectors_per_disk+7)/8) /* 251 */
#define tag_data (tag_delete+length_delete) /* 255 */
#define tag_sectors ((tag_data+bytes_per_sector-1)/bytes_per_sector) /* 2 */
/* round up to a multiple of the sector size for start of real data sectors */
#define tag_length (tag_sectors*bytes_per_sector) /* 256 */

/**********/
/* delays */
/**********/

/* RX8E interface to RX01 controller delay, per data chunk */
#define byte_time ((18 * microsecond) / IOFUDGE )
#define word_time ((23 * microsecond) / IOFUDGE )

/* RX01 seek times */
#define track_time ((10 * millisecond) / IOFUDGE )
#define settle_time ((25 * millisecond) / IOFUDGE )
#define rotate_time ((166 * millisecond) / IOFUDGE )
#define sector_time (rotate_time / sectors_per_track)


/*********************************************************/
/* Interface between device implementation and "console" */
/*********************************************************/

/* files used to simulate the device */
static FILE *disk[2];
static char diskname[2][NAME_LENGTH];
static char disktag[2][tag_length];
static int current_track[2];
static int current_sector[2];

/* timers used to simulate delays between I/O initiation and completion */
static struct timer rx8e_delay;
static struct timer rx01_delay;

static diskclose(u)
int u;
{
	if (disk[u] != NULL) {
		fseek( disk[u], 0L, 0 );
		fwrite( disktag[u], tag_length, 1, disk[u] );
		fclose( disk[u] );
		disk[u] = NULL;
		diskname[u][0] = '\0';
	}
}

static int diskopen(u, f)
int u;
char * f;
{
	diskclose(u);
	set_file_name( diskname[u], f );
	if ((disk[u] = fopen( diskname[u], "r+" )) == NULL) { /* can't open */
		diskname[u][0] = '\0';
	} else { /* the file exists, is it an RX01 disk? */
		if (fread( disktag[u], tag_length, 1, disk[u] ) == 1) {
			/* tag readable */
			if ((disktag[u][0] != 'r') || (disktag[u][1] != 'x')
			 || (disktag[u][2] != '0') || (disktag[u][3] != '1')) {
				/* bad header */
				diskclose(u);
			}
		} else { /* couldn't read tag */
			diskclose(u);
		};
	}
	current_track[u] = min_track;
	current_sector[u] = min_sector; /* should be randomized */
	return (disk[u] != NULL);
}

rx8epower() /* power-on initialize */
{
	disk[0] = NULL;
	disk[1] = NULL;
	diskname[0][0] = '\0';
	diskname[1][0] = '\0';
	init_timer( rx8e_delay );
	init_timer( rx01_delay );
	register_device( diskopen, diskclose, 1,
			 "RX1", "-- RX01 floppy disk drive 1     ",
			 diskname[1] );
	register_device( diskopen, diskclose, 0,
			 "RX0", "-- RX01 floppy disk drive 0     ",
			 diskname[0] );
}


/*************************************/
/* "officially visible" device state */
/*************************************/

static int command_register;
#define function  ((command_register >> 1) & 7)
#define unit      ((command_register >> 4) & 1)
#define eight_bit ((command_register >> 6) & 1)
#define maint     ((command_register >> 7) & 1)

static int interface_register;

static int error_code;
static int RXTA; /* track address */
static int RXSA; /* sector address */
static int RXES; /* error and status */

static unsigned char buffer[128]; /* one sector stored in RX01 controller */

static int transfer_request;
static int error_flag;
static int done_flag;
static int interrupt_enable;

/*************************/
/* Device implementation */
/*************************/

/* conditions to which the RX01 disk drive reacts */
#define rx01_idle 0
#define rx01_read 1
#define rx01_write 2
#define rx01_write_del 3
#define rx01_read_stat 4
/* function awaiting RX01 action */
static int rx01_function;

static void rx01_done()
/* called from within rx01_event() to indicate finishing of a rx01 operation */
{
	done_flag = 1;
	rx01_function = rx01_idle;
	if (interrupt_enable == 1) {
		irq = irq + 1;
	}
	interface_register = RXES;
}

static void rx01_event(p)
int p;
{ /* called from timer as each sector spins by disk heads */
	int u;

	switch (rx01_function) {
	case rx01_idle:
		break;
	case rx01_read:
		if (disk[unit] == NULL) { /* load head and find no media */
			error_code = 0110;
			error_flag = 1;
			rx01_done();
		} else if (current_sector[unit] == RXSA) {
			long int sectnum =
				  ((RXTA - min_track) * sectors_per_track)
				+ (RXSA - min_sector);
			fseek( disk[unit],
			       (sectnum * bytes_per_sector) + tag_length,
			       0 );
			RXES &= 07676; /* turn off DD and CRC bits */
			if(fread(buffer, bytes_per_sector, 1, disk[unit])!=1) {
				error_flag = 1;
				RXES |= 00001; /* report CRC error */
			} else {
				/* report deleted data bit */
				int byte =disktag[unit][tag_delete+(sectnum/8)];
				int bit = (byte >> (sectnum % 8)) & 1;
				RXES |= (bit << 6); /* report DD bit */
			}
			rx01_done();
		}
		break;
	case rx01_write:
	case rx01_write_del:
		if (disk[unit] == NULL) { /* load head and find no media */
			error_code = 0110;
			error_flag = 1;
			rx01_done();
		} else if (current_sector[unit] == RXSA) {
			long int sectnum =
				  ((RXTA - min_track) * sectors_per_track)
				+ (RXSA - min_sector);
			fseek( disk[unit],
			       (sectnum * bytes_per_sector) + tag_length,
			       0 );
			RXES &= 07676; /* turn off DD and CRC bits */
			if(fwrite(buffer, bytes_per_sector, 1, disk[unit])!=1){
				error_flag = 1;
				error_code = 0140; /* call it a bad header */
			} else {
				/* set bit in deleted data array */
				int bit = (rx01_function == rx01_write_del);
				disktag[unit][tag_delete+(sectnum/8)]
					&= ~(1 << (sectnum % 8));
				disktag[unit][tag_delete+(sectnum/8)]
					|= (bit << (sectnum % 8));
				RXES |= (bit << 6); /* report DD bit */
			}
			rx01_done();
		}
		break;
	case rx01_read_stat:
		if (disk[unit] == NULL) { /* load head and find no media */
			error_code = 0110;
			error_flag = 1;
			rx01_done();
		} else if (current_sector[unit] == min_sector) {
			RXES |= 0200; /* turn on Drv Rdy bit */
			RXES &= 0303; /* turn off Init Done bit */
			rx01_done();
		}
		break;
	}

	for (u = 0; u <= 1; u++) {
		if (disk[0] != NULL) { /* a disk is mounted */
			/* make it spin */
			current_sector[u] += 1;
			/* should allow other interleave factors */
			if (current_sector[u] > max_sector) {
				current_sector[u] -= sectors_per_track;
			}
		}
	}

	/* schedule next sector to spin by head */
	schedule( &rx01_delay, sector_time, rx01_event, 0 );
}

/* events to which the RX8E controller responds */
#define rx8_init      1 /* initialize */
#define rx8_init2     2 /* initialize */
#define rx8_init3     3 /* initialize */
#define rx8_init4     4 /* initialize */
#define rx8_init5     5 /* initialize */
#define rx8_init6     6 /* initialize */
#define rx8_xfer      7 /* ready for XDR */
#define rx8_fill8     8 /* 8 bit fill buffer from interface register */
#define rx8_fill12    9 /* 12 bit fill buffer from interface register */
#define rx8_empty8   10 /* 8 bit fill buffer into interface register */
#define rx8_empty12  11 /* 12 bit empty buffer into interface register */
#define rx8_seek     12 /* XDR involved with seek */
#define rx8_seekdone 13 /* second step in seek */
#define rx8_seekfail 14 /* third step in seek */
#define rx8_read_err 15 /* alternate third step in seek */
#define rx8_nop      16 /* no op is more complex than manual hints */

/* internal state of RX8E controller */
static int partial; /* partial completion state for multiple byte transfers */
static int bufpos;  /* position in buffer */

static void rx8_done()
/* called from withing rx8e_event() to indicate end of an rx8 operation */
{
	done_flag = 1;
	partial = 0;
	bufpos = 0;
	if (interrupt_enable == 1) {
		irq = irq + 1;
	}
	interface_register = RXES;
}

static void rx8e_event(p)
int p;
{ /* called from timer when an interface delay has completed */
	switch (p) {
	case rx8_init: /* initialize step 1, start seek + 10 on drive 1 */
		bufpos = 0;
		partial = 0;
		schedule( &rx8e_delay, track_time * 10,
			  rx8e_event, rx8_init2 );
		break;
	case rx8_init2: /* initialize step 2, start seek home drive 1 */
		current_track[1] += 10;
		if (current_track[1] > max_track) {
			current_track[1] = max_track;
		}
		schedule( &rx8e_delay, settle_time+track_time*current_track[1],
			  rx8e_event, rx8_init3 );
		break;
	case rx8_init3: /* initialize step 3, start seek + 10 on drive 0 */
		current_track[1] = min_track;
		schedule( &rx8e_delay, track_time * 10,
			  rx8e_event, rx8_init4 );
		break;
	case rx8_init4: /* initialize step 4, start seek home drive 0  */
		current_track[0] += 10;
		if (current_track[0] > max_track) {
			current_track[0] = max_track;
		}
		schedule( &rx8e_delay, settle_time+track_time*current_track[0],
			  rx8e_event, rx8_init5 );
		break;
	case rx8_init5: /* initialize step 5, start seek track 1 drive 0 */
		current_track[0] = min_track;
		schedule( &rx8e_delay, settle_time + track_time,
			  rx8e_event, rx8_init6 );
		break;
	case rx8_init6: /* initialize final step, fire up a read */
		current_track[0] = 1;
		rx01_function = rx01_read;
		break;
	case rx8_xfer: /* get ready for XDR */
		partial = 0;
		transfer_request = 1;
		break;
	case rx8_fill8: /* Fill Buffer, 8 bit mode */
		buffer[bufpos] = interface_register & 0377;
		bufpos += 1;
		if (bufpos >= bytes_per_sector) {
			rx8_done();
		} else {
			transfer_request = 1;
		}
		break;
	case rx8_fill12: /* Fill Buffer, 12 bit mode */
		/* note, in RX8E interface card, data is shifted into
		   the sector buffer serially, most significant bit
		   first.  Thus, byte 1 holds the top 8 bits of word
		   one, byte 2 the bottom 4 of word 1, then the top
		   4 of word 2, and byte 3 holds the bottom 8 of word 2 */
		if (partial == 0) {
			buffer[bufpos] = (interface_register&07760)>> 4;
			bufpos += 1;
			buffer[bufpos] = (interface_register&00017)<< 4;
			partial = 1;
		} else {
			buffer[bufpos] |=(interface_register&07400)>> 8;
			bufpos += 1;
			buffer[bufpos] = (interface_register&00377);
			bufpos += 1;
			partial = 0;
		}
		if (bufpos >= 96) {
			for (;bufpos < bytes_per_sector; bufpos++) {
				buffer[bufpos] = buffer[96];
			}
			rx8_done();
		} else {
			transfer_request = 1;
		}
		break;
	case rx8_empty8: /* Empty Buffer, 8 bit mode */
		if (bufpos < bytes_per_sector) {
			interface_register = buffer[bufpos];
			bufpos += 1;
			transfer_request = 1;
		} else {
			rx8_done();
		}
		break;
	case rx8_empty12: /* Empty Buffer, 12 bit mode */
		/* note, in RX8E interface card, data is shifted from
		   the buffer into the least significant bit of the
		   interface register.  Thus byte zero ends up in the
		   high 8 bits of the first word, followed by the top
		   4 bits of byte 1, in the low 4 bits of the word, and
		   the second word is made of the low 4 bits of byte 1
		   in the high 4 bits, followed by all of byte 2 in the
		   low 8 bits */
		if (bufpos < 96) {
			if (partial == 0) {
				interface_register =
				  (buffer[bufpos] << 4) |
				  (buffer[bufpos + 1] >> 4);
				bufpos += 1;
				partial = 1;
			} else {
				interface_register =
				  ((buffer[bufpos] & 017) << 8) |
				  buffer[bufpos+1];
				bufpos += 2;
				partial = 0;
			}
			transfer_request = 1;
		} else {
			rx8_done();
		}
		break;
	case rx8_seek: /* XDR after read or write command */
		if (partial == 0) { /* first, transfer RXSA */
		/*	RXSA = interface_register & 0377; */
			RXSA = interface_register & 0177; /* 7 bits */
			transfer_request = 1;
			partial = 1;
		} else if (partial == 1) { /* then, transfer RXTA, start IO */
			bufpos = 0;
			partial = 0;
			RXTA = interface_register & 0377; /* 8 bits */
			if ((RXTA > max_track)||(RXTA < min_track)) {
				/* bad track address */
				error_code = 0040;
				error_flag = 1;
				rx8_done();
			} else if ((RXSA < min_sector)||(RXSA > max_sector)) {
				/* bad sector */
				schedule( &rx8e_delay, 2*rotate_time,
					  rx8e_event, rx8_seekfail );
			} else { /* sector and track both valid */
				long int time;
				int tracks;
				tracks = RXTA - current_track[unit];
				if (tracks < 0) { tracks = -tracks; }
				time = settle_time + track_time * tracks;
				if (tracks == 0) { time = 0; }
				time += sector_time;
				schedule( &rx8e_delay, time,
					  rx8e_event, rx8_seekdone );
			}
		}
		break;
	case rx8_seekfail: /* seek failed, sector number invalid */
		error_code = 0070;
		error_flag = 1;
		rx8_done();
		break;
	case rx8_seekdone: /* seek worked */
		current_track[unit] = RXTA;
		partial = 0; /* !!! */
		switch (function) {
		case 02: /* Write Sector */
			rx01_function = rx01_write;
			break;
		case 03: /* Read Sector */
			rx01_function = rx01_read;
			break;
		case 06: /* Write Deleted Data Sector */
			rx01_function = rx01_write_del;
			break;
		}
		break;
	case rx8_read_err: /* read error register */
		rx8_done();
		interface_register = error_code;
		break;
	case rx8_nop: /* no-op (a bit complicated) */
		RXES &= 0303; /* turn off Init Done bit */
		rx8_done();
		break;
	}
}


/***********************************************/
/* Initialization used by CAF and reset switch */
/***********************************************/

rx8einit() /* console reset or programmed reset */
{
	int u;
	for (u = 0; u <= 1; u++) {
		if ( (current_track[u] <= min_track)
		   ||(current_track[u] >= max_track)) {
			current_track[u] = min_track;
		}
	}

	interface_register = 0;
	error_code = 0;
	RXES = 0;

	command_register = 00000; /* disk 0 */
	RXTA = 1;                 /* track 1 */
	RXSA = 1;                 /* sector 1 */

	transfer_request = 0;
	error_flag = 0;
	done_flag = 0;
	interrupt_enable = 0;
	/* assume the cpu clears irq */

	/* set disk motors spinning (if they're already spinning, no-op */
	rx01_function = rx01_idle;
	schedule( &rx01_delay, sector_time, rx01_event, 0 );

	/* load heads and wait for them to bounce */
	schedule( &rx8e_delay, settle_time, rx8e_event, rx8_init );
}

/********************/
/* IOT Instructions */
/********************/

rx8edev(op)
int op;
{
	switch (op) {
	case 00: /* NOP */
		break;
	case 01: /* LCD */
		command_register = ac;
		ac = 0;
		if (maint) {
			transfer_request = 1;
			error_flag = 1;
			done_flag = 1;
		} else switch (function) {
		case 00: /* Fill Buffer */
			schedule( &rx8e_delay, byte_time,
				  rx8e_event, rx8_xfer );
			break;
		case 01: /* Empty Buffer */
			if (eight_bit) {
				schedule( &rx8e_delay, byte_time,
					  rx8e_event, rx8_empty8 );
			} else {
				schedule( &rx8e_delay, word_time,
					  rx8e_event, rx8_empty12 );
			}
			break;
		case 02: /* Write Sector */
		case 03: /* Read Sector */
		case 06: /* Write Deleted Data Sector */
			schedule( &rx8e_delay, byte_time,
				  rx8e_event, rx8_xfer );
			break;
		case 04: /* No op */
			schedule( &rx8e_delay, byte_time,
				  rx8e_event, rx8_nop );
			break;
		case 05: /* Read Status */
			rx01_function = rx01_read_stat;
			break;
		case 07: /* Read Error Register */
			schedule( &rx8e_delay, byte_time,
				  rx8e_event, rx8_read_err );
			break;
		}
		break;
	case 02: /* XDR */
		if (maint) {
			ac |= interface_register;
		} else switch (function) {
		case 00: /* Fill Buffer */
			interface_register = ac;
			if (eight_bit) {
				schedule( &rx8e_delay, byte_time,
					  rx8e_event, rx8_fill8 );
			} else {
				schedule( &rx8e_delay, word_time,
					  rx8e_event, rx8_fill12 );
			}
			break;
		case 01: /* Empty Buffer */
			if (eight_bit) {
				ac |= interface_register;
				schedule( &rx8e_delay, byte_time,
					  rx8e_event, rx8_empty8 );
			} else {
				ac = interface_register;
				schedule( &rx8e_delay, word_time,
					  rx8e_event, rx8_empty12 );
			}
			break;
		case 02: /* Write Sector */
		case 03: /* Read Sector */
		case 06: /* Write Deleted Data Sector */
			interface_register = ac;
			schedule( &rx8e_delay, byte_time, rx8e_event,
				  rx8_seek );
			break;
		case 04: /* No op */
		case 05: /* Read Status */
			ac |= interface_register;
			break;
		case 07: /* Read Error Register */
			ac |= interface_register;
			break;
		}
		break;
	case 03: /* STR */
		if (transfer_request == 1) {
			pc = (pc + 1) & 07777;
		}
		if (!maint) {
			transfer_request = 0;
		}
		break;
	case 04: /* SER */
		if (error_flag == 1) {
			pc = (pc + 1) & 07777;
		}
		if (!maint) {
			error_flag = 0;
		}
		break;
	case 05: /* SDN */
		if (done_flag == 1) {
			pc = (pc + 1) & 07777;
		}
		if (!maint) {
			if (interrupt_enable == 1) {
				irq = irq - done_flag;
			}
			done_flag = 0;
		}
		break;
	case 06: /* INTR */
		if (interrupt_enable == 1) {
			irq = irq - done_flag;
		}
		interrupt_enable = ac & 1;
		if (interrupt_enable == 1) {
			irq = irq + done_flag;
		}
		break;
	case 07: /* INIT */
		rx8einit();
		break;
	}
}
