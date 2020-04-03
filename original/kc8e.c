/* File: kc8e.c
   Author: Douglas Jones, Dept. of Comp. Sci., U. of Iowa, Iowa City, IA 52242.
   Date: Feb. 29, 1996
   Language: C (UNIX)
   Purpose: DEC KC8/E control panel emulator, using X and Xt widgets.

   Based on the description in the PDP-8/E Maintenance manual,
   Digital Equipment Corporation, 1971, liberally interpreted.

*/

#include <stdio.h>
#include <strings.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Core.h>
#include <X11/Shell.h>
#define XK_MISCELLANY
#include <X11/keysymdef.h>

#include "realtime.h"
#include "bus.h"


/***************************/
/* User controlled options */
/***************************/

/* default frequency for console display update and X-events polling */
#define default_console_interval (10 * millisecond + 1)

/* characteristics of heartbeat display on control panel */
#define heartbeat_interval (1L * second)
#define on_interval (100L * millisecond)

/* height of console, in pixels, divisible by 30; width is 2.1 times this */
#define PDP8height 300


/**************************/
/* Window data structures */
/**************************/

XtAppContext app_context;	 /* the topmost context for everything */
static Widget	appshell;        /* the topmost widget */
static Widget	frontpanelshell; /* a framework for the front panel */
static Widget	frontpanel;      /* the console front panel */


/*******************************/
/* tools for making frontpanel */
/*******************************/

static Display*	dpy;		/* its display */
static int	scr;		/* its screen */
static Colormap	cmap;		/* its colormap */

#define bright 65535		/* fully on color value */
static XColor grey;		/* color for for grey */
static XColor darkgrey;		/* color for for darker shade of grey */
static XColor goldenrod;	/* .. for goldenrod (when in doubt, grey) */
static XColor terracotta;	/* .. for terracotta (in doubt, dark grey) */

/* 16 by 16 bitmap data for dark shades */
static char dark_bits[] = {
   0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa,
   0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa,
   0x55, 0x55, 0xaa, 0xaa, 0x55, 0x55, 0xaa, 0xaa};
static Pixmap dark_bitmap;	/* used to darken shades (uses darkbits) */

/* fonts */
static XFontStruct* headinfo;	/* info about headline font */
static XFontStruct* subinfo;	/* info about font for subheadline */
static XFontStruct* tinyinfo;	/* info about tiny font used to label things */
static XFontStruct* helpinfo;	/* info about font used for help messages */

/* all GC's have black backgrounds */
static GC whiteGC;		/* gc with white foreground, tinyfont */
static GC greyGC;		/* gc with grey foreground, tinyfont */
static GC darkgreyGC;		/* gc with dark grey foreground, tinyfont */
static GC headGC;		/* gc with big headline font */
static GC subGC;		/* gc with modest subheadline font */
static GC blackGC;		/* gc with black foreground */
static GC goldGC;		/* gc with goldenrod foreground */
static GC darkgoldGC;		/* gc with dark goldenrod foreground */
static GC terraGC;		/* gc with terracotta foreground */
static GC darkterraGC;		/* gc with dark terracotta foreground */
static GC helpGC;		/* gc used for help messages */


/*****************************************/
/* tools for filling a window with boxes */
/*****************************************/

struct box_rec {
	int x,y;	  /* where to put box (upper left corner) */
	int width,height; /* how big is rectangle */
	GC boxGC;	  /* GC to use */
	char rule;	  /* drawing rule 0 for outline, 1 for solid */
	char* label;	  /* character string to use as label */
	GC labGC;	  /* GC to use for label (if label != NULL) */
	XFontStruct* labinfo;	/* data about font (if label != NULL) */
	int lablen;	  /* label length (if label != NULL) */
	int labx,laby;	  /* coordinates of label (if label != NULL) */
	void (* press)(); /* hook to call when mouse click in box */
			  /* pointer to box is passed as param to hook */
	char* help;	  /* help message to display for box */
	int state;	  /* state of box */
	struct box_rec* next;    /* pointer to next box in console */
	struct box_rec* sub;     /* pointer to subsidiary boxes */
	struct box_rec* replot;  /* pointer to box to replot (when needed) */
};
#define NOBOX (struct box_rec *)NULL
#define blankbox 0
#define linebox 1
#define fillbox 2
#define linefillbox 3
#define circlebox 4

static boxshow( box )
struct box_rec* box;
{
	struct box_rec* b;

	b = box->sub;

	if (box->rule == linebox) { /* outline */
		while (b != NOBOX) { /* put outline over sub boxes */
			boxshow( b );
			b = b->next;
		}
		XDrawRectangle( dpy, XtWindow(frontpanel), box->boxGC,
				box->x, box->y, box->width-1, box->height-1 );
	} else if (box->rule == fillbox) { /* solid */
		XFillRectangle( dpy, XtWindow(frontpanel), box->boxGC,
				box->x, box->y, box->width, box->height );
		while (b != NOBOX) { /* put sub boxes over filled region */
			boxshow( b );
			b = b->next;
		}
	} else if (box->rule == linefillbox) { /* solid outlined box */
		XFillRectangle( dpy, XtWindow(frontpanel), box->boxGC,
				box->x, box->y, box->width, box->height );
		while (b != NOBOX) { /* put outline over sub boxes */
			boxshow( b );
			b = b->next;
		}
		XDrawRectangle( dpy, XtWindow(frontpanel), whiteGC,
				box->x, box->y, box->width-1, box->height-1 );
	} else if (box->rule == circlebox) { /* lightbulb */
		XFillArc( dpy, XtWindow(frontpanel), box->boxGC,
				box->x, box->y, box->width, box->height,
				0,360*64 );
	} else {
		while (b != NOBOX) { /* put outline over sub boxes */
			boxshow( b );
			b = b->next;
		}
	}
	if (box->label != NULL) {
		XDrawString( dpy, XtWindow(frontpanel), box->labGC,
			     box->labx, box->laby, box->label, box->lablen);
	}
}

/* box records globally known to boxpress so it can implement help requests */
static struct box_rec* helpbox;
static struct box_rec* helpok;

static showhelpmsg( m )
char * m;
/* make a help message appear, with OK button armed to make it go away */
{
	helpbox->label = m;
	helpbox->lablen = strlen(helpbox->label);
	helpbox->rule = linefillbox;
	helpbox->sub = helpok;
	boxshow( helpbox );
}

static boxpress( x, y, button, box )
int x, y;
unsigned int button;
struct box_rec* box;
{
	struct box_rec* child;

	/* a mouse button was pressed in box */
	/* step 1:  was it in a sub-box of box? */
	child = box->sub;
	while (child != NOBOX) {
		if (  (x > child->x) && (x < (child->x + child->width) )
		   && (y > child->y) && (y < (child->y + child->height) )
		   ) {
			boxpress( x, y, button, child );
			return;
		}
		child = child->next;
	}

	/* step 2:  if not in a sub-box, */
	if (button == Button1) { /* take action */
		(* box->press)(box);
	} else if (box->help != NULL) { /* offer help */
		showhelpmsg( box->help );
	}
}

static struct box_rec* makebox( b )
struct box_rec* b;
/* copy from a prototype box to the real thing; used because many boxes
   can frequently be copied from one prototype, with only a few changed
   parameters for each
*/
{
	struct box_rec* n;
	XCharStruct overall;
	int dir, up, down;
	n = (struct box_rec*)malloc( sizeof(struct box_rec) );
	n->x = b->x;
	n->y = b->y;
	n->width = b->width;
	n->height = b->height;
	n->boxGC = b->boxGC;
	n->label = b->label;
	n->lablen = b->lablen;
	n->labx = b->labx;
	n->laby = b->laby;
	n->labGC = b->labGC;
	n->labinfo = b->labinfo;
	if (b->label != NULL) {
		if (b->lablen==0) {
			n->lablen = strlen(b->label);
		}
		if ((b->labx==0)||(b->laby==0)) {
			XTextExtents( n->labinfo, n->label, n->lablen,
				     &dir, &up, &down, &overall );
		}
		if (b->labx==0) {
			int w = overall.rbearing - overall.lbearing;
			n->labx = b->x+(b->width/2)-(w/2)-overall.lbearing;
		}
		if (b->laby==0) {
			int h = overall.ascent + overall.descent;
			n->laby = b->y+(b->height/2)+(h/2)-overall.descent;
		}
	}
	n->rule = b->rule;
	n->press = b->press;
	n->help = b->help;
	n->state = b->state;
	n->next = b->next;
	n->sub = b->sub;
	return n;
}

/***************************************************************/
/* tools for making PDP-8/E/F style toggle switches from boxes */
/***************************************************************/

static int toggleswitch(b)
struct box_rec* b;
/* change state of switch with top-level box b */
{
	struct box_rec* sup = b->replot;
	struct box_rec* sub = sup->sub;
	if (sub->height < (sup->height/2)) { /* switch is up, put down */
		sub->height = sup->height - sup->height/5;
		boxshow(sup);
		return 0;
	} else { /* switch is down, put up */
		sub->height = sup->height/5;
		boxshow(sup);
		return 1;
	}
}

struct box_rec* toggle = NOBOX;
static void bounceinternal()
/* called internally to make a momentary switch change back */
{
	(void) toggleswitch( toggle );
	toggle = NOBOX;
}

static void bounceswitch(b)
/* called to mark a button-press on a momentary switch */
struct box_rec* b;
{
	(void) toggleswitch(b);
	changeregs();
	toggle = b;
}

static struct box_rec* makeswitch( b, state )
struct box_rec* b;
int state;
/* called to make a switch from prototype b with initial state 0 or 1 */
{
	struct box_rec* n = makebox( b );
	struct box_rec* sub = makebox( b );

	/* n will be the box outlining the switch, sub will be the toggle */
	if (state == 0) { /* switch down */
		sub->height = b->height - b->height/5;
	} else { /* switch up */
		sub->height = b->height/5;
	}
	sub->boxGC = b->labGC;
	sub->next = NOBOX;
	sub->sub = NOBOX;
	sub->replot = n;
	n->sub = sub;
	n->replot = n;

	return n;
}


/*****************************************************/
/* code to update the display of PDP-8/E/F registers */
/*****************************************************/

/* where are boxes for each indicator light and some of the switches */
static struct box_rec* cpmaboxes[15];	/* bits of cpma */
static struct box_rec* runbox;		/* run */
static struct box_rec* databoxes[12];	/* data display */
static struct box_rec* srboxes[12];	/* switch register */

/* state of data display */
static int datastate = 3;
#define showstate 0
#define showstatus 1
#define showac 2
#define showmd 3
#define showmq 4
#define showbus 5
#define showmax 6
static struct box_rec* showdatabox[showmax];  /* boxes in data select menu */

static int getdata()
{
	switch (datastate) {
	case showstate:
		return (1 << 11) | (run << 10) | (run << 9)
			| ((mb >> 3) & 0700)
			| (sw << 3);
	case showstatus:
		return (link >> 1) | ((irq == 0) << 9) | (enab << 7)
#ifdef KE8E
			| (gtf << 10)
#endif
#ifdef KM8E
			| (uf << 6) | (ifr >> 9) | (dfr >> 12)
#endif
		;
	case showac:
		return ac;
	case showmd:
		return mb;
	case showmq:
		return mq;
	case showbus:
		return mb^ac;  /* a kluge */
	}
}

static changeregs()
{
	int x;
	int data = getdata();
	struct box_rec* boxx;
	for (x = 0; x < 15; x++) {
		boxx = cpmaboxes[x];
		if (cpma & (040000 >> x)) { /* bit is one */
			if (boxx->boxGC != whiteGC) { /* bit changed */
				boxx->boxGC = whiteGC;
				boxshow( boxx );
			}
		} else { /* bit is zero */
			if (boxx->boxGC != blackGC) { /* bit changed */
				boxx->boxGC = blackGC;
				boxshow( boxx );
			}
		}
	}

	if (run != 0) { /* running */
		if (runbox->boxGC != whiteGC) {
			runbox->boxGC = whiteGC;
			boxshow( runbox );
		}
	} else { /* halted */
		if (runbox->boxGC != blackGC) {
			runbox->boxGC = blackGC;
			boxshow( runbox );
		}
	}

	for (x=0; x<12; x++) {
		if (data & (04000 >> x)) { /* bit is one */
			if (databoxes[x]->boxGC != whiteGC) {
				databoxes[x]->boxGC = whiteGC;
				boxshow( databoxes[x] );
			}
		} else { /* bit is zero */
			if (databoxes[x]->boxGC != blackGC) {
				databoxes[x]->boxGC = blackGC;
				boxshow( databoxes[x] );
			}
		}
	}
}


/**********************************************/
/* Implementation of device control interface */
/**********************************************/

struct device_rec {
	int (* mount)(); /* hook to mount file on device */
	void (* dismount)(); /* hook to dismount mounted file */
	int unit; /* thing to pass as param to mount and dismount */
	char * name; /* device name */
	char * longname; /* descriptive device name */
	char * file; /* file attached to device */
	struct device_rec * next; /* next list element */
};
static struct device_rec * devices = NULL;

void register_device(m,d,u,n,l,f)
int (* m)(); /* hook to mount file on device */
void (* d)(); /* hook to dismount mounted file */
int u; /* device unit */
char * n; /* device name */
char * l; /* descriptive device name */
char * f; /* file attached to device */
{
	struct device_rec * temp;
	temp = (struct device_rec *)malloc( sizeof( struct device_rec ) );
	temp -> mount = m;
	temp -> dismount = d;
	temp -> unit = u;
	temp -> name = n;
	temp -> longname = l;
	temp -> file = f;
	if (devices == NULL) { /* make a singleton list */
		temp->next = temp;
		devices = temp;
	} else { /* add to linked list */
		temp->next = devices->next;
		devices->next = temp;
	}
}

close_devices()
{
	struct device_rec * temp = devices;
	do {
		(* (temp -> dismount))( temp -> unit );
		temp = temp -> next;
	} while (temp != devices);
}

dump_devices( f )
FILE *f;
{
	struct device_rec * temp = devices;
	do {
		if ((temp -> file)[0] != '\0') { /* device is mounted */
			fputs( "m ", f );
			fputs( temp -> name, f );
			fputs( " ", f );
			fputs( temp -> file, f );
			fputs( "\n", f );
		}
		temp = temp -> next;
	} while (temp != devices);
}

void mount_device( n, f )
char * n;
char * f;
{
	/* quietly try to mount files on devices, used during startup */
	struct device_rec * temp = devices;
	struct device_rec * d = NULL;
	do {
		if (strcmp(temp -> name, n) == 0) { /* they match */
			d = temp;
			break;
		}
		temp = temp -> next;
	} while (temp != devices);
	if (d != NULL) { /* a real device was specified */
		int u = d -> unit;
		if ( ((d -> file)[0] == '\0') && (f[0] != '\0') ) {
			/* so long as a device is not mounted
			   and so long as a nonblank name was provided
			   try to do the mount */
			(void)(*(d -> mount))( u, f );
			/* ignore any error codes */
		}
	}
}


/******************************************/
/* button press event handling procedures */
/******************************************/

static void (* keypress)() = NULL; /* hook to call for keypress (char param) */

static void backpress(b)
struct box_rec* b;
/* mouseclick in background, do nothing */
{
}

/* box records globally known to logopress so special panel can be toggled */
static struct box_rec* logobox;  /* the box holding logo or control panel */
static struct box_rec* decbox;   /* the box holding the DEC logo */
static struct box_rec* panelbox; /* the box holding panel */

/* heartbeat */
static struct box_rec* heartbeat_box; /* heartbeat on the alternate panel*/
static struct timer heartbeat_delay;

static void heartbeat()
{
	if (heartbeat_box->boxGC == blackGC) {
		heartbeat_box->boxGC = greyGC;
		schedule( &heartbeat_delay, on_interval,
			  heartbeat, 0 );
	} else {
		heartbeat_box->boxGC = blackGC;
		schedule( &heartbeat_delay, heartbeat_interval - on_interval,
			  heartbeat, 0 );
	}
	if (logobox->sub == panelbox) {
		boxshow( heartbeat_box );
	}
}

static void helppress(b)
struct box_rec* b;
{
	showhelpmsg( b->help );
}

static void powerpress(b)
struct box_rec* b;
/* mouseclick in exit button or power control area, shutdown */
{
	/* ... first clean up X ... */
	powerdown();
}

#ifdef DEBUG
static void tracepress(b)
struct box_rec* b;
/* mouse click in button requesting diagnostic trace information */
{
	output_debug();
}
#endif

/* control of console refresh rate and button clicks to change it */
long int console_interval = default_console_interval;
struct box_rec* console_interval_box; /* display box for interval */

static void minuspress(b)
struct box_rec* b;
/* mouse click on minus box to decrease console interval */
{
	if (console_interval > microsecond) {
		console_interval = (console_interval * 2) / 3;
	}
	(void) sprintf( console_interval_box->label,
			"%ld", console_interval/microsecond );
	console_interval_box->lablen = strlen(console_interval_box->label);
	boxshow( console_interval_box );
}

static void pluspress(b)
struct box_rec* b;
/* mouse click on minus box to decrease console interval */
{
	if (console_interval < second) {
		console_interval = (console_interval * 3) / 2;
	}
	(void) sprintf( console_interval_box->label,
			"%ld", console_interval/microsecond );
	console_interval_box->lablen = strlen(console_interval_box->label);
	boxshow( console_interval_box );
}

/* device display control state */
struct box_rec* device_name_box; /* display box for device name */
struct box_rec* file_name_box; /* display box for file name */
static int filenameactive = 0; /* state of file name acquisition routines */

static void dnpress(b)
struct box_rec* b;
/* mouse click down box to scroll through list of devices */
{
	if (filenameactive == 0) { /* only allowed if not in mid name change */
		devices = devices->next;
		device_name_box->label = devices->name;
		device_name_box->lablen = strlen( devices->name );
		device_name_box->help = devices->longname;
		file_name_box->label = devices->file;
		file_name_box->lablen = strlen( devices->file );
		boxshow( device_name_box );
		boxshow( file_name_box );
	} else {
		showhelpmsg( "please finish typing a new file name" );
	}
}

static void filekeypress(c)
char c;
/* keypress in file name box */
{
	int pos = file_name_box->lablen;
	if (c == '\r') {
		/* done gathering file name, reset same as mouse click */
		filenameactive = 0;
		keypress = NULL;

		/* dehighlight typing box */
		file_name_box->boxGC = darkgreyGC;

		if (pos > 0) {
			file_name_box->label[ pos ] = 0;
			if(!(*(devices->mount))(devices->unit,devices->file)){
				showhelpmsg(
				 "this file cannot be mounted on that device");
			}
			file_name_box->lablen = strlen( devices->file );
		}
		boxshow( file_name_box );
	} else if ((c >= ' ') & (c <= '~')) {
		if (pos < NAME_LENGTH) {
			file_name_box->label[ pos ] = c;
			file_name_box->lablen = pos + 1;
			boxshow( file_name_box );
		}
	} else if (c == '\b') {
		if (pos > 0) {
			file_name_box->lablen = pos - 1;
			boxshow( file_name_box );
		}
	}
}

static void filepress(b)
struct box_rec* b;
/* mouseclick in the box to mount a file on a device */
{
	if (filenameactive == 0) {
		/* if not gathering file name, set state to gathering name */
		filenameactive = 1;

		/* first dismount previous file */
		(* (devices->dismount))( devices->unit );

		/* make sure result of dismount is displayed */
		file_name_box->label = devices->file;
		file_name_box->lablen = strlen( devices->file );

		/* highlight typing box */
		file_name_box->boxGC = greyGC;

		boxshow( file_name_box );
		keypress = filekeypress;
	} else {
		/* if gathering file name, finish up as if return pressed */
		int pos = file_name_box->lablen;
		filenameactive = 0;
		keypress = NULL;

		/* dehighlight typing box */
		file_name_box->boxGC = darkgreyGC;

		if (pos > 0) {
			file_name_box->label[ pos ] = 0;
			if(!(*(devices->mount))(devices->unit,devices->file)){
				showhelpmsg(
				 "this file cannot be mounted on that device");
			}
			file_name_box->lablen = strlen( devices->file );
		}
		boxshow( file_name_box );
	}
}

static void logopress(b)
struct box_rec* b;
/* toggle between DEC logo and special control panel */
{
	if (filenameactive == 0) { /* do it only if file name is entered */
		if (logobox->sub == decbox) {
			logobox->sub = panelbox;
		} else {
			logobox->sub = decbox;
		}
		boxshow( logobox );
	} else {
		showhelpmsg( "please finish typing a new file name");
	}
}

static void datapress(b)
struct box_rec* b;
/* mouseclick in data display selection area */
{
	/* unhighlight previously selected option */
	showdatabox[datastate]->rule = blankbox;

	if (b->state < 0) { /* roll through data selection options */
		datastate = (datastate + 1) % showmax;
	} else { /* select a specific data selection option */
		datastate = b->state;
	}

	/* highlight newly selected option */
	showdatabox[datastate]->rule = linebox;
	boxshow( b->replot );
	changeregs();
}

static void swpress(b)
/* mouseclick on sw switch */
struct box_rec* b;
{
	sw = toggleswitch(b);
	changeregs();
}

static void srpress(b)
/* mouseclick on some switch in switch register */
struct box_rec* b;
{
	int bit = toggleswitch(b);
	int bitnum = b->state;
	sr = (sr & ~(04000 >> bitnum)) | (bit << (11-bitnum));
}

static void addrloadpress(b)
/* mouseclick on load address switch */
struct box_rec* b;
{
	if (run == 0) {
		pc = sr;
#ifdef KM8E
		cpma = pc | ifr;
#else
		cpma = pc;
#endif
		if (cpma < MAXMEM) {
			mb = memory[cpma];
		} else {
			mb = 0;
		}
	}
	bounceswitch(b);
}

static void extdaddrloadpress(b)
/* mouseclick on load extended address switch */
struct box_rec* b;
{
#ifdef KM8E
	if (run == 0) {
		ib = (sr << 9) & 070000;
		ifr = ib;
		dfr = (sr << 12) & 070000;
		cpma = pc | ifr;
	}
#endif
	bounceswitch( b );
}

static void clearpress(b)
/* mouseclick on clear switch */
struct box_rec* b;
{
	if (run == 0) {
		clearflags();
	}
	bounceswitch( b );
}

static void contpress(b)
/* mouseclick on cont switch */
struct box_rec* b;
{
	if (run == 0) {
		run = 1;
	}
	bounceswitch( b );
}

static void exampress(b)
/* mouseclick on exam switch */
struct box_rec* b;
{
	if (run == 0) {
#ifdef KM8E
		cpma = (pc | ifr);
#else
		cpma = (pc);
#endif
		if (cpma < MAXMEM) {
			mb = memory[cpma];
		}
		pc = (pc + 1) & 07777;
#ifdef KM8E
		cpma = (pc | ifr);
#else
		cpma = (pc);
#endif
	}
	bounceswitch( b );
}

static void haltpress(b)
/* mouseclick on halt switch */
struct box_rec* b;
{
	run = 0;
	bounceswitch( b );
}

static void steppress(b)
/* mouseclick on single step switch */
struct box_rec* b;
{
	run = 2;
	bounceswitch( b );
}

static void deppress(b)
/* mouseclick on deposit switch */
struct box_rec* b;
{
	if (run == 0) {
#ifdef KM8E
		cpma = (pc | ifr);
#else
		cpma = (pc);
#endif
		mb = sr;
		if (cpma < MAXMEM) {
			memory[cpma] = mb;
		}
		pc = (pc + 1) & 07777;
#ifdef KM8E
		cpma = (pc | ifr);
#else
		cpma = (pc);
#endif
	}
	bounceswitch( b );
}

static void helpokpress(b)
/* mouseclick on ok button in help message */
struct box_rec* b;
{
	/* the following makes the help message box go away */
	/* this undoes the work done by showhelp */
	helpbox->rule = fillbox;
	helpbox->label = NULL;
	helpbox->sub = NOBOX;
	boxshow(helpbox);
	helpbox->rule = blankbox;
}



/******************************************************/
/* code to build the actual PDP-8 control panel image */
/******************************************************/

/* the top-level box on the screen */
static struct box_rec* boxlist;

static makeboxes()
#define h PDP8height /* full control panel height */
#define i (h/10)     /* top and side indent margin */
#define p (h/20)     /* partial margin indent */
#define c (h/30)     /* caption bar width */
#define t (h/80)     /* thin line width */
#define s (h/15)     /* switch width */
{
	int x;
	struct box_rec b;
	struct box_rec* sublist;

	/* box structures use indenting to show hierarchical relationship */

	b.x = i-t; b.y = i-t;
	b.width = 2*h-i+2*t; b.height = i+p;
	b.boxGC = blackGC;
	b.label = NULL;
	b.rule = linefillbox;
	b.press = backpress;
	b.help =
"This area toggles between DEC's original artwork and the emulator controls";
	b.next = NOBOX; b.sub = NOBOX;
	boxlist = makebox(&b); /* the logo bar across the top */
	{
		b.width = (4*i)/7; b.height = i-t;
		b.boxGC = whiteGC;
		b.lablen = 1;
		b.labx = 0;
		b.laby = i+s-(t/2);
		b.labGC = headGC;
		b.labinfo = headinfo;
		b.rule = linebox;
		b.press = logopress;
		b.state = 0;
		sublist = NULL; b.sub = NULL;
		for (x=0; x<7; x++) {
			b.x = 2*i+c/2+x*(4*i)/7; b.y = i;
			b.next = sublist;
			b.label = "digital"+x;
			sublist = makebox(&b); /* add logo stuff to bar */
			b.label = NULL;
		}

		b.x = i; b.y = i;
		b.width = 2*h-i; b.height = i-t;
		b.boxGC = terraGC;
		b.label = "pdp8/e";
		b.lablen = 0;
		b.labx = 6*i+s;
		b.rule = linefillbox;
		b.next = NOBOX; b.sub = sublist;
		sublist = makebox(&b); /* the logo stripe */

		b.x = i; b.y = 2*i-t;
		b.width = 2*h-i; b.height = p;
		b.boxGC = goldGC;
		b.label = "digital equipment corporation maynard massachusetts";
		b.labx = 2*i+c/2;
		b.laby = 0;
		b.labGC = subGC;
		b.labinfo = subinfo;
		b.next = sublist;
		sublist = makebox(&b); /* the company name stript */
	}
	decbox = sublist; /* keep real logo as alternate bar here */
	{
		b.x = i; b.y = 2*i+p-s-t;
		b.width = s-t; b.height = s-t;
		b.rule = linefillbox;
		b.boxGC = darkgreyGC;
		b.label = "+";
		b.labGC = subGC;
		b.labinfo = subinfo;
		b.labx = 0;
		b.press = dnpress;
		b.help =
"click the left button here to cycle through the list of devices";
		b.next = NOBOX; b.sub = NOBOX;
		sublist = makebox(&b);

		b.x = i+s; b.y = i+c-t;
		b.width = 3*s-t; b.height = s-t;
		b.rule = blankbox;
		b.label = "DEVICE";
		b.labx = i+s+p-t;
		b.press = backpress;
		b.help =
"the device name is displayed below";
		b.next = sublist; b.sub = NOBOX;
		sublist = makebox(&b);

		b.x = i+4*s; b.y = i+c-t;
		b.width = 3*s-t; b.height = s-t;
		b.label = "FILE";
		b.labx = i+4*s+p;
		b.press = backpress;
		b.help =
"the file attached to the device is displayed below";
		b.next = sublist; b.sub = NOBOX;
		sublist = makebox(&b);

		b.x = i+s; b.y = 2*i+p-s-t;
		b.width = 3*s-t; b.height = s-t;
		b.rule = fillbox;
		b.boxGC = blackGC;
		b.label = devices->name;
		b.labx = i+s+p-t;
		b.press = backpress;
		b.help = devices->longname;
		b.next = sublist; b.sub = NOBOX;
		sublist = makebox(&b);
		device_name_box = sublist;

		b.x = i+4*s; b.y = 2*i+p-s-t;
		b.width = 12*s-t; b.height = s-t;
		b.rule = linefillbox;
		b.boxGC = darkgreyGC;
		b.label = devices->file;
		b.labx = i+4*s+p;
		b.laby = sublist->laby; /* get good pos, label may be blank */
		b.press = filepress;
		b.help =
"click the left button here to mount a new file on the device";
		b.next = sublist; b.sub = NOBOX;
		sublist = makebox(&b);
		file_name_box = sublist;

		b.x = i+16*s-c-t; b.y = i+c;
		b.width = c-t; b.height = c-t;
		b.rule = circlebox;
		b.boxGC = blackGC;
		b.label = " ";
		b.labx = 0;
		b.laby = 0;
		b.press = backpress;
		b.help =
"This blinks once per simulated second while the emulator runs";
		b.next = sublist; b.sub = NOBOX;
		sublist = makebox(&b);
		heartbeat_box = sublist;

#ifdef DEBUG
		b.x = 2*h-11*s; b.y = i;
		b.width = 2*s+s/2; b.height = s-t;
		b.rule = linefillbox;
		b.boxGC = darkgreyGC;
		b.label = "TRACE";
		b.press = tracepress;
		b.help =
"click here for a list of locations and instrucitons executed";
		b.next = sublist; b.sub = NOBOX;
		sublist = makebox(&b);
#endif

		b.x = 2*h-8*s; b.y = i;
		b.width = 2*s+s/2; b.height = s-t;
		b.rule = linefillbox;
		b.boxGC = darkgreyGC;
		b.label = "AUTHOR";
		b.press = helppress;
		b.help =
"This emulator was written by Douglas W. Jones, jones@cs.uiowa.edu";
		b.next = sublist; b.sub = NOBOX;
		sublist = makebox(&b);

		b.x = 2*h-8*s; b.y = 2*i+p-s-t;
		b.width = 2*s+s/2; b.height = s-t;
		b.label = "CREDIT";
		b.help =
"PDP-8 and d|i|g|i|t|a|l are trademarks of Digital Equipment Corporation";
		b.next = sublist; b.sub = NOBOX;
		sublist = makebox(&b);

		b.x = 2*h-5*s; b.y = i;
		b.width = 2*s-t; b.height = s-t;
		b.label = "EXIT";
		b.press = powerpress;
		b.help =
"click the left button here to exit the emulator (same as the power switch)";
		b.next = sublist; b.sub = NOBOX;
		sublist = makebox(&b);

		b.x = 2*h-3*s; b.y = i;
		b.width = 2*s-t; b.height = s-t;
		b.label = "HELP";
		b.press = helppress;
		b.help =
"click the right button on any box for help message like this about that box";
		b.next = sublist; b.sub = NOBOX;
		sublist = makebox(&b);

		b.x = 2*h-s; b.y = i;
		b.width = s; b.height = s-t;
		b.label = "OK";
		b.press = logopress;
		b.help =
"click the left button here to restore the original DEC artwork";
		b.next = sublist; b.sub = NOBOX;
		sublist = makebox(&b);

		b.x = 2*h-5*s; b.y = 2*i+p-s-t;
		b.width = s; b.height = s-t;
		b.label = "-";
		b.press = minuspress;
		b.help =
"click the left button here to decrease the time between display updates";
		b.next = sublist; b.sub = NOBOX;
		sublist = makebox(&b);

		b.x = 2*h-4*s; b.y = 2*i+p-s-t;
		b.width = 3*s; b.height = s-t;
		b.rule = fillbox;
		b.boxGC = blackGC;
		b.label = (char *) malloc( 24 );
		(void) sprintf( b.label, "%ld", console_interval/microsecond );
		b.press = backpress;
		b.help =
"This is the time between display updates, in simulated microseconds";
		b.next = sublist; b.sub = NOBOX;
		sublist = makebox(&b);
		console_interval_box = sublist;

		b.x = 2*h-s; b.y = 2*i+p-s-t;
		b.width = s; b.height = s-t;
		b.rule = linefillbox;
		b.boxGC = darkgreyGC;
		b.label = "+";
		b.press = pluspress;
		b.help =
"click the left button here to increase the time between display updates";
		b.next = sublist; b.sub = NOBOX;
		sublist = makebox(&b);
	}
	panelbox = sublist;
	boxlist->sub = panelbox; /* patch in logo bar contents */
	logobox = boxlist;

	b.x = 5*i-t; b.y = 3*i-c;
	b.width = 8*i; b.height = c;
	b.rule = blankbox;
	b.label = "MEMORY ADDRESS";
	b.labx = 0;
	b.labGC = whiteGC;
	b.labinfo = tinyinfo;
	b.press = backpress;
	b.help = "the memory address register is displayed below here";
	b.next = boxlist; b.sub = sublist;
	boxlist = makebox(&b); /* title for bar above mem addr lights */


	{
		sublist = NOBOX;
		b.width = 2*i; b.height = c;
		b.rule = linefillbox;
		b.sub = NOBOX;
		b.label = "EMA";
		b.help =
"the extended memory address (IF or DF) is displayed below here";
		for (x=0; x<5; x++) {
			b.x = 3*i-t+x*2*i; b.y = 3*i;
			if (x & 1) { /* odd boxes */
				b.boxGC = terraGC;
			} else { /* even boxes */
				b.boxGC = goldGC;
			}
			b.next = sublist;
			sublist = makebox(&b); /* add color to bar */
			b.label = NULL;
			b.help =
"the memory address register is displayed below here";
		}
	}
	b.x = 3*i-t; b.y = 3*i;
	b.width = 10*i; b.height = c;
	b.rule = blankbox;
	b.next = boxlist; b.sub = sublist;
	boxlist = makebox(&b); /* bar across the top of mem addr lights */

	{
		sublist = NOBOX;
		b.width = c-t; b.height = c-t;
		b.rule = circlebox;
		b.press = backpress;
		b.help = "the memory address register is displayed here";
		for (x=0; x<15; x++) {
			b.x = 3*i-t+c/2+x*s; b.y = 3*i+p;
			b.state = x;
			if (cpma & (040000 >> x)) { /* bit is one */
				b.boxGC = whiteGC;
			} else { /* bit is zero */
				b.boxGC = blackGC;
			}
			b.next = sublist;
			sublist = makebox(&b); /* one light box */
			cpmaboxes[x] = sublist;
		}
	}
	b.x = 3*i-t; b.y = 3*i+p;
	b.width = 10*i; b.height = c;
	b.rule = blankbox;
	b.next = boxlist; b.sub = sublist;
	boxlist = makebox(&b); /* bar of address lights */

	b.x = 15*i+p-t-s; b.y = 3*i;
	b.width = s; b.height = c;
	b.boxGC = goldGC;
	b.label = "RUN";
	b.rule = linefillbox;
	b.help = "the machine is halted if the light below is off";
	b.next = boxlist; b.sub = NOBOX;
	boxlist = makebox(&b); /* bar across the top of run light */

	b.x = 15*i+p-t-s+c/2; b.y = 3*i+p;
	b.width = c-t; b.height = c-t;
	b.label = NULL;
	b.rule = circlebox;
	if (run == 0) {
		b.boxGC = blackGC;
	} else {
		b.boxGC = whiteGC;
	}
	b.help = "the machine is halted if this light is off";
	b.next = boxlist; b.sub = NOBOX;
	boxlist = makebox(&b); /* run light */
	runbox = boxlist;

	{
		int data = getdata();
		sublist = NOBOX;
		b.width = c-t; b.height = c-t;
		b.rule = circlebox;
		b.press = backpress;
		b.help =
"these lights display the state, status, ac, md, mq or bus";
		for (x=0; x<12; x++) {
			b.x = 5*i-t+c/2+x*s; b.y = 4*i;
			b.state = x;
			if (data & (04000 >> x)) { /* bit is one */
				b.boxGC = whiteGC;
			} else { /* bit is zero */
				b.boxGC = blackGC;
			}
			b.next = sublist;
			sublist = makebox(&b); /* one light box */
			databoxes[x] = sublist;
		}
	}
	b.x = 5*i-t; b.y = 4*i;
	b.width = 12*s; b.height = c;
	b.rule = blankbox;
	b.next = boxlist; b.sub = sublist;
	boxlist = makebox(&b); /* bar of data lights */

	{
		struct box_rec* subsub;
		sublist = NOBOX;
		b.press = backpress;
		b.help =
"the above lights display the state, status, ac, md, mq or bus";
		for (x=0; x<12; x++) {
			b.x = 5*i+x*s-t; b.y = 4*i+c+t;
			b.width = s; b.height = c+c/2;
			b.boxGC = whiteGC;
			{
				static char* row[] = {
					"F","D","E",
					"IR0","IR1","IR2",
					"DIR","DATA","SW",
					"PAUSE","BRK","BRK"
				};
				b.label = row[x];
			}
			b.lablen = 0;
			b.rule = linebox;
			b.next = NOBOX; b.sub = NOBOX;
			subsub = makebox(&b); /* top caption */

			b.x = 5*i+x*s-t; b.y = 4*i+c+t+c+c/2;
			b.width = s; b.height = c;
			{
				static char* row[] = {
					"LINK","GT","INTB",
					"NOINT","ION","UM",
					"IF0","IF1","IF2",
					"DF0","DF1","DF2"
				};
				b.label = row[x];
			}
			b.rule = linebox;
			b.next = subsub; b.sub = NOBOX;
			subsub = makebox(&b); /* mid caption */

			b.x = 5*i+x*s-t ; b.y = 4*i+c+t;
			b.width = s; b.height = 2*i;
			b.label = "0 1 2 3 4 5 6 7 8 9 1011"+(2*x);
			b.lablen = 1+(x>9);
			b.rule = linefillbox;
			if (((x/3) & 1) == 0) {
				b.boxGC = terraGC;
			} else {
				b.boxGC = goldGC;
			}
			b.next = sublist; b.sub = subsub;
			sublist = makebox(&b); /* vertical bar under light */
		}

		b.x = 5*i-t+12*s; b.y = 4*i+c+t;
		b.width = 2*i+p; b.height = 2*i;
		b.boxGC = blackGC;
		b.label = NULL;
		b.rule = linefillbox;
		b.press = datapress;
		b.help =
"click the left button here to select data to display";
		b.state = -1;
		b.next = sublist; b.sub = NOBOX; /* to be filled in later! */
		sublist = makebox(&b); /* the data select switch */
		sublist->replot = sublist;

		subsub = NOBOX;
		b.width = 2*s; b.height = c;
		for (x=0; x<6; x++) {
			static char* helpmsg[] = {
"click the left button here for a display of some CPU state information",
"click the left button here for a display of the CPU status register",
"click the left button here for a display of the accumulator",
"click the left button here for a display of the memory data bus",
"click the left button here for a display of the multiplier quotient register",
"click the left button here for an approximate display of the data on the bus"
			};
			b.x = 5*i-t+12*s; b.y = 4*i+c+t+x*c;
			if (x == datastate) {
				b.rule = linebox;
			} else {
				b.rule = blankbox;
			}
			b.label = ("STATE STATUSAC    MD    MQ    BUS   ")+6*x;
			b.lablen = 6;
			b.labx = 5*i-t+12*s+c;
			b.boxGC = whiteGC;
			b.state = x;
			b.help = helpmsg[x];
			b.next = subsub; b.sub = NOBOX;
			subsub = makebox(&b); /* data select caption */
			subsub->replot = sublist;
			showdatabox[x] = subsub;
		}
		sublist->sub = subsub; /* fill in sub-boxes retrospectively */

	}
	b.x = 5*i-t; b.y = 4*i+c+t;
	b.width = 12*s+2*i+p; b.height = 2*i;
	b.label = NULL;
	b.lablen = 0;
	b.labx = 0;
	b.boxGC = whiteGC;
	b.rule = blankbox;
	b.press = backpress;
	b.help =
"the above lights display the state, status, ac, md, mq or bus";
	b.next = boxlist; b.sub = sublist;
	boxlist = makebox(&b); /* bar under data display lights */

	{
		b.x = i; b.y = 7*i;
		b.width = s; b.height = s+i;
		b.boxGC = terraGC;
		b.label = "OFF";
		b.laby = b.y+c+t;
		b.rule = linefillbox;
		b.press = powerpress;
		b.help = "click the left button here to exit the emulator";
		b.next = NOBOX; b.sub = NOBOX;
		sublist = makebox(&b); /* the off key position */

		b.x = i+s; b.y = 7*i;
		b.boxGC = goldGC;
		b.label = "POWER";
		b.next = sublist;
		sublist = makebox(&b); /* the power-on key position */

		b.x = i+2*s; b.y = 7*i;
		b.boxGC = terraGC;
		b.label = "LOCK";
		b.next = sublist;
		sublist = makebox(&b); /* the panel-lock key position */
	}
	b.x = i-t; b.y = 7*i;
	b.width = 3*s+2*t; b.height = s+i+t;
	b.boxGC = whiteGC;
	b.label = NULL;
	b.laby = 0;
	b.rule = linebox;
	b.next = boxlist; b.sub = sublist;
	boxlist = makebox(&b); /* power control enclosing box */

	{
		b.x = 4*i-p+t; b.y = 7*i;
		b.width = s; b.height = s;
		b.boxGC = goldGC;
		b.label = "SW";
		b.rule = linefillbox;
		b.press = backpress;
		b.help =
"the SW switch, below, controls certain system options";
		b.next = NOBOX; b.sub = NOBOX;
		sublist = makebox(&b); /* the sw overbar */

		b.x = 4*i-p+2*t; b.y = 7*i+s+t;
		b.width = s-2*t; b.height = i-2*t;
		b.boxGC = darkgoldGC;
		b.label = NULL;
		b.labGC = goldGC;
		b.rule = fillbox;
		b.press = swpress;
		b.help = "click the left button here to toggle the SW switch";
		b.next = sublist; b.sub = NOBOX;
		sublist = makeswitch( &b, sw );
	}
	b.x = 4*i-p; b.y = 7*i;
	b.width = s+2*t; b.height = s+i+t;
	b.boxGC = whiteGC;
	b.labGC = whiteGC;
	b.press = backpress;
	b.help = "the SW switch controls certain system options";
	b.rule = linebox;
	b.next = boxlist; b.sub = sublist;
	boxlist = makebox(&b); /* sw enclosing box */

	{
		b.x = 5*i-t; b.y = 7*i;
		b.width = 12*s; b.height = c;
		b.boxGC = terraGC;
		b.label = "SWITCH REGISTER";
		b.rule = linefillbox;
		b.press = backpress;
		b.help =
"the switch register, or SR (below) allows binary input from the front panel";
		b.next = NOBOX; b.sub = NOBOX;
		sublist = makebox(&b); /* the switch register overbar */

		b.width = s; b.height = c;
		b.press = backpress;
		b.label = NULL;
		for (x=0; x<12; x++) {
			b.x = 5*i+x*s-t ; b.y = 7*i+c;
			b.state = x;
			b.label = "0 1 2 3 4 5 6 7 8 9 1011"+(2*x);
			b.lablen = 1+(x>9);
			if (((x/3) & 1) == 0) {
				b.boxGC = terraGC;
			} else {
				b.boxGC = goldGC;
			}
			b.next = sublist;
			sublist = makebox(&b); /* switch overbar */
		}

		b.width = s-2*t; b.height = i-2*t;
		b.rule = fillbox;
		b.label = NULL;
		b.lablen = 0;
		b.press = srpress;
		b.help =
"click the left button here (or press F1 - F12) to toggle a bit of SR";
		for (x=0; x<12; x++) {
			b.x = 5*i+x*s; b.y = 7*i+s+t;
			b.state = x;
			if (((x/3) & 1) == 0) {
				b.boxGC = darkterraGC;
				b.labGC = terraGC;
			} else {
				b.boxGC = darkgoldGC;
				b.labGC = goldGC;
			}
			b.next = sublist;
			sublist = makeswitch(&b,(sr >> (11-x)) & 1);
			srboxes[x] = sublist;
		}
	}
	b.x = 5*i-2*t; b.y = 7*i;
	b.width = 12*s+2*t; b.height = s+i+t;
	b.boxGC = whiteGC;
	b.labGC = whiteGC;
	b.rule = linebox;
	b.press = backpress;
	b.help = "these switches are the switch register, or SR";
	b.next = boxlist; b.sub = sublist;
	boxlist = makebox(&b); /* switch register enclosing box */

	{
		b.x = 13*i+p; b.y = 7*i;
		b.width = s; b.height = s;
		b.boxGC = terraGC;
		b.label = "ADDR";
		b.rule = linefillbox;
		b.press = backpress;
		b.help = "the load address switch is below";
		b.next = NOBOX; b.sub = NOBOX;
		sublist = makebox(&b); /* the addr-load label */

		b.x = 13*i+p+t; b.y = 7*i+s+t;
		b.width = s-2*t; b.height = i-2*t;
		b.boxGC = darkterraGC;
		b.label = NULL;
		b.labGC = terraGC;
		b.rule = fillbox;
		b.press = addrloadpress;
		b.help =
"click the left button here to load the switch register to the program counter";
		b.next = sublist;
		sublist = makeswitch(&b, 1); /* the addr-load switch */

		b.x = 13*i+p+s; b.y = 7*i;
		b.width = s; b.height = s;
		b.boxGC = goldGC;
		b.label = "EXTD";
		b.labGC = whiteGC;
		b.rule = linefillbox;
		b.press = backpress;
		b.help = "the load extended memory address switch is below";
		b.next = sublist;
		sublist = makebox(&b); /* the extd addr load label */

		b.x = 13*i+p+t+s; b.y = 7*i+s+t;
		b.width = s-2*t; b.height = i-2*t;
		b.boxGC = darkgoldGC;
		b.label = NULL;
		b.labGC = goldGC;
		b.rule = fillbox;
		b.press = extdaddrloadpress;
		b.help =
"click the left button here to load SR into the IF and DF fields of the PSW";
		b.next = sublist;
		sublist = makeswitch(&b, 1); /* the extd-addr-load switch */
	}
	b.x = 13*i+p-t; b.y = 7*i;
	b.width = 2*s+2*t; b.height = s+i+t;
	b.boxGC = whiteGC;
	b.labGC = whiteGC;
	b.rule = linebox;
	b.press = backpress;
	b.help =
"these switches load address information from the switch register";
	b.next = boxlist; b.sub = sublist;
	boxlist = makebox(&b); /* address load enclosing box */

	{
		b.x = 15*i+p-t; b.y = 7*i;
		b.width = 2*s; b.height = c;
		b.boxGC = terraGC;
		b.label = "START";
		b.rule = linefillbox;
		b.help =
"the switches below are frequently used in starting the computer";
		b.next = NOBOX; b.sub = NOBOX;
		sublist = makebox(&b); /* caption over clear and cont */

		b.x = 15*i+p-t; b.y = 7*i+c;
		b.width = s; b.height = c;
		b.label = "CLR";
		b.help =
"the switch below clears all registers except the program counter";
		b.next = sublist;
		sublist = makebox(&b); /* caption over clear */

		b.x = 15*i+p+s-t; b.y = 7*i+c;
		b.boxGC = goldGC;
		b.label = "CONT";
		b.help =
"the switch below starts the computer running or continues from a halt";
		b.next = sublist;
		sublist = makebox(&b); /* caption over cont */

		b.x = 15*i+p+2*s-t; b.y = 7*i;
		b.width = s; b.height = s;
		b.boxGC = terraGC;
		b.label = "EXAM";
		b.help =
"the switch below reads from memory and then increments the address";
		b.next = sublist;
		sublist = makebox(&b); /* caption over exam */

		b.x = 15*i+p+3*s-t; b.y = 7*i;
		b.boxGC = goldGC;
		b.label = "HALT";
		b.help = "the switch below halts the machine";
		b.next = sublist;
		sublist = makebox(&b); /* caption over halt */

		b.x = 15*i+p+4*s-t; b.y = 7*i;
		b.boxGC = terraGC;
		b.label = "STEP";
		b.help =
"the switch below causes a single instruction (program step) to be executed";
		b.next = sublist;
		sublist = makebox(&b); /* caption over sing-step */

		b.x = 15*i+p; b.y = 7*i+s+t;
		b.width = s-2*t; b.height = i-2*t;
		b.boxGC = darkterraGC;
		b.label = NULL;
		b.labGC = terraGC;
		b.rule = fillbox;
		b.press = clearpress;
		b.help =
"click the left button here to clear all registers except the program counter";
		b.next = sublist;
		sublist = makeswitch(&b, 1); /* the clear switch */

		b.x = 15*i+p+s; b.y = 7*i+s+t;
		b.boxGC = darkgoldGC;
		b.labGC = goldGC;
		b.press = contpress;
		b.help =
"click the left button here to start the computer or continue from halt";
		b.next = sublist;
		sublist = makeswitch(&b, 1); /* the cont switch */

		b.x = 15*i+p+2*s; b.y = 7*i+s+t;
		b.boxGC = darkterraGC;
		b.labGC = terraGC;
		b.press = exampress;
		b.help =
"click the left button here to examine memory and then increment the address";
		b.next = sublist;
		sublist = makeswitch(&b, 1); /* the exam switch */

		b.x = 15*i+p+3*s; b.y = 7*i+s+t;
		b.boxGC = darkgoldGC;
		b.labGC = goldGC;
		b.press = haltpress;
		b.help = "click the left button here to halt the machine";
		b.next = sublist;
		sublist = makeswitch(&b, 1); /* the halt switch */

		b.x = 15*i+p+4*s; b.y = 7*i+s+t;
		b.boxGC = darkterraGC;
		b.labGC = terraGC;
		b.press = steppress;
		b.help =
"click the left button here to cause one instruction (program step) execute";
		b.next = sublist;
		sublist = makeswitch(&b, 1); /* the sing step switch */
	}
	b.x = 15*i+p-2*t; b.y = 7*i;
	b.width = 5*s+2*t; b.height = s+i+t;
	b.boxGC = whiteGC;
	b.labGC = whiteGC;
	b.rule = linebox;
	b.press = backpress;
	b.help = "these switches control program execution";
	b.next = boxlist; b.sub = sublist;
	boxlist = makebox(&b); /* start halt exam enclosing box */

	{
		b.x = 19*i+c; b.y = 7*i;
		b.width = s; b.height = s;
		b.boxGC = goldGC;
		b.label = "DEP";
		b.rule = linefillbox;
		b.press = backpress;
		b.help =
"the switch below deposits data from SR to memory then increments the address";
		b.next = NOBOX; b.sub = NOBOX;
		sublist = makebox(&b); /* label on dep box */

		b.x = 19*i+c+t; b.y = 7*i+s+t;
		b.width = s-2*t; b.height = i-2*t;
		b.boxGC = darkgoldGC;
		b.label = NULL;
		b.labGC = goldGC;
		b.rule = fillbox;
		b.press = deppress;
		b.help =
"click the left button here to load SR into memory then increment the address";
		b.next = sublist;
		sublist = makeswitch(&b, 0); /* the dep switch */
	}
	b.x = 19*i+c-t; b.y = 7*i;
	b.width = s+2*t; b.height = s+i+t;
	b.boxGC = whiteGC;
	b.labGC = whiteGC;
	b.rule = linebox;
	b.press = backpress;
	b.help =
"this switch deposits data from SR to memory and then increments the address";
	b.next = boxlist; b.sub = sublist;
	boxlist = makebox(&b); /* deposit enclosing box */

	{
		b.x = 2*h-s; b.y = h-i/2-s/2;
		b.width = s; b.height = s;
		b.boxGC = darkgreyGC;
		b.label = "OK";
		b.labGC = subGC;
		b.rule = linefillbox;
		b.labinfo = subinfo;
		b.press = helpokpress;
		b.help = "click the left button here to close the help window";
		b.next = NOBOX; b.sub = NOBOX;
		helpok = makebox(&b);
	}
	b.x = i-t; b.y = h-i+t;
	b.width = 2*h-i+2*t; b.height = i-2*t;
	b.boxGC = blackGC;
	b.rule = blankbox;
	b.label = NULL;
	b.labx = i-t+c;
	b.laby = h-i/2+t;
	b.labGC = helpGC;
	b.labinfo = helpinfo;
	b.press = backpress;
	b.help = "click the left button on the OK box to close this window";
	b.next = boxlist; b.sub = NOBOX;
	boxlist = makebox(&b); /* help message box (unused initially) */
	helpbox = boxlist;

	/* main box */

	b.x = 0; b.y = 0;
	b.width = 2*h+i; b.height = h;
	b.boxGC = whiteGC;
	b.rule = linebox;
	b.press = backpress;
	b.help = NULL;
	b.next = NOBOX; b.sub = boxlist;
	boxlist = makebox(&b); /* main box */
}
#undef h
#undef t
#undef i
#undef s


/********************************************/
/* Implementation of control panel function */
/********************************************/

static struct timer console_delay;
static struct timer halt_delay;

extern void (* ttybreak) (); /* hook to tty for keyboard overrun */

static handle_button_press(w, d, e)
Widget w;
caddr_t d;
XButtonPressedEvent *e;
{
	boxpress( e->x, e->y, e->button, boxlist );
}

static handle_button_release(w, d, e)
Widget w;
caddr_t d;
XButtonReleasedEvent *e;
{
	if (toggle != NOBOX) { /* handle undoing of button press */
		bounceinternal();
	}
}

static handle_key_press(w, d, e)
Widget w;
caddr_t d;
XKeyPressedEvent *e;
{
	int shift = (ShiftMask & e->state) != 0;
	int control = (ControlMask & e->state) != 0;
	char ch; /* the ASCII character */
	KeySym key = XKeycodeToKeysym( dpy, e->keycode, shift );

	/* first deal with keys that map to ASCII, then handle others */

	if ((key == XK_BackSpace)||(key == XK_Left)) {
		ch = '\b';
	} else if ((key == XK_Tab)||(key == XK_Right)) {
		ch = '\t';
	} else if ((key == XK_Linefeed)||(key == XK_Down)) {
		ch = '\n';
	} else if ((key == XK_Return)||(key == XK_Next)) {
		ch = '\r';
	} else if (key == XK_Up) {
		ch = '\v';
	} else if (key == XK_Escape) {
		ch = '[' & 037;
	} else if (key == XK_Delete) {
		ch = 0177;
	} else if ((key >= ' ') && (key <= '~')) {
		if (control) {
			ch = key & 037;
		} else {
			ch = key;
		}
	} else if ((key >= XK_F1) && (key <= XK_F12)) {  /* NOT ASCII */
		/* accept F1-F12 as toggles to SR0 to SR11 */
		srpress(srboxes[key - XK_F1]);
		return; /* ignore function keys after toggling SR */
	} else {
		return; /* ignore all other non-ascii chars */
	}

	if (keypress != NULL) {
		(* keypress)( ch );
		/* e->x and e->y are not used */
	} else {
		ttystuff( ch );
	}
}

static handle_exposure(w, d, e)
Widget w;
caddr_t d;
XExposeEvent *e;
{
	if (e->count == 0) { /* replot the whole thing */
		boxshow( boxlist );
	}
}

static void console_event()
/* all update of console display happens here! */
{
	changeregs(); /* always display any changes to registers ! */

	while (XtAppPending(app_context) || (run == 0)) { /* get console opes */
		XEvent event;
		XtAppNextEvent(app_context, &event );
		XtDispatchEvent( &event );
	}

	if (run == 2) { /* single step */
 		schedule( &console_delay, console_interval, console_event, 0 );
		countdown -= (console_interval - 1);
		run = 0;
	} else { /* normal long step */
 		schedule( &console_delay, console_interval, console_event, 0 );
	}
}

/*********************************/
/* Window manager initialization */
/*********************************/

startwindow(argc,argv)
int argc;
char **argv;
{
	Arg arg[25];
	XGCValues gcvalues;
	unsigned int n;

	/*
	 *  Set up the top level to house it all
	 */
        XtToolkitInitialize();
        app_context = XtCreateApplicationContext();
        dpy = XtOpenDisplay( app_context, NULL, NULL, "PDP8-E", NULL, 0,
                                                &argc, argv);

	scr = DefaultScreen(dpy);
	cmap = DefaultColormap(dpy, scr);

	/*
	 *  Get the fonts we need
	 */
	if ((headinfo = XLoadQueryFont( dpy,
				"-*-helvetica-bold-r-*-*-18-180-*-*-*-*-*-*" ))
	    == NULL) {
		if ((headinfo = XLoadQueryFont( dpy,
				"-*-*-*-r-*-*-16-*-*-*-*-*-*-*" ))
		    == NULL) {
			ttyrestore();exit(-1);
		}
	}

	if ((subinfo = XLoadQueryFont( dpy,
				"-*-helvetica-bold-r-*-*-10-100-*-*-*-*-*-*" ))
	    == NULL) {
		if ((subinfo = XLoadQueryFont( dpy,
				"-*-*-*-r-*-*-10-*-*-*-*-*-*-*" ))
		    == NULL) {
			ttyrestore();exit(-1);
		}
	}

	if ((tinyinfo = XLoadQueryFont( dpy,
				"-*-clean-medium-r-*-*-6-*-*-*-*-*-*-*" ))
	    == NULL) {
		if ((tinyinfo = XLoadQueryFont( dpy,
				"-*-*-*-r-*-*-6-*-*-*-*-*-*-*" ))
		    == NULL) {
			ttyrestore();exit(-1);
		}
	}

	if ((helpinfo = XLoadQueryFont( dpy,
				"-*-helvetica-medium-r-*-*-12-*-*-*-*-*-*-*" ))
	    == NULL) {
		if ((helpinfo = XLoadQueryFont( dpy,
				"-*-*-*-r-*-*-12-*-*-*-*-*-*-*" ))
		    == NULL) {
			ttyrestore();exit(-1);
		}
	}

	/*
	 * Create front-panel widget
	 */

	appshell = XtAppCreateShell( NULL, "PDP8-E",
				     applicationShellWidgetClass,
				     dpy, NULL, NULL );

	frontpanelshell = XtCreatePopupShell( "frontpanelshell",
					      topLevelShellWidgetClass,
					      appshell, arg, 0 );

	n = 0;
	XtSetArg(arg[n], XtNwidth, PDP8height * 21/10);          n++;
	XtSetArg(arg[n], XtNheight, PDP8height);                 n++;
	XtSetArg(arg[n], XtNbackground, BlackPixel( dpy, scr )); n++;
	frontpanel = XtCreateWidget( "panel", widgetClass,
				     frontpanelshell, arg, n);
	XtManageChild(frontpanel);
	XtPopup(frontpanelshell, XtGrabNonexclusive);

	XSetWindowColormap( dpy, XtWindow(frontpanel), cmap );

	/*
	 * Create black and white Graphics Contexts
	 */

	gcvalues.foreground = BlackPixel( dpy, scr );
	gcvalues.background = BlackPixel( dpy, scr );
	gcvalues.fill_style = FillOpaqueStippled;
	blackGC = XCreateGC(dpy, XtWindow(frontpanel),
			GCForeground | GCBackground,
			&gcvalues);

	gcvalues.foreground = WhitePixel( dpy, scr );
	gcvalues.font = tinyinfo->fid;
	whiteGC = XCreateGC(dpy, XtWindow(frontpanel),
			GCForeground | GCBackground | GCFont,
			&gcvalues);

	gcvalues.font = headinfo->fid;
	headGC = XCreateGC(dpy, XtWindow(frontpanel),
			GCForeground | GCBackground | GCFont,
			&gcvalues);

	gcvalues.font = subinfo->fid;
	subGC = XCreateGC(dpy, XtWindow(frontpanel),
			GCForeground | GCBackground | GCFont,
			&gcvalues);

	gcvalues.font = helpinfo->fid;
	helpGC = XCreateGC(dpy, XtWindow(frontpanel),
			GCForeground | GCBackground | GCFont,
			&gcvalues);
	
	/*
	 * Create monochrome Graphics Contexts
	 */

	grey.red = 128*256;
	grey.green = 128*256;
	grey.blue = 128*256;
	if (  XAllocColor( dpy, cmap, &grey )
	   && (grey.pixel != WhitePixel( dpy, scr ))
	   ) { /* we got the color we want! */
		gcvalues.foreground = grey.pixel;
		greyGC = XCreateGC(dpy, XtWindow(frontpanel),
				GCForeground | GCBackground,
				&gcvalues);
	} else { /* fake it as black */
		grey.pixel = BlackPixel( dpy, scr );
		greyGC = blackGC;
	}

	darkgrey.red = 80*256;
	darkgrey.green = 80*256;
	darkgrey.blue = 80*256;
	dark_bitmap = XCreateBitmapFromData( dpy, XtWindow(frontpanel),
					     dark_bits, 16, 16);
	if (   XAllocColor( dpy, cmap, &darkgrey )
	   && (darkgrey.pixel != grey.pixel)
	   ) { /* we got the color we want */
		gcvalues.foreground = darkgrey.pixel;
		darkgreyGC = XCreateGC(dpy, XtWindow(frontpanel),
				GCForeground | GCBackground,
				&gcvalues);
	} else if (greyGC != blackGC) { /* fake it as stippled grey */
		darkgrey.pixel = grey.pixel;
		gcvalues.foreground = darkgrey.pixel;
		gcvalues.stipple = dark_bitmap;
		darkgreyGC = XCreateGC(dpy, XtWindow(frontpanel),
				GCForeground | GCBackground |
				GCFillStyle | GCStipple,
				&gcvalues);
	} else { /* give up and use black */
		darkgrey.pixel = BlackPixel( dpy, scr );
		darkgreyGC = blackGC;
	}

	/*
	 * Create color Graphics Contexts
	 */

	goldenrod.red = 224*256;
	goldenrod.green = 160*256;
	goldenrod.blue = 32*256;
	gcvalues.stipple = dark_bitmap;
	if (XAllocColor( dpy, cmap, &goldenrod )) { /* we got what we want */
		gcvalues.foreground = goldenrod.pixel;
		goldGC = XCreateGC(dpy, XtWindow(frontpanel),
			GCForeground | GCBackground,
			&gcvalues);
		darkgoldGC = XCreateGC(dpy, XtWindow(frontpanel),
			GCForeground | GCBackground | GCFillStyle | GCStipple,
			&gcvalues);
	} else if (darkgrey.pixel != grey.pixel) { /* fake it with grey */
		goldenrod.pixel = grey.pixel;
		goldGC = greyGC;
		gcvalues.foreground = grey.pixel;
		darkgoldGC = XCreateGC(dpy, XtWindow(frontpanel),
			GCForeground | GCBackground | GCFillStyle | GCStipple,
			&gcvalues);
	} else { /* fake it with grey, but give up on gold/terra difference */
		goldenrod.pixel = grey.pixel;
		goldGC = greyGC;
		darkgoldGC = darkgreyGC;
	}

	terracotta.red = 208*256;
	terracotta.green = 112*256;
	terracotta.blue = 64*256;
	gcvalues.stipple = dark_bitmap;
	if (XAllocColor( dpy, cmap, &terracotta )) { /* we got what we want */
		gcvalues.foreground = terracotta.pixel;
		terraGC = XCreateGC(dpy, XtWindow(frontpanel),
			GCForeground | GCBackground,
			&gcvalues);
		darkterraGC = XCreateGC(dpy, XtWindow(frontpanel),
			GCForeground | GCBackground | GCFillStyle | GCStipple,
			&gcvalues);
	} else if (darkgrey.pixel != grey.pixel) { /* fake it with darkgrey */
		terracotta.pixel = darkgrey.pixel;
		terraGC = darkgreyGC;
		gcvalues.foreground = darkgrey.pixel;
		darkterraGC = XCreateGC(dpy, XtWindow(frontpanel),
			GCForeground | GCBackground | GCFillStyle | GCStipple,
			&gcvalues);
	} else { /* fake it with grey; give up on gold/terra difference */
		terracotta.pixel = grey.pixel;
		terraGC = greyGC;
		darkterraGC = darkgreyGC;
	}

	/*
	 * Setup to handle events
	 */

	XtAddEventHandler(frontpanel, ButtonPressMask, FALSE,
			  handle_button_press, NULL);
	XtAddEventHandler(frontpanel, ButtonReleaseMask, FALSE,
			  handle_button_release, NULL);
	XtAddEventHandler(frontpanel, LeaveWindowMask, FALSE,
			  handle_button_release, NULL);
	XtAddEventHandler(frontpanel, KeyPressMask, FALSE,
			  handle_key_press, NULL);
	XtAddEventHandler(frontpanel, ExposureMask, FALSE,
			  handle_exposure, NULL);

} 

kc8getinfo( d, s )
Display * *d;
int *s;
/* call this from powerup in devices that need to manipulate windows */
{
	*d = dpy;
	*s = scr;
}

Widget kc8makepopupshell( n )
char* n;
/* call this from powerup in any device that needs its own windows */
{
	Arg arg[2];
	return XtCreatePopupShell( n, topLevelShellWidgetClass,
				      appshell, arg, 0 );
}

/**********************************************************/
/* Interface between cpu implementation and control panel */
/**********************************************************/

static void hitbreak() /* called by keyboard server to get attention */
{
	/* do nothing!  this was useful on the dumb-terminal
	   version, but is not needed on a window-based system */
}

kc8power(argc,argv) /* power-on initialize */
unsigned int argc;
char **argv;
{
	init_timer( console_delay );
	schedule( &console_delay, console_interval, console_event, 0 );
	init_timer( heartbeat_delay );
	schedule( &heartbeat_delay, heartbeat_interval, heartbeat, 0 );
	init_timer( halt_delay );
	ttybreak = hitbreak;
	startwindow( argc, argv );
	makeboxes();
}

kc8init() /* console reset */
{
	/* nothing to do here */
}

kc8halt() /* respond to halt instruction */
{
	/* force console event! */
	schedule( &halt_delay, 0L, console_event, 0 );
}
