/****************************************************************************/
/*																			*/
/*	Application:	Snapshot												*/
/*																			*/
/*	Description:	This application demonstrates how to quickly and		*/
/*					efficiently capture the main device's desktop into		*/
/*					a window.  The program basically reads the image 		*/
/*					stored in the the main device's pixmap then copies		*/
/*					it to a custom pixmap.  The custom pixmap is de-		*/
/*					fined at the same depth of the main device and 			*/
/*					contains an identical copy of that device's color-		*/
/*					table.  This is done to provide the fastest 			*/
/*					performance possible when copying from an offscreen		*/
/*				 	to onscreen pixmap.  By making sure the pixel values	*/
/*				 	map to the exact same colors in both colortables,		*/
/*				 	copybits will do a direct transfer of bits without		*/
/*				 	wasting time remapping the colors.  Also the ctSeed		*/
/*				 	field for each colortable should be the same.  Finally,	*/
/*				 	since the main device's bounding rect is different		*/
/*				 	than that of the offscreen's, the copying performance	*/
/*				 	for the device to the offscreen is slightly affected	*/
/*				 	because of the scaling required.  However, the copying	*/
/*				 	performance for the offscreen to the window is the 		*/
/*				 	best possible since the bounding rects for each are		*/
/*				 	identical.												*/
/*																			*/
/*	File:			Snapshot.c												*/
/*																			*/
/*	Programmer:		Edgar Lee												*/
/*	Organization:	Apple Computer, Inc.									*/
/*	Department:		Developer Technical Support, DTS						*/
/*	Language:		C (Think C version 4.0.4)								*/
/*	Date Created:	10-04-91												*/
/*																			*/
/****************************************************************************/

#include <AppleEvents.h>
#include <Errors.h>
#include <Events.h>
#include <Fonts.h>
#include <Gestalt.h>
#include <Memory.h>
#include <Menus.h>
#include <Dialogs.h>
#include <OSUtils.h>
#include <QDOffscreen.h>
#include <QuickDraw.h>
#include <Resources.h>
#include <Script.h>
#include <ToolUtils.h>
#include <TextEdit.h>
#include <Windows.h>

/* Constant Declarations */

#define	WWIDTH		((screenBits.bounds.right - screenBits.bounds.left) / 2)
#define	WHEIGHT		((screenBits.bounds.bottom - screenBits.bounds.top) / 2)

#define WLEFT		(((screenBits.bounds.right - screenBits.bounds.left) - WWIDTH) / 2)
#define WTOP		(((screenBits.bounds.bottom - screenBits.bounds.top) - WHEIGHT) / 2)

/* Global Variable Definitions */

WindowPtr	gWindow;			/* Main window */
PixMap		gPixMap;			/* Offscreen pixmap */
Rect		gBounds;			/* Offscreen pixmap's bounding rect */

void initMac();

void createWindow();

void initPixmap();
void createImage();
void drawImage();

void doEventLoop();

int main(int argc,char **argv)
{
	initMac();
	
	createWindow();				/* Create a window to display the final image. */
	
	initPixmap();				/* Initialize offscreen pixmap. */
	createImage();				/* Copy the main screen's pixmap into the offscreen's. */
	drawImage();				/* Copy the offscreen's pixmap onto the window. */

	doEventLoop();				/* Handle any events. */
	
	DisposPixMap( &gPixMap );	/* Clean up the memory allocated for the pixmap. */
	DisposeWindow( gWindow );
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

void createWindow()
{
	Rect wBounds;
	
	/* Create a window to display the final offscreen image. */
	
	SetRect( &wBounds, WLEFT, WTOP, WLEFT + WWIDTH, WTOP + WHEIGHT );
	
	gWindow = NewCWindow( 0L, &wBounds, "\pSnapshot Test", false, documentProc,
							(WindowPtr)-1L, true, 0L );
							
	SetRect( &gBounds, 0, 0, gWindow->portRect.right - gWindow->portRect.left,
					gWindow->portRect.bottom - gWindow->portRect.top );
							
	SetPort( gWindow );
}

void initPixmap()
{
	Ptr			offBaseAddr;	/* Pointer to the off-screen pixel image */
	short		bytesPerRow;
	GDHandle	mainDevice;
	CTabHandle	cTable;
	short		depth;

	/* Get a handle to the main device. */
	mainDevice = GetMainDevice();

	/* Store its current pixel depth. */
	depth = (**(**mainDevice).gdPMap).pixelSize;

	/* Make an identical copy of its pixmap's colortable. */
	cTable = (**(**mainDevice).gdPMap).pmTable;
	HandToHand( &cTable );

	bytesPerRow = ((gBounds.right - gBounds.left) * depth) / 8;
	offBaseAddr = NewPtr((unsigned long)bytesPerRow * (gBounds.bottom - gBounds.top));
	
	gPixMap.baseAddr = offBaseAddr;  			/* Point to image */
	gPixMap.rowBytes = bytesPerRow | 0x8000;	/* MSB set for PixMap */
	gPixMap.bounds = gBounds;     				/* Use given bounds */
	gPixMap.pmVersion = 0;           			/* No special stuff */
	gPixMap.packType = 0;            			/* Default PICT pack */
	gPixMap.packSize = 0;            			/* Always zero in mem */
	gPixMap.hRes = 72;      					/* 72 DPI default res */
	gPixMap.vRes = 72;      					/* 72 DPI default res */
	gPixMap.pixelSize = depth;       			/* Set # bits/pixel */
	gPixMap.planeBytes = 0;          			/* Not used */
	gPixMap.pmReserved = 0;          			/* Not used */

	gPixMap.pixelType = 0;       				/* Indicates indexed */
	gPixMap.cmpCount = 1;        				/* Have 1 component */
	gPixMap.cmpSize = depth;     				/* Component size=depth */
	gPixMap.pmTable = cTable; 					/* Handle to CLUT */
}

void createImage()
{
	GDHandle	mainDevice;

	mainDevice = GetMainDevice();

	/* Store the screen's pixmap image in the offscreen pixmap. */

	CopyBits( *(**mainDevice).gdPMap, (BitMap *)(&gPixMap),
				&(**(**mainDevice).gdPMap).bounds, &gPixMap.bounds, srcCopy, 0l );
}

void drawImage()
{
	/* Copy the offscreen image back onto the window. */

	CopyBits( (BitMap *)&gPixMap, &gWindow->portBits, &gPixMap.bounds,
				&gWindow->portRect, srcCopy, 0l);
				
	ShowWindow( gWindow );
}

void doEventLoop()
{
	EventRecord anEvent;
	WindowPtr   evtWind;
	short       clickArea;
	Rect        screenRect;

	for (;;)
	{
		if (WaitNextEvent( everyEvent, &anEvent, 0, nil ))
		{
			if (anEvent.what == mouseDown)
			{
				clickArea = FindWindow( anEvent.where, &evtWind );
				
				if (clickArea == inDrag)
				{
					screenRect = (**GetGrayRgn ()).rgnBBox;
					DragWindow( evtWind, anEvent.where, &screenRect );
				}
				else if (clickArea == inContent)
				{
					if (evtWind != FrontWindow())
						SelectWindow( evtWind );
				}
				else if (clickArea == inGoAway)
					if (TrackGoAway( evtWind, anEvent.where ))
						return;
			}
			else if (anEvent.what == updateEvt)
			{
				evtWind = (WindowPtr)anEvent.message;	
				SetPort( evtWind );
				
				BeginUpdate( evtWind );
				drawImage();
				EndUpdate (evtWind);
			}
		}
	}
}