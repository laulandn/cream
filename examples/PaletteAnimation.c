/*______________________________________________________*/
/*					   Palette Demo						*/
/*                          by                          */
/*                  RICHARD P. COLLYER                  */
/*              Developer Technical Support             */
/*                 Apple Computer, Inc.                 */
/*                       08/15/90                       */
/*______________________________________________________*/

#include	<Quickdraw.h>
#include	<Windows.h>
#include	<Events.h>
#include 	<Gestalt.h>
#include	<Events.h>
#include	<Palettes.h>
#include	<SegLoad.h>
#include	<Packages.h>
#include <NumberFormatting.h>

extern _DataInit();

#define	TRUE			0xFF
#define	FALSE			0
#define Gestalttest		0xA1AD
#define NoTrap			0xA89F

#define	clutID			150
#define	numcolor		256

Rect				WinMinusScroll;
WindowPtr			myWindow;
CTabHandle			mycolors;
PaletteHandle		srcPalette;
Boolean				DoneFlag;

/*______________________________________________________*/
/*             Set Up Usage of Palette                  */
/*______________________________________________________*/
void Draw()
/* The drawing routine is a collection of 253 random Rects which are colored with 
the corresponding color in the color table. */
{
	float					left, top, right, bottom;
	Rect					CRect;
	int						i;

	EraseRect (&WinMinusScroll);
	for (i = 1; i < numcolor-2; ++i) {
		do {
			left = Random() / 32767.0 * WinMinusScroll.right;
			if (left < 0)
				left = -left;
			} while (left > WinMinusScroll.right - 40);
			
		do {
			right = Random() / 32767.0 * WinMinusScroll.right;
			if (right < 0)
				right = -right;
			} while (right < left);
			
		do {
			top = Random() / 32767.0 * WinMinusScroll.bottom;
			if (top < 0)
				top = -top;
			} while (top > WinMinusScroll.bottom - 40);
			
		do {
			bottom = Random() / 32767.0 * WinMinusScroll.bottom;
			if (bottom < 0)
				bottom = -bottom;
			} while (bottom < top);
		SetRect(&CRect, left, top, right, bottom);
		PmForeColor(i);
		FrameRect (&CRect);
		PaintRect (&CRect);
		}
}

/*______________________________________________________*/
/*               Initialization traps                   */
/*______________________________________________________*/
void init()
{
	Rect				BaseRect;
	OSErr				err;
	long				feature;

	UnloadSeg(_DataInit);
	InitGraf(&qd.thePort);
	FlushEvents(everyEvent, 0);
	InitWindows();
	InitCursor();
	
	DoneFlag = FALSE;
/*______________________________________________________*/
/*            Use Gestalt to find 32BQD                 */
/*______________________________________________________*/
if ((GetTrapAddress(Gestalttest) != GetTrapAddress(NoTrap))) {
	err = Gestalt(gestaltQuickdrawVersion, &feature);
	if (!err) {
		if ((feature & 0x0f00) != 0x0200)
			DoneFlag = TRUE;
		}
	else 
		DoneFlag = TRUE;
	}
else
	DoneFlag = TRUE;
/*______________________________________________________*/
/*                     Set Rects                        */
/*______________________________________________________*/
	SetRect(&BaseRect, 40, 60, 472, 282);
	SetRect(&WinMinusScroll, BaseRect.left-40, BaseRect.top-60, BaseRect.right-60, 
				BaseRect.bottom - 80);
/*______________________________________________________*/
/*        Open Window & set Palette & Picture           */
/*______________________________________________________*/
	mycolors = GetCTable (clutID);

	myWindow = NewCWindow(nil, &BaseRect, "\pUsing Palette Manager", TRUE, documentProc, 
							(WindowPtr) -1, TRUE, 150);
	SetPort((WindowPtr) myWindow);

	srcPalette = NewPalette (numcolor, mycolors, pmAnimated, 0x1500);
	SetPalette ((WindowPtr) myWindow, srcPalette, TRUE);
	
	Draw();
}

int main(int argc,char **argv)
{
	EventRecord 	myEvent;
	int				yieldTime = 0, Counter;
	RGBColor		changecolor, black = {0,0,0};
	long			startTicks, finalTicks;
	char			*countsstrPtr, countstr;
/*______________________________________________________*/
/*                   Main Event loop                    */
/*______________________________________________________*/
	init();
	countsstrPtr = &countstr;	
	startTicks = TickCount();
	Counter = 0;
	for ( ;; ) {
		if (DoneFlag) {
			DoneFlag = FALSE;
			RGBForeColor (&black);
			NumToString(finalTicks,countsstrPtr);
			MoveTo (20,20);
			DrawString ("\pThe Number of Tick = ");
			MoveTo (150,20);
			DrawString (countsstrPtr);
			for (;;) {
				if (WaitNextEvent(everyEvent, &myEvent, yieldTime, nil)) {
					switch (myEvent.what) {
						case mouseDown:
						case keyDown:
						case autoKey:
							DoneFlag = TRUE;
							break;
						default:
							break;
						}
					}
				if (DoneFlag)
					ExitToShell();
				}
			}
			
		/* Animate the colors one step for eash event loop */
		GetEntryColor (srcPalette, 1, &changecolor);
		AnimatePalette (myWindow, mycolors, 2, 1, numcolor - 2);
		AnimateEntry (myWindow, numcolor - 1, &changecolor);
		Palette2CTab (srcPalette, mycolors);
		
		if (++Counter == 1000) {
			finalTicks = TickCount() - startTicks;
			DoneFlag = TRUE;
			}
		}
}