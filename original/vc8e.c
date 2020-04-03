/* File: vc8e.c
   Date:
   Language: C (UNIX)
   Purpose: DEC 

   Based on the description in the PDP-8/E Small Computer Handbook,
   Digital Equipment Corporation, 1971.

   WARNING -- this description is incomplete, and therefore, the
   implementation given here is likely to be wrong!  It would be
   helpful if I could find the M885/M869 schematics or the VC8E
   maintenance manual.
*/


#include <stdio.h>
#include <strings.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Core.h>
#include <X11/Shell.h>

#include "realtime.h"
#include "bus.h"


/******************************************************************/
/* options:  The user may select any of a the following displays: */
/*       VR14, VR20, TEK602, TEK611                               */
/* If the VR14 display is selected, the old version of the        */
/* interface will be emulated (pre M869 Rev D -- M885 Rev F).     */
/*                                                                */
/* The user may select how big an X-window is used to emulate the */
/* display.  All real displays had 1024 x 1024 resolution.  Using */
/* smaller windows saves memory and screen space.                 */
/*                                                                */
/* Note:  The multi-channel scope option is not supported!        */
/******************************************************************/

/* Define the display type */
#define VR14

/* translations from display resolution to scale factor */
#define RES_1024  0
#define RES_512   1
#define RES_256   2
#define RES_128   3

/* select a resolution */
#define PIX_SCALE RES_256


/**********************************************************************/
/* derivation of actual display parameters from user selected options */
/**********************************************************************/

#if defined(VR14)   /* basic point-plot display */
#define CRT "VR14"
#define SETTLING_INTERVAL	(21*microsecond)
#define STATUS_MASK		00003
#endif

#if defined(VR20) /* two color point-plot display */
#define CRT "VR20"
#define SETTLING_INTERVAL	(21*microsecond)
#define STATUS_MASK		00077
#endif

#if defined(TEK602) /* basic point-plot display */
#define CRT "TEK602"
#define SETTLING_INTERVAL	(6*microsecond)
#define STATUS_MASK		00077
#endif

#if defined(TEK611) /* storage scope display */
#define CRT "TEK611"
#define SETTLING_INTERVAL	(6*microsecond)
#define STATUS_MASK		00077
#endif

#if !defined(CRT)
	error misconfigured display options
#endif

/* PIXELS gives the screen dimension in displayed pixels */
#define PIXELS       (1 << (10 - PIX_SCALE))

/* persistance of the phosphor on the screen (about the flicker fusion time) */
#define PERSISTANCE        ( 60 * millisecond )

/* number of points that must be remembered so they can be erased later */
/* the factor of 2 is a safety margin in case software doesn't wait for done */
#define MAX_POINTS   ( 2 * ( PERSISTANCE / SETTLING_INTERVAL ))


/******************/
/* Interface to X */
/******************/

static Display* dpy;            /* its display */
static int      scr;            /* its screen */
static Colormap cmap;           /* its colormap */
static Widget crtshell;         /* the X window shell */
static Widget crt;              /* the X window in which output will plot */

static XColor beam;             /* color for for CRT beam */
#if defined(VR20)
static XColor red;		/* color for red CRT beam */
#endif

static GC whiteGC;              /* gc with white foreground */
static GC blackGC;              /* gc with black foreground */
static GC beamGC;               /* gc with color of CRT beam foreground */
static GC redGC;                /* gc with for VR20 color CRT foreground */
static GC greenGC;              /* gc with for VR20 color CRT foreground */

/* must define type of this proc */
Widget kc8makepopupshell();

static init_x()
{
	Arg arg[25];
	XGCValues gcvalues;
	unsigned int n;

	kc8getinfo( &dpy, &scr );
        cmap = DefaultColormap(dpy, scr);
	crtshell = kc8makepopupshell( CRT );

	/*
	 * Create a drawing area
	 */

	n = 0;
	XtSetArg(arg[n], XtNwidth, PIXELS);                      n++;
	XtSetArg(arg[n], XtNheight, PIXELS);                     n++;
	XtSetArg(arg[n], XtNbackground, BlackPixel( dpy, scr )); n++;
	crt = XtCreateWidget( CRT, widgetClass, crtshell, arg, n);
	XtManageChild(crt);
	XtPopup(crtshell, XtGrabNonexclusive);

	/*
	 * Create black and white Graphics Contexts
	 */

	gcvalues.foreground = BlackPixel( dpy, scr );
	gcvalues.background = BlackPixel( dpy, scr );
	blackGC = XCreateGC(dpy, XtWindow(crt),
			GCForeground | GCBackground,
			&gcvalues);

	gcvalues.foreground = WhitePixel( dpy, scr );
	whiteGC = XCreateGC(dpy, XtWindow(crt),
			GCForeground | GCBackground,
			&gcvalues);

	/*
	 * Create monochrome Graphics Contexts
	 */

	beam.red = 128*256;
	beam.green = 255*256; /* a bright green shade? */
	beam.blue = 128*256;
	if (  XAllocColor( dpy, cmap, &beam )
	   && (beam.pixel != WhitePixel( dpy, scr ))
	   ) { /* we got the color we want! */
		gcvalues.foreground = beam.pixel;
                beamGC = XCreateGC(dpy, XtWindow(crt),
                                GCForeground | GCBackground,
                                &gcvalues);
        } else { /* fake it as white if we can't get that color */
                beam.pixel = WhitePixel( dpy, scr );
                beamGC = whiteGC;
        }

        /* set default colors in case this isn't a VR20 */
	greenGC = beamGC;
	redGC = beamGC;

#if defined(VR20) /* we need the another color */
	red.red = 255*256;
	red.green = 96*256; /* a red shade? */
	red.blue = 96*256;
	if (  XAllocColor( dpy, cmap, &red )
	   && (red.pixel != WhitePixel( dpy, scr ))
	   ) { /* we got the color we want! */
		gcvalues.foreground = red.pixel;
                redGC = XCreateGC(dpy, XtWindow(crt),
                                GCForeground | GCBackground,
                                &gcvalues);
        } else { /* fake it as white if we can't get that color */
                red.pixel = WhitePixel( dpy, scr );
                redGC = whiteGC;
        }
#endif
}

static void display_point(x,y)  /* put a point on the screen */
int x, y;
{
        XDrawPoint(dpy, XtWindow(crt), beamGC, x, PIXELS-y);
}

static void reset_point(x,y)    /* remove a point from the screen */
int x, y;
{
        XDrawPoint(dpy, XtWindow(crt), blackGC, x, PIXELS-y);
}

static void flood_screen(c)	/* flood the screen with color c */
GC c;
{
	XFillRectangle(dpy, XtWindow(crt), c, 0, 0, PIXELS, PIXELS );
}


/*************************************/
/* "officially visible" device state */
/*************************************/

static int x_reg = 0;
static int y_reg = 0;

static int enab_stat_reg;
/* status bits and fields */
#define done_flag     04000
#define write_through 00040
#define store         00020
#define erase         00010
#define color         00004
#define channel_no    00002
#define interrupt     00001


/*********************************/
/* Display Device Implementation */
/*********************************/

/* FIFO queue of points to erase when they decay */
typedef struct point{
	short int x, y; /* coordinates of point */
	long int delay; /* time from previous point to this point */
} Point;
static Point point_queue[MAX_POINTS];
static int tail;   /* enqueue pointer */
static int head;   /* dequeue pointer */
long int queue_interval; /* total time interval between head and tail */

/* array counting how many times each pixel is listed in the point_queue */
static short int point_count[PIXELS][PIXELS];

/* timer used to simulate decay of points on the screen */
static struct timer plot_timer; /* time until next plotted point winks out */

static void remove_point()
{
	int x,y;

	x = point_queue[head].x;
	y = point_queue[head].y;

	if ( point_count[x][y] <= 1) {
		reset_point(x,y);
		point_count[x][y] = 0;
	} else {
		point_count[x][y]--;
	}

	head = (head + 1) % MAX_POINTS;

	if ( head != tail ){
		queue_interval -= point_queue[head].delay;
		schedule( &plot_timer, point_queue[head].delay,
			  remove_point, NULL);
	} else {
		queue_interval = 0;
	}
}

static void add_point()
{
	int x,y;

	/* recall that x and y values are signed;
	   this converts them to unsigned values, origin in lower left */
	x = (x_reg & 01777) ^ 01000;
	y = (y_reg & 01777) ^ 01000;

	/* scale x and y to the displayed number of pixels */
	x = x >> PIX_SCALE;
	y = y >> PIX_SCALE;

	/* x and y are now in the range 0..(PIXELS-1) */

	display_point(x, y);

#if defined(TEK611)
	/* storage scope behavior */
	if (enab_stat_reg & store) {
		/* poor fake of write_through mode */
		if (!(enab_stat_reg & write_through)) {
			point_count[x][y]++;
			return;
		}
	}
#endif

	/* recall that this point has been displayed */
	point_count[x][y]++;

	/* enqueue this point for deletion */
	point_queue[tail].x = x;
	point_queue[tail].y = y;
	if(head == tail){ /* empty list, therefore timer is not set */
		schedule( &plot_timer, PERSISTANCE, remove_point, NULL);
		queue_interval = 0;
	} else {
		long int d;
		d = PERSISTANCE - (query_timer( &plot_timer ) + queue_interval);
		point_queue[tail].delay = d;
		queue_interval += d;
	}
	/* note that, for software that ignores the done bit,
	   the following code could overrun the head pointer */
	tail = (tail + 1) % MAX_POINTS;
}


/**********************************/
/* internal behavior of interface */
/**********************************/

static struct timer dilx_timer; /* time delay for dilx */
static struct timer dily_timer; /* time delay for dily */
static struct timer VR20_timer; /* time delay for color change */
static struct timer TEK_timer;  /* time delay for erase pulse */
static int done_timer_count;

static setdoneflag(p)
int p;
{
	/* only set done if all done-timers are expired */
	if (done_timer_count > 0) {
		done_timer_count--;
	}
	if (done_timer_count == 0) {
		if (!(enab_stat_reg & done_flag)) {
			enab_stat_reg |= done_flag;
			if (enab_stat_reg & interrupt) {
				irq = irq + 1;
			}
		}
	}
}

static enderase(p)
int p;
{
	int i, j;
	for(i =0; i < PIXELS; i++)
		for( j=0; j < PIXELS; j++)
			point_count[i][j] = 0;
	flood_screen( blackGC );
	/* this here? ---- ac = enab_stat_reg & ~erase;  ---*/
	setdoneflag(p);
}

static resetdoneflag()
{
	if (enab_stat_reg & done_flag) {
		enab_stat_reg &= ~done_flag;
		if (enab_stat_reg & interrupt) {
			irq = irq + 1;
		}
	}
}

/******************/
/* Initialization */
/******************/


vc8epower() /* global initialize */
{
	int i,j;

	/* Initializing display list to NULL */
	init_timer(plot_timer);
	head = 0;
	tail = 0;

	for(i =0; i < PIXELS; i++)
		for( j=0; j < PIXELS; j++)
			point_count[i][j] = 0;
	
	/* Initializing timers for delays until done to NULL */
	init_timer(dilx_timer);
	init_timer(dily_timer);
	init_timer(VR20_timer);
	init_timer(TEK_timer);
	done_timer_count = 0;

	/* Do X initialization */
	init_x();
}


vc8einit() /* console reset */
{
	enab_stat_reg = 0;
}

/********************/
/* IOT Instructions */
/********************/

vc8edev5(op)
int op;
{
        switch (op) {

        case 00: /* DILC */
		enab_stat_reg = 0;
                break;

        case 01: /* DICD */
		resetdoneflag();
                break;

        case 02: /* DISD */
                if (enab_stat_reg & done_flag) {
			pc = (pc+1) & 07777;
                }
                break;

        case 03: /* DILX */
		resetdoneflag();
		x_reg =  ac & 01777;  /* 10 bits only! */
		/* wait for to settle */
		done_timer_count++;
		schedule(&dilx_timer, SETTLING_INTERVAL, setdoneflag, NULL);
                break;

        case 04: /* DILY */
		resetdoneflag();
		y_reg = ac & 01777;  /* 10 bits only! */
		/* wait for to settle */
		done_timer_count++;
		schedule(&dily_timer, SETTLING_INTERVAL, setdoneflag, NULL);
                break;

        case 05: /* DIXY */
		resetdoneflag();

		/* intensify */
		add_point();

		/* no delay needed here, intensify pulse is 1 microsec */
		done_timer_count++;
		setdoneflag(1);
                break;

        case 06: { /* DILE */
			int old_stat = enab_stat_reg;
			enab_stat_reg |= ac & STATUS_MASK;

			/* things that cause delays are tested in order of
			   increasing delay in order to get the maximum
			   of all the conditions that may cause any delay */
			if ((old_stat ^ enab_stat_reg) & color) {
				/* -- VR20 color change -- */
				long int delay;
				/* is this needed? ---- resetdoneflag() */
				if (enab_stat_reg & color) { /* green to red */
					beamGC = redGC;
					delay = 300 * microsecond;
				} else { /* red to green */
					beamGC = greenGC;
					delay = 1600 * microsecond;
				}
				done_timer_count++;
				schedule(&VR20_timer, delay, setdoneflag, NULL);
			}
			if (enab_stat_reg & erase) {
				/* -- erase Tex storage scope screen -- */
				long int delay;
				/* is this needed? ---- resetdoneflag() */
				flood_screen( beamGC );
				ac = enab_stat_reg & ~erase;  /*this here---*/
				delay = 50 * millisecond;
				done_timer_count++;
				schedule(&TEK_timer, delay, enderase, NULL);
			}
			ac = 00000;
		}
                break;

        case 07: /* DIRE */
		ac = enab_stat_reg;
                break;

        }
}
