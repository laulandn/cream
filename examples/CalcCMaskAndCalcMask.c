/****************************************************************************/
/*																			*/
/*	Application:	CalcCMask & CalcMask									*/
/*																			*/
/*	Description:	This snippet shows how to use both CalcCMask or			*/
/*					CalcMask to create a mask given a source bitmap			*/
/*					image.  As decribed on pages IV-24 and V-72 of			*/
/*					Inside Mac, the two routines compute a destination		*/
/*					bitmap image with 1's only in the pixels where			*/
/*					paint can not leak from any of the outer edges.			*/
/*					This is similar to the lasso tool found in many			*/
/*					drawing apps.											*/
/*																			*/
/*	File:			CalcCMask & CalcMask.¹									*/
/*					CalcCMask & CalcMask.c									*/
/*					CalcCMask & CalcMask.¹.rsrc								*/
/*																			*/
/*	Programmer:		Edgar Lee												*/
/*	Organization:	Apple Computer, Inc.									*/
/*	Department:		Developer Technical Support, DTS						*/
/*	Language:		C (Think C version 5.0.1)								*/
/*	Date Created:	2-26-92													*/
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

#define	WWIDTH		176
#define	WHEIGHT		106

#define WLEFT		(((screenBits.bounds.right - screenBits.bounds.left) - WWIDTH) / 2)
#define WTOP		(((screenBits.bounds.bottom - screenBits.bounds.top) - WHEIGHT) / 2)

/* Global Variable Definitions */

WindowPtr	gWindow;

void initMac();
void createWindow();
void doCalcCMaskExample();

void DisposeGrafPort();
GrafPtr CreateGrafPort();

void doEventLoop();

int main(int argc,char **argv)
{
	initMac();
	
	createWindow();
	
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

void createWindow()
{
	Rect rect;
	
	SetRect( &rect, WLEFT, WTOP, WLEFT + WWIDTH, WTOP + WHEIGHT );
	
	gWindow = NewCWindow( 0L, &rect, "\pCalc-C-Mask", true, documentProc,
							(WindowPtr)-1L, true, 0L );
							
	SetPort( gWindow );
}

#define	COLOR_VERSION

void doCalcCMaskExample()
{
	PicHandle	pict;				/* B/W Pict used to create mask. */
	GWorldPtr	gworld;				/* Gworld used for CopyMask source. */
	GrafPtr     mask;				/* Mask created by CalcCMask. */
	GrafPtr		sourceForMask;		/* Bitmap of pict image used for creating mask. */
	RGBColor	seedColor;			/* Color used to determine the mask. */
	Rect		rect;				/* Bounding rect of mask and source. */
	CGrafPtr	currentPort;		/* Saved CGrafPtr for later restore. */
	GDHandle	currentDevice;		/* Saved device for later restore. */
	
	/* Load the pict resource to be used for the mask. */
	pict = (PicHandle)GetResource( 'PICT', 128 );
	
	/* Define the bounding rect for the source and mask bitmap. */
	rect = (**pict).picFrame;
	OffsetRect( &rect, -rect.left, -rect.top );
	
	/* Allocate the source bitmap for which the mask will be created from. */
	sourceForMask = CreateGrafPort( &rect );
	
	/* Create the source bitmap's image by using the pict. */
	SetPort( sourceForMask );
	DrawPicture( pict, &rect );
	SetPort( gWindow );
	
	/* Release the memory used by the pict. */
	ReleaseResource( pict );

	/* Create a gworld of a blue rectangle for the CopyMask source. */
	GetGWorld( &currentPort, &currentDevice );
	NewGWorld( &gworld, 8, &rect, nil, nil, 0 );
	SetPort( gworld );
	ForeColor( blueColor );
	PaintRect( &rect );
	SetGWorld( currentPort, currentDevice );
	
	/* Copy the source image to the window to see what the mask was created from. */
	CopyBits( &sourceForMask->portBits, &gWindow->portBits,
				&sourceForMask->portRect, &rect, srcCopy, nil );
	
	/* Allocate the bitmap for the mask. */
	mask = CreateGrafPort( &sourceForMask->portRect );

#ifdef COLOR_VERSION

	/* Create a mask from the source bitmap of all colors that match the seedColor. */
	seedColor.red = seedColor.green = seedColor.blue = 0;
	
	CalcCMask( &sourceForMask->portBits, &mask->portBits, &sourceForMask->portRect,
				&mask->portRect, &seedColor, nil, 0 );

#else

	CalcMask( &sourceForMask->portBits.baseAddr, &mask->portBits.baseAddr,
				sourceForMask->portBits.rowBytes, mask->portBits.rowBytes,
				sourceForMask->portRect.bottom - sourceForMask->portRect.top,
				sourceForMask->portBits.rowBytes >> 1 );

#endif

	/* Now draw the blue rectangle with its mask. */
	OffsetRect( &rect, sourceForMask->portRect.right, 0 );
	CopyMask( &(*gworld).portPixMap, &mask->portBits, &gWindow->portBits,
				&sourceForMask->portRect, &mask->portRect, &rect );

	/* Release the used memory. */
	DisposeGrafPort( sourceForMask );
	DisposeGrafPort( mask );
	DisposeGWorld( gworld );
}

GrafPtr CreateGrafPort( bounds )	/* Originally written by Forrest Tanaka. */
Rect *bounds;
{
	GrafPtr	savedPort;		/* Saved GrafPtr for later restore. */
	GrafPtr	newPort;		/* New GrafPort. */
	Rect	localBounds;	/* Local copy of bounds. */

	GetPort( &savedPort );

	/* Set the top-left corner of bounds to (0,0). */
	localBounds = *bounds;
	OffsetRect( &localBounds, -bounds->left, -bounds->top );

	/* Allocate a new GrafPort. */
	newPort = (GrafPtr)NewPtrClear( sizeof( GrafPort ) );
	
	if (newPort != nil)
	{
		/* Initialize the new port and make the current port. */
		OpenPort( newPort );

		/* Initialize and allocate the bitmap. */
		newPort->portBits.bounds = localBounds;
  		newPort->portBits.rowBytes = ((localBounds.right + 15) >> 4) << 1;
		newPort->portBits.baseAddr =  NewPtrClear( newPort->portBits.rowBytes *
													(long)localBounds.bottom );
		if (newPort->portBits.baseAddr != nil)
		{
			/* Clean up the new port. */
			newPort->portRect = localBounds;
			ClipRect( &localBounds );
			RectRgn( newPort->visRgn, &localBounds );
			EraseRect( &localBounds );
		}
		else
		{
			/* Allocation failed; deallocate the port. */
			ClosePort( newPort );
			DisposPtr( (Ptr)newPort );
			newPort = nil;
		}
	}
	
	SetPort( savedPort );
	return newPort;
}

void DisposeGrafPort( doomedPort )		/* Originally written by Forrest Tanaka. */
GrafPtr doomedPort;
{
	ClosePort( doomedPort );
	DisposPtr( doomedPort->portBits.baseAddr );
	DisposPtr( (Ptr)doomedPort );
}

void doEventLoop()
{
	EventRecord event;
	WindowPtr   window;
	short       clickArea;
	Rect        screenRect;

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
				}
				else if (clickArea == inGoAway)
					if (TrackGoAway( window, event.where ))
						return;
			}
			else if (event.what == updateEvt)
			{
				window = (WindowPtr)event.message;	
				SetPort( window );
				
				BeginUpdate( window );
				doCalcCMaskExample();
				EndUpdate( window );
			}
		}
	}
}