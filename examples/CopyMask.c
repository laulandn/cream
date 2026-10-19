/*--------------------------------------------------------------------------*/
/*																			*/
/*		CopyMask.															*/
/*			by John Wang													*/
/*																			*/
/*		Description:	Shows how CopyMask can used to fade a screen to a	*/
/*			lighter color.													*/
/*																			*/
/*		Version:		1.0 Completed 12/2/91								*/
/*																			*/
/*--------------------------------------------------------------------------*/

#include 	<Gestalt.h>
#include	<Palettes.h>
#include	<Dialogs.h>
#include	<ToolUtils.h>
#include	<Devices.h>
#include	<Processes.h>
#include	<Fonts.h>
#include	<QDOffscreen.h>


extern void ExitToShell();


int main(int argc, char **argv)
{
	WindowPtr		mainWinPtr;
	OSErr			error;
	SysEnvRec		theWorld;
	GWorldPtr		myOffscreen1, myOffscreen2;
	GDHandle		oldDevice;
	CGrafPtr		oldPort;
	Rect			windRect, offRect, myRect;
	RGBColor		theColor;
	int				theDepth, i;
	
	/* Make sure ColorQD exists. */
	error = SysEnvirons(1, &theWorld);
	if (theWorld.hasColorQD == false) {
		SysBeep(50);
		ExitToShell();
		return 0;
	}
	
	/* Initialize all the needed managers. */
	InitGraf(&qd.thePort);
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(nil);
	InitCursor();

	/* Define output window. */
	SetRect(&windRect, 100, 100, 400, 400);
	SetRect(&offRect, 0, 0, 300, 300);
	mainWinPtr = NewCWindow(nil, &windRect, "\pJohn", true, documentProc, (WindowPtr) -1, false, 0);
	SetPort(mainWinPtr);
	GetGWorld(&oldPort, &oldDevice);

	/* Create two offscreen GWorlds. */
	if (NewGWorld(&myOffscreen1, 0, &offRect, nil, nil, 0))
		Debugger();
	if (NewGWorld(&myOffscreen2, 0, &offRect, nil, nil, 0))
		Debugger();
	
	/* Now, draw ramp to first offscreen pixmap. */
	SetGWorld(myOffscreen1, nil);
	for (i=0; i<10; i++) {
		theColor.red = theColor.green = theColor.blue = (i*28) << 8;
		RGBForeColor(&theColor);
		SetRect(&myRect, 0, i*30, 300, i*30+30);
		PaintRect(&myRect);
	}

	/* Now, draw mask to second offscreen pixmap. */
	SetGWorld(myOffscreen2, nil);
	SetRect(&myRect, 0, 0, 300, 300);
	theColor.red = theColor.green = theColor.blue = (128) << 8;
	RGBForeColor(&theColor);
	PaintRect(&myRect);

	/*	Draw to the ramp to the window.	*/
	SetGWorld(oldPort, oldDevice);
	CopyBits((BitMapPtr) *myOffscreen1->portPixMap, &(*mainWinPtr).portBits, &offRect,
				&offRect, 0, nil);

	/* Wait until user clicks button. */
	do {
	} while (!Button());
	
	/*	Now fade the window to white using CopyMask.  The destination must be erased
		for CopyMask to work.	*/
	SetGWorld(oldPort, oldDevice);
	EraseRect(&offRect);
	CopyMask((BitMapPtr) *myOffscreen1->portPixMap, (BitMapPtr) *myOffscreen2->portPixMap,
				&(*mainWinPtr).portBits, &offRect, &offRect, &offRect);

	/* Wait until user clicks button. */
	do {
	} while (Button());
	do {
	} while (!Button());
	return 0;
}
