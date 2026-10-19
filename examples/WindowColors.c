/****************************************************************************/
/*																			*/
/*	Application:	WindowColors											*/
/*																			*/
/*	Description:	This application displays the default window colors		*/
/*					stored in the System's 'wctb' resource.  Note that		*/
/*					this app is 7.0 only, since it only recognizes the 7.0	*/
/*					version of the resource.								*/
/*																			*/
/*	Files:			WindowColors.¹											*/
/*					WindowColors.c											*/
/*																			*/
/*	Programmer:		Edgar Lee												*/
/*	Organization:	Apple Computer, Inc.									*/
/*	Department:		Developer Technical Support, DTS						*/
/*	Language:		C (Think C version 5.0.1)								*/
/*	Date Created:	02-19-92												*/
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

#define	WWIDTH		375
#define	WHEIGHT		155

#define WLEFT		(((screenBits.bounds.right - screenBits.bounds.left) - WWIDTH) / 2)
#define WTOP		(((screenBits.bounds.bottom - screenBits.bounds.top) - WHEIGHT) / 2)

/* Global Variable Definitions */

#define	TOTALWINCOLORS	13

CTabHandle	gWindowCTable;

Str255	gTitles[TOTALWINCOLORS] =
{
	"\pContent", "\pFrame", "\pText", "\pHilite",
	"\pTitleBar", "\pHiliteLight", "\pHiliteDark", "\pTitleBarLight",
	"\pTitleBarDark", "\pDialogLight", "\pDialogDark", "\pTingeLight",
	"\pTingeDark"
};


void initMac();
void createWindow();
void drawWindowColors();
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
	Rect		rect;
	WindowPtr	window;
	
	SetRect( &rect, WLEFT, WTOP, WLEFT + WWIDTH, WTOP + WHEIGHT );
	window = NewCWindow( 0L, &rect, "\pDefault Window Colors", true, documentProc,
							(WindowPtr)-1L, true, 0L );						
	SetPort( window );
	
	TextFont( geneva );
	TextSize( 9 );
	TextMode( srcCopy );
	
	gWindowCTable = (CTabHandle)NewHandle( sizeof( ColorTable )
					+ ((TOTALWINCOLORS - 1) * sizeof( ColorSpec )));
}

void drawWindowColors()
{
	int			i;
	int			col, row;
	Rect		rect;
		
	if ((gWindowCTable = (CTabHandle)GetResource( 'wctb', 0 )) != nil)
	{
		for (i = 0; i < TOTALWINCOLORS; i++)
		{
			col = 20 + ((i % 5) * 68);
			row = 25 + ((i / 5) * 45);
			
			SetRect( &rect, col, row, col + 60, row + 20 );
			RGBForeColor( &(**gWindowCTable).ctTable[i].rgb );
			PaintRect( &rect ); 
			
			InsetRect( &rect, -2, -2 );
			ForeColor( blackColor );
			FrameRect( &rect );
			
			MoveTo( rect.left, rect.top - 3 );
			DrawString( gTitles[i] );
		}
	}
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
				drawWindowColors();
				EndUpdate (evtWind);
			}
		}
	}
}