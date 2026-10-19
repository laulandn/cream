/****************************************************************************/
/*																			*/
/*	Application:	Band Copying the Sequel									*/
/*																			*/
/*	File:			Band Copying the Sequel.¹								*/
/*					Band Copying the Sequel.c								*/
/*					Band Copying the Sequel.¹.rsrc							*/
/*																			*/
/*	Description:	Under low memory conditions, it's often necessary		*/
/*					to draw an image by individual bands rather than		*/
/*					to draw the entire image at once, and in most cases		*/
/*					this method works fine unless the image requires		*/
/*					dithering from its source to its destination.  The		*/
/*					purpose of this app is to show the problem that may		*/
/*					occur when copying these individual bands and to		*/
/*					provide one possible solution.							*/
/*																			*/
/*	Programmer:		Edgar Lee												*/
/*	Organization:	Apple Computer, Inc.									*/
/*	Department:		Developer Technical Support, DTS						*/
/*	Language:		C (Think C version 5.0.1)								*/
/*	Date Created:	11-03-91												*/
/*																			*/
/****************************************************************************/

#include 	<Gestalt.h>
#include	<Palettes.h>
#include	<Dialogs.h>
#include	<ToolUtils.h>
#include	<Devices.h>
#include	<Processes.h>
#include	<Fonts.h>
#include	<Resources.h>
#include	<QDOffscreen.h>


extern void ExitToShell();


/* Constant Declarations */

#define	WWIDTH		300		/* Window width for the color and b/w images. */
#define	WHEIGHT		300		/* Window height for the color and b/w images. */

#define WLEFT		(((screenBits.bounds.right - screenBits.bounds.left) - WWIDTH) / 2)
#define WTOP		(((screenBits.bounds.bottom - screenBits.bounds.top) - WHEIGHT) / 2)

#define TOTALBANDS	25		/* Total number of bands to separate Pict into. */
#define	PADHEIGHT	8		/* Padding to add above band in pixels. */
#define	SCALING		4		/* Scale source by factor of... */

/* Global Variable Definitions */

WindowPtr		gCWindow;
WindowPtr		gWindow;

GWorldPtr		gGWorld;
PixMapHandle	gPixMap;

PicHandle		gPict;				/* Pict resource used for test. */
int				gPictWidth;			/* Width of pict resource. */
int				gPictHeight;		/* Height of pict resource. */
int				gBandStart;			/* Starting band number to draw. */
int				gBandEnd;			/* Ending band number to draw. */
int 			gBandHeight;		/* Thickness of band in pixels. */
int				gPadding = 0;		/* Current value of padding. */

void initMac();			/* Initializes the environment. */
void initPictInfo();	/* Initializes the picture information. */

void createWindows();	/* Creates windows for color and b/w images. */
void createGWorld();	/* Creates 1 bit offscreen GWorld used for dithering. */
void drawCWindow();		/* Displays the original color image. */
void drawBwWindow();	/* Displays the dithered/enlarged b/w image. */
void doBandCopy();		/* Copies image by individual bands. */
void doNoBandCopy();	/* Copies the entire image at once without bands. */
void drawMessage();		/* Draws text in color window. */

void doEventLoop();		/* Handles all events. */

int main(int argc,char **argv)
{
	initMac();
	initPictInfo();

	createWindows();
	createGWorld();
	
	doEventLoop();
}

void initMac()
{
	MaxApplZone();
	
	InitGraf( &thePort );
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs( nil );
	InitCursor();
	FlushEvents( 0, everyEvent );	
}

void initPictInfo()
{
	gPict = (PicHandle)GetResource( 'PICT', 128 );

	gPictWidth = (**gPict).picFrame.right - (**gPict).picFrame.left;
	gPictHeight = (**gPict).picFrame.bottom - (**gPict).picFrame.top;
	
	gBandStart = 0;								/* Start at band number 0. */
	gBandEnd = 0;								/* End at band number 0. */
	gBandHeight = gPictHeight / TOTALBANDS;
}

void createWindows()
{
	Rect rect;
	
	SetRect( &rect, WLEFT, WTOP, WLEFT + WWIDTH, WTOP + WHEIGHT );
	
	OffsetRect( &rect, -((WWIDTH + 10) / 2), 0 );
	gCWindow = NewCWindow( 0L, &rect, "\pBand Copying the Sequel", true, documentProc,
							(WindowPtr)-1L, false, 0L );
	
	OffsetRect( &rect, WWIDTH + 10, 0 );
	gWindow = NewCWindow( 0L, &rect, "\pBandCopying ON - Padding OFF", true, documentProc,
							(WindowPtr)-1L, true, 0L );
							
	SetPort( gCWindow );
}

void createGWorld()
{
	int		i;
	Rect	rect;
	
	/* Create a 1bit offscreen gWorld to be used for B/W dithering. */

	NewGWorld( &gGWorld, 1, &gWindow->portRect, nil, nil, 0 );
	gPixMap = GetGWorldPixMap( gGWorld );
}

void drawCWindow()
{
	Rect	rect;
	int		row;
	
	/* Reposition the picture's rect then draw it into the first window. */
	
	SetPort( gCWindow );
	
	rect.left = (WWIDTH - gPictWidth) / 2;
	rect.top = (WHEIGHT - gPictHeight) / 2;
	rect.right = rect.left + gPictWidth;
	rect.bottom = rect.top + gPictHeight;
	
	DrawPicture( gPict, &rect );
	
	TextFont( helvetica );
	TextSize( 12 );
	
	row = 20;	
	drawMessage( &row, "\p¥ Click the mouse button in the" );
	drawMessage( &row, "\p   b/w window to draw another band." );
	drawMessage( &row, "\p¥ Press any key other than ESC to toggle between" );
	drawMessage( &row, "\p   using band copying with or without padding." );
	
	row = 200;	
	drawMessage( &row, "\p¥ Press ESC to toggle between using copybits" );
	drawMessage( &row, "\p   with or without bands." );
}

void drawMessage( row, string )
int		*row;
Str255	string;
{
	MoveTo( 10, *row += 15 );
	DrawString(  string );
}

void drawBwWindow()
{
	Rect rect;
	
	if (gPadding >= 0)
	{
		SetRect( &rect, 0, 0, gPictWidth * SCALING, (gBandHeight + gPadding) * SCALING );
		UpdateGWorld( &gGWorld, 1, &rect, nil, nil, stretchPix );
		gPixMap = GetGWorldPixMap( gGWorld );
		doBandCopy();
	}
	else
	{
		UpdateGWorld( &gGWorld, 1, &gWindow->portRect, nil, nil, stretchPix );
		gPixMap = GetGWorldPixMap( gGWorld );
		doNoBandCopy();
	}
}

void doBandCopy()
{
	int			i;
	int			hoffset, voffset;	/* Starting position of picture in b/w window. */
	Rect		colorRect;			/* Source rect for the color image band. */
	Rect		gworldRect;			/* Source/destinaton rect for the gworld image band. */
	Rect		bwRect;				/* Destination rect for the b/w image band. */
	CGrafPtr	currentPort;		/* CGrafPort of window. */
	GDHandle	currentGDevice;		/* GDevice used by window. */
	Str255		string;
		
	hoffset = (WWIDTH - gPictWidth) / 2;
	voffset = (WHEIGHT - gPictHeight) / 2;

	SetPort( gWindow );
	TextFont( helvetica );
	TextSize( 9 );

	GetGWorld( &currentPort, &currentGDevice );

	for (i = gBandStart; i <= gBandEnd; i++)
	{
		SetGWorld( gGWorld, nil );

		/* Define the source band rect for the color image, */
		/*	INCLUDING any padding if applicable.			*/
		colorRect.left = hoffset;
		colorRect.right = hoffset + gPictWidth;
		colorRect.top = voffset + (i * gBandHeight) - gPadding;
		colorRect.bottom = voffset + (i * gBandHeight) + gBandHeight;
		
		/* Define the destination band rect for the gworld dithered image, */
		/*	allow extra space for padding if necessary.					   */
		SetRect( &gworldRect, 0, 0, gPictWidth * SCALING,
							(gBandHeight + gPadding) * SCALING );
	
		/* Copy the color source into the B/W gworld using dithering. */
		/*  Also, be sure to copy any padded area as well.			  */
		CopyBits( &gCWindow->portBits, (BitMap*)*gPixMap, &colorRect, &gworldRect, ditherCopy, 0l);
		
		SetGWorld( currentPort, currentGDevice );
		
		/* Now, set the source band rect to the gworld image's EXCLUDING any padding. */
		/*  If padding was used, offset pass the padding and down to the band.        */
		SetRect( &gworldRect, 0, gPadding * SCALING, gPictWidth * SCALING,
								(gPadding + gBandHeight) * SCALING );
		
		/* Define the destination band rect for the final b/w image. */
		/*  No padding should apply here!!							 */
		SetRect( &bwRect, hoffset / SCALING,
						(voffset / SCALING) + ((i * gBandHeight) * SCALING),
						(hoffset / SCALING) + (gPictWidth * SCALING),
						(voffset / SCALING) + (((i * gBandHeight) + gBandHeight) * SCALING) );

		/* Finally, copy the offscreen image into the B/W window. */
		CopyBits( (BitMap*)*gPixMap, &gWindow->portBits, &gworldRect, &bwRect, srcCopy, 0l);
	
		SetPort( gWindow );
		MoveTo( bwRect.right + 3, bwRect.bottom );
		NumToString( i, &string );
		DrawString( string );
	}
}

void doNoBandCopy()
{
	Rect		colorRect;			/* Source rect for the color image band. */
	Rect		gworldRect;			/* Source/destinaton rect for the gworld image band. */
	Rect		bwRect;				/* Destination rect for the b/w image band. */
	int			hoffset, voffset;	/* Starting position of picture in b/w window. */
	CGrafPtr	currentPort;		/* CGrafPort of window. */
	GDHandle	currentGDevice;		/* GDevice used by window. */
	
	GetGWorld( &currentPort, &currentGDevice );
	
	hoffset = (WWIDTH - gPictWidth) / 2;
	voffset = (WHEIGHT - gPictHeight) / 2;

	colorRect.left = hoffset;
	colorRect.top = voffset;
	colorRect.right = hoffset + gPictWidth;
	colorRect.bottom = voffset + gPictHeight;

	SetRect( &bwRect, hoffset / SCALING,
					voffset / SCALING,
					(hoffset / SCALING) + (gPictWidth * SCALING),
					(voffset / SCALING) + (gPictHeight * SCALING) );
	
	SetGWorld( gGWorld, nil );
	CopyBits( &gCWindow->portBits, (BitMap*)*gPixMap, &colorRect, &bwRect, ditherCopy, 0l);
	SetGWorld( currentPort, currentGDevice );
	CopyBits( (BitMap*)*gPixMap, &gWindow->portBits, &bwRect, &bwRect, srcCopy, 0l);
}

void doEventLoop()
{
	EventRecord event;
	WindowPtr   window;
	short       clickArea;
	Rect        screenRect;
	static int	oldPadding;
	
	for (;;)
	{
		if (WaitNextEvent( everyEvent, &event, 0, nil ))
		{
			if (event.what == mouseDown)
			{
				clickArea = FindWindow( event.where, &window );
				
				if (clickArea == inDrag)
				{
					screenRect = (**GetGrayRgn()).rgnBBox;
					DragWindow( window, event.where, &screenRect );
				}
				else if (clickArea == inContent)
				{
					if (window != FrontWindow())
						SelectWindow( window );
					else
					{
						if (gPadding >= 0)
						{	
							gBandEnd++;
							
							if (gBandEnd > TOTALBANDS)
							{
								gBandEnd = 0;
								SetPort( gWindow );
								EraseRect( &gWindow->portRect );
							}
							
							gBandStart = gBandEnd;
							drawBwWindow();
						}
					}
				}
				else if (clickArea == inGoAway)
					if (TrackGoAway( window, event.where ))
						return;
			}
			else if (event.what == keyDown || event.what == autoKey)
			{
				if ((event.message & charCodeMask) == 0x1b)
				{
					SetPort( gWindow );
					EraseRect( &gWindow->portRect );
					
					if (gPadding >= 0)
						gPadding = -1;
					else
						gPadding = oldPadding;
				}
				
				if (gPadding == 0)
				{
					gPadding = PADHEIGHT;
					SetWTitle( gWindow, "\pBandCopying ON - Padding ON" );
					oldPadding = 0;
				}
				else if (gPadding > 0)
				{
					gPadding = 0;
					SetWTitle( gWindow, "\pBandCopying ON - Padding OFF" );
					oldPadding = PADHEIGHT;
				}
				else
					SetWTitle( gWindow, "\pBandCopying OFF" );
					
				gBandStart = 0;
				drawBwWindow();
			}
			
			else if (event.what == updateEvt)
			{
				window = (WindowPtr)event.message;	
				SetPort( window );
				
				BeginUpdate( window );
				
				gBandStart = 0;
				drawCWindow();
				drawBwWindow();
				
				EndUpdate( window );
			}
		}
	}
}