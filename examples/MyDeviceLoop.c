/****************************************************************************/
/*																			*/
/*	Application:	MyDeviceLoop											*/
/*																			*/
/*	Description:	This snippet shows how to write a device loop that		*/
/*					works under System 7 and pre-7.0 systems.  As			*/
/*					described on pages 21-23 and 21-24 of Inside Mac		*/
/*					volume VI, a device loop procedure searches all			*/
/*					active screen devices, calling a drawing procedure		*/
/*					whenever it encounters a screen that intersects			*/
/*					the drawing region.  In this app the drawing			*/
/*					region is the app's window bounds and the chosen		*/
/*					drawing procedure simply displays the screen's			*/
/*					colors for every device the window bounds intersect.	*/
/*																			*/
/*	Files:			MyDeviceLoop.¹											*/
/*					MyDeviceLoop.c											*/
/*																			*/
/*	Programmer:		Edgar Lee												*/
/*	Organization:	Apple Computer, Inc.									*/
/*	Department:		Developer Technical Support, DTS						*/
/*	Language:		C (Think C version 5.0.1)								*/
/*	Date Created:	02-21-92												*/
/*																			*/
/****************************************************************************/

/* Constant Declarations */

#define	WWIDTH		400
#define	WHEIGHT		256

#define WLEFT		(((screenBits.bounds.right - screenBits.bounds.left) - WWIDTH) / 2)
#define WTOP		(((screenBits.bounds.bottom - screenBits.bounds.top) - WHEIGHT) / 2)

/* Global Variable Definitions */

WindowPtr	gWindow;

void initMac();
void createWindow();
void doMyDeviceLoop();
void doDraw();
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
	gWindow = NewCWindow( 0L, &rect, "\pMyDeviceLoop", true, documentProc,
							(WindowPtr)-1L, true, 0L );						
	SetPort( gWindow );
	
	TextFont( times );
	TextSize( 48 );
	TextMode( srcCopy );
}

void doMyDeviceLoop()
{
	int			depth;
	Rect		gDeviceRect;
	Rect		intersectingRect;
	GDHandle	gDevice;
	Point		point;
	
	/* Get the handle to the first device in the list. */
	gDevice = GetDeviceList();
	
	/* Loop through all the devices in the list. */
	while (gDevice != nil)
	{
		/* Get the device's gdRect and convert it to local coordinates. */
		gDeviceRect = (**gDevice).gdRect;
		depth = (**(**gDevice).gdPMap).pixelSize;
			
		GlobalToLocal( &topLeft( gDeviceRect ) );
		GlobalToLocal( &botRight( gDeviceRect ) );
		
		/* Check if the app's window rect intersects the device's, and if it */
		/*	does, set the clip region's rect to the intersection, then DRAW! */
		
		if (SectRect( &gWindow->portRect, &gDeviceRect, &intersectingRect ))
		{
			ClipRect( &intersectingRect );
			doDraw( depth, &intersectingRect );
		}
		
		/* Get the next device in the list. */
		gDevice = GetNextDevice( gDevice );
	}
}

void doDraw( depth, rect )
int		depth;
Rect	*rect;
{
	int				i;
	int				totalColors;
	int				penThickness;
	RGBColor		color;
	CTabHandle		ctable;
	Str255			string = "\pDirect Colors";
	
	if (depth > 8)
	{
		BackColor( blackColor );
		ForeColor( blueColor );
		
		/* Draw text for direct colors mode. */
		EraseRect( &gWindow->portRect );
		MoveTo( (gWindow->portRect.right - StringWidth( string )) / 2, 130 );
		DrawString( string );
	}
	else
	{
		/* Get the colortable at this depth. */
		ctable = GetCTable( depth );
		
		/* Set the line thickness to a fraction of the window height. */
		totalColors = (2 << depth) / 2;
		penThickness = gWindow->portRect.bottom / totalColors;
		PenSize( 1, penThickness );
	
		/* Now draw the colors at this depth. */
		for (i = 0; i < totalColors; i++)
		{
			RGBForeColor( &(**ctable).ctTable[i].rgb );
			MoveTo( 0, i * penThickness );
			LineTo( gWindow->portRect.right, i * penThickness );
		}
		
		/* Release the colortable memory. */
		DisposCTable( ctable );
	}
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
				doMyDeviceLoop();
				EndUpdate( window );
			}
		}
	}
}