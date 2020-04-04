/* File: cr8f.c
   Author: Douglas Jones, Dept. of Comp. Sci., U. of Iowa, Iowa City, IA 52242.
   CoAuthor: Satish Viswanatham
   Date: Mar. 5, 1996
   Language: C (UNIX)
   Purpose: DEC CR8-F Card Reader and Control

   Based on the description in the Small Computer Handbook,
   Digital Equipment Corporation, 1973, plus inspection of the
   schematics in the PDP-8/E/F/M maintenance manual.

*/

#include <stdio.h>
#include "realtime.h"
#include "bus.h"
#include "utility.h"
#include "kc8m.h"


#define read_time ( 1 * millisecond)
#define pick_lead_time ( 46 * millisecond)
#define trail_time ( 23 * millisecond)


/*************************************/
/* "officially visible" device state */
/*************************************/

static int read_buffer;
static int data_ready_flag;
static int card_done_flag;
static int ready_true_trans_flag;
static int trouble_trans_flag;
static int true_trouble_enable;
static int ready_done_enable;

/**********************/
/* other device state */
/**********************/

static int cur_column;  /* current card column */
static int byte1, byte2, byte3;   /* three consecutive bytes of input */


/*********************************************************/
/* Interface between device implementation and "console" */
/*********************************************************/

/* timers used to simulate delays between I/O initiation and completion */
static struct timer read_delay;

static FILE *reader_stream = NULL;
static char cardname[NAME_LENGTH];

static void cardclose(int u)
{
        if (reader_stream != NULL){
                fclose(reader_stream);
                reader_stream = NULL;
                cardname[0] = '\0';

		/* putting reader off-line requests interrupt if enabled */
		if (true_trouble_enable) {
			irq = irq - ready_true_trans_flag + 1;
		}

		/* putting reader off-line sets flags */
		ready_true_trans_flag = 0;
		trouble_trans_flag = 1;
        }
}

static int cardopen(int u, char *f)
{
        cardclose(u);
        set_file_name(cardname, f);
        if (( reader_stream = fopen(cardname, "r")) == NULL){ /* fail */
                cardname[0] = '\0';
	} else { /* success */
		/* check for prefix on input file */
		if ((fgetc( reader_stream ) != 'H')
		||  (fgetc( reader_stream ) != '8')
		||  (fgetc( reader_stream ) != '0')) {
			fclose(reader_stream);
			reader_stream = NULL;
			cardname[0] = '\0';
		} else {
			/* putting reader on-line may request interrupt */
			if (true_trouble_enable) {
				irq = irq - trouble_trans_flag + 1;
			}

			/* putting reader on-line sets flags */
			ready_true_trans_flag = 1;
			trouble_trans_flag = 0;
		}
        }
        return (reader_stream != NULL);
}


/*************************/
/* Device Implementation */
/*************************/

static void end_of_card(int p)
{
	if (ready_done_enable) {
		irq = irq - card_done_flag + 1;
	}
	card_done_flag = 1;
	if (feof(reader_stream)) {
		cardclose(0);
	}
}

static void read_odd_column(int);    /* forward declaration */

static void read_even_column(int p)
{
	byte1 = fgetc(reader_stream);
	byte2 = fgetc(reader_stream);
	byte3 = fgetc(reader_stream);
	read_buffer = (byte1 << 4) | (byte2 >> 4);
	cur_column++;

	if (ready_done_enable) {
		irq = irq - data_ready_flag + 1;
	}
	data_ready_flag = 1;

	/* note that even columns never end cards! */
	schedule( &read_delay, read_time, read_odd_column, 0);
}

static void read_odd_column(int p)
{
	read_buffer = ((byte2 & 0017) << 8) | byte3;
	cur_column++;

	if (ready_done_enable) {
		irq = irq - data_ready_flag + 1;
	}
	data_ready_flag = 1;

	if ( cur_column >= 80){ /* end of card */
		schedule( &read_delay, trail_time, end_of_card, 0); 
	} else {
		schedule( &read_delay, read_time, read_even_column, 0); 
	}
}


/***********************/
/* Data Conversion     */
/***********************/

/* map 12 bit card codes to 8 bit "compressed codes" */
char compressed_code[4096];

/* map 12 bit card codes to 6 bit "holerith codes" (truncated ASCII) */
char hollerith_code[4096];

void init_conversions(void)
/* initialize conversion tables */
{
	int i;

        /* go through all possible 12 bit codes */
	/* card column numbering:   12 11  0  1  2  3  4  5  6  7  8  9 */
        /* bit numbering:            0  1  2  3  4  5  6  7  8  9 10 11 */
	for (i = 0; i < 4096; i++) {

		/* this material used for error detection in compressed code */
		int bitcount;
		bitcount = (i & 00774) >> 2; /* extract columns 1-7 */
		/* the following 3 lines count the 1 bits in columns 1-7 */
		bitcount = (bitcount & 0125) + ((bitcount & 0052) >> 1);
		bitcount = (bitcount & 0063) + ((bitcount & 0114) >> 2);
		bitcount = (bitcount & 0017) + ((bitcount & 0160) >> 4);

		compressed_code[ i ] =
		/* rearrange bits */
		   ((i & 00001) << 7) /* row 9 goes in bit 4 */
		 | ((i & 07000) >> 5) /* rows 12-11-10 go in bits 5 to 7 */
		 | ((i & 00002) << 2) /* row 8 goes in bit 8 */
		/* encode remaining numeric punches */
		 | ((i & 00400) >> 8) /* row 1 sets bit 11        */
		 | ((i & 00200) >> 6) /* row 2 sets bit 10        */
		 | ((i & 00100) >> 5) /* row 3 sets both 10       */
		 | ((i & 00100) >> 6) /*                    and 11*/
		 | ((i & 00040) >> 3) /* row 4 sets bit 9         */
		 | ((i & 00020) >> 2) /* row 5 sets both 9        */
		 | ((i & 00020) >> 4) /*                   and 11 */
		 | ((i & 00010) >> 1) /* row 6 sets both 9        */
		 | ((i & 00010) >> 2) /*                   and 10 */
		 | ((i & 00004)     ) /* row 7 sets 9             */
		 | ((i & 00004) >> 1) /*              and 10      */
		 | ((i & 00004) >> 2) /*                    and 11*/
		/* detect errors in numeric punches */
		 | ((bitcount > 1) << 11);

		hollerith_code[ i ] =
		/* encode zone */
		   ((i & 03000) >> 5) /* rows 11,0 go in bits 6,7 */
		 | ((i & 04000) >> 6) /* row 12 sets both 6       */
		 | ((i & 04000) >> 7) /*                    and 7 */
		/* encode numeric punches */
		 | ((i & 00400) >> 8) /* row 1 sets bit 11        */
		 | ((i & 00200) >> 6) /* row 2 sets bit 10        */
		 | ((i & 00100) >> 5) /* row 3 sets both 10       */
		 | ((i & 00100) >> 6) /*                    and 11*/
		 | ((i & 00040) >> 3) /* row 4 sets bit 9         */
		 | ((i & 00020) >> 2) /* row 5 sets both 9        */
		 | ((i & 00020) >> 4) /*                   and 11 */
		 | ((i & 00010) >> 1) /* row 6 sets both 9        */
		 | ((i & 00010) >> 2) /*                   and 10 */
		 | ((i & 00004)     ) /* row 7 sets 9             */
		 | ((i & 00004) >> 1) /*              and 10      */
		 | ((i & 00004) >> 2) /*                    and 11*/
		 | ((i & 00002) << 2) /* row 8 sets bit 8         */
		 | ((i & 00001) << 3) /* row 9 sets bit 8         */
		 | ((i & 00001)     );/*                  and 11  */
	}
}


/******************/
/* Initialization */
/******************/

void cr8fpower(void) /* global initialize */
{
        /* set up timers used to delay I/O activity */
        init_timer(read_delay);
	init_conversions();

	register_device( cardopen, cardclose, 0,
			 "CDR", "-- Card reader             ",
			 cardname);
}

void cr8finit(void) /* console reset */
{
        true_trouble_enable = 0;
	ready_done_enable = 1;
	ready_true_trans_flag = 0;
	trouble_trans_flag = 0;	
	data_ready_flag = 0;
	card_done_flag = 0;
}


/********************/
/* IOT Instructions */
/********************/

void cr8fdev3(int op)
{
        switch (op) {
        case 01: /* RCSF */
                if (data_ready_flag == 1) {
			pc = (pc + 1) & 07777;
                }
                break;
        case 02: /* RCRA */
		ac = hollerith_code[read_buffer];
		if (ready_done_enable == 1) {
			irq = irq - data_ready_flag;
		}
		data_ready_flag = 0;
                break;
        case 03: /* no operation! */
                break;
        case 04: /* RCRB */
		ac = read_buffer;
		if (ready_done_enable == 1) {
			irq = irq - data_ready_flag;
		}
		data_ready_flag = 0;
                break;
        case 05: /* RCNO */
		if (((ac & 00002) != 0)
		&& (true_trouble_enable == 0)) {
			true_trouble_enable = 1;
			irq = irq + ready_true_trans_flag + trouble_trans_flag;
		}
		if (((ac & 00001) != 0 )
		&& (ready_done_enable == 0)) {
			ready_done_enable = 1;
			irq = irq + card_done_flag + data_ready_flag;
		}
                break;
        case 06: /* RCRC */
		ac = compressed_code[read_buffer];	
		if (ready_done_enable == 1) {
			irq = irq - data_ready_flag;
		}
		data_ready_flag = 0;
                break;
        case 07: /* RCNI */
		ac =
		   (ready_true_trans_flag << 8)
		 | (trouble_trans_flag << 9)
		 | (card_done_flag << 10)
		 | (data_ready_flag << 11);
		break;
        }
}

void cr8fdev7(int op)
{
        switch (op) {
        case 01: /* RCSD */
		if (card_done_flag == 1) {
			pc = (pc +1) & 07777;
		}
                break;
        case 02: /* RCSE */
		if (reader_stream != NULL) { /* reader on-line */
			/* get header of next card */
			byte1 = fgetc(reader_stream);
			byte2 = fgetc(reader_stream);
			byte3 = fgetc(reader_stream);

                        if ((byte1==EOF) /* normal EOF */
                        ||  (byte2==EOF) /* truncated file? */
                        ||  (byte3==EOF) /* truncated file? */
                        ||  ((byte1 & 0x80)==0) /* corrupt file */
                        ||  ((byte2 & 0x80)==0) /* corrupt file */
                        ||  ((byte3 & 0x80)==0) /* corrupt file */
			) { /* declare the reader off line */
				cardclose(0);
			} else { /* a card is available */
				pc = (pc +1) & 07777;
				if (ready_done_enable == 1) {
					irq = irq - card_done_flag;
				}
				card_done_flag = 0;
				cur_column = 0;
				schedule( &read_delay, pick_lead_time, read_even_column, 0); 
			}
		}
                break;
        case 03: /* no operation! */
                break;
	case 04: /* RCRD */
		if (ready_done_enable == 1) {
			irq = irq - card_done_flag;
		}
		card_done_flag = 0;
		break;
	case 05: /* RCSI */
		if ( ( true_trouble_enable
		   &&  (ready_true_trans_flag || trouble_trans_flag)
		     )
                ||   ( ready_done_enable
                   &&  (data_ready_flag || card_done_flag)
                )    ) {
			pc = (pc + 1) & 07777;
		}
		break;
        case 06: /* no operation! */
                break;
	case 07: /* RCTF*/
		if (true_trouble_enable == 1) {
			irq = irq - ready_true_trans_flag +-trouble_trans_flag;
		}
		ready_true_trans_flag = 0;
		trouble_trans_flag = 0;
		break;
        }
}
