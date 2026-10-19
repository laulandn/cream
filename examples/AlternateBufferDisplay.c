/*--------------------------------------------------------------------------*/
/*																			*/
/*		AlternateBufferDisplay.												*/
/*			by John Wang													*/
/*																			*/
/*		Description:	Shows that copybits along with offscreen bitmaps	*/
/*			can emulate alternating buffer displays at a reasonable rate.	*/
/*																			*/
/*		Version:		1.0 Completed 11/12/91								*/
/*																			*/
/*--------------------------------------------------------------------------*/

#include 	<Gestalt.h>
#include	<Palettes.h>
#include	<Dialogs.h>
#include	<ToolUtils.h>
#include	<Devices.h>
#include	<Processes.h>
#include	<Fonts.h>
#include	<Menus.h>
#include	<QDOffscreen.h>


extern void ExitToShell();


#define Gestalttest		0xA1AD
#define NoTrap			0xA89F

#define	appleID			128			
#define	appleMenu		0
#define	aboutMeCommand	1

#define	fileID			129
#define	quitCommand 	1

#define	aboutMeDLOG		128
#define	okButton		1

/*------------------------------------------------------*/
/*	Global Variables.									*/
/*------------------------------------------------------*/

Rect				TotalRect, minRect, WinMinusScroll, InitWindowSize;
WindowPtr			myWindow;
Boolean				DoneFlag;
MenuHandle			mymenu0, mymenu1;
GWorldPtr			offscreenGWorld1, offscreenGWorld2;

/*------------------------------------------------------*/
/*	showAboutMeDialog().								*/
/*------------------------------------------------------*/

void showAboutMeDialog()
{
	GrafPtr 	savePort;
	DialogPtr	theDialog;
	short		itemHit;

	GetPort(&savePort);
	theDialog = GetNewDialog(aboutMeDLOG, nil, (WindowPtr) -1);
	SetPort(theDialog);

	do {
		ModalDialog(nil, &itemHit);
	} while (itemHit != okButton);

	CloseDialog(theDialog);

	SetPort(savePort);
	return;
}

/*------------------------------------------------------*/
/*	init().												*/
/*------------------------------------------------------*/

void init()
{
	RgnHandle			tempRgn;
	Rect				BaseRect;
	OSErr				err;
	GDHandle			SaveGD;
	CGrafPtr			SavePort;
	RGBColor			black = {0,0,0}, white = {65535,65535,65535};

	InitGraf(&qd.thePort);
	FlushEvents(everyEvent, 0);
	InitWindows();
	InitDialogs(nil);
	InitCursor();

	/*	Set up menus.	*/
	mymenu0 = GetMenu(appleID);
	AddResMenu(mymenu0, 'DRVR');
	InsertMenu(mymenu0,0);
	mymenu1 = GetMenu(fileID);
	InsertMenu(mymenu1,0);
	DrawMenuBar();

	/* Main initialization.	*/
	DoneFlag = FALSE;

	/*	Set Rects.	*/
	SetRect(&BaseRect, 40, 60, 490, 540);
	SetRect(&WinMinusScroll, BaseRect.left-40, BaseRect.top-60, BaseRect.right-56, 
				BaseRect.bottom - 76);
	SetRect(&InitWindowSize, WinMinusScroll.left, WinMinusScroll.top, 
							WinMinusScroll.right, WinMinusScroll.bottom);
	tempRgn = GetGrayRgn();
	HLock ((Handle) tempRgn);
	TotalRect = (**tempRgn).rgnBBox;
	SetRect(&minRect, 80, 80, (**tempRgn).rgnBBox.right - 40, 
				(**tempRgn).rgnBBox.bottom - 40);
	HUnlock ((Handle) tempRgn);

	/*	Open window and set up picture.	*/
	GetGWorld (&SavePort, &SaveGD);
	myWindow = NewCWindow(nil, &BaseRect, "", TRUE, zoomDocProc, 
							(WindowPtr) -1, TRUE, 150);
	SetGWorld((CGrafPtr)myWindow, SaveGD);
	DrawGrowIcon (myWindow);

	GetGWorld (&SavePort, &SaveGD);
	
	/*	Make first offscreen buffer.	*/
	if (NewGWorld (&offscreenGWorld1, 0, &InitWindowSize, nil, nil, 0))
		Debugger();
	SetGWorld (offscreenGWorld1, nil);
	RGBForeColor(&white);
	PaintRect (&InitWindowSize);
	
	/*	Make second offscreen buffer.	*/
	if (NewGWorld (&offscreenGWorld2, 0, &InitWindowSize, nil, nil, 0))
		Debugger();
	SetGWorld (offscreenGWorld2, nil);
	RGBForeColor(&black);
	PaintRect (&InitWindowSize);
	
	SetGWorld (SavePort, SaveGD);
}

/*------------------------------------------------------*/
/*	doCommand().										*/
/*------------------------------------------------------*/

void doCommand(mResult)
	long	mResult;
{
	int 					theMenu, theItem;
	char					daName[256];
	GrafPtr 				savePort;

	theItem = LoWord(mResult);
	theMenu = HiWord(mResult);
	
	switch (theMenu) {
		case appleID:
			if (theItem == aboutMeCommand)
				showAboutMeDialog();
			else {
				GetItem(mymenu0, theItem, daName);
				GetPort(&savePort);
				(void) OpenDeskAcc(daName);
				SetPort(savePort);
			}
			break;

		case fileID:
			switch (theItem) {
				case quitCommand:
					DoneFlag = TRUE;
					break;
				default:
					break;
				}
			break;
	}
	HiliteMenu(0);
	return;
}

/*------------------------------------------------------*/
/*	draw().												*/
/*------------------------------------------------------*/

void draw()
{
	RGBColor		black = {0,0,0}, White = {65535,65535,65535};
	long			delay;
	char			theString[255];
	GDHandle		screensDevice;
	Rect			area;
	int				i;
		
	RGBForeColor (&black);
	RGBBackColor (&White);
	EraseRect(&WinMinusScroll);
	

	area = WinMinusScroll;
	LocalToGlobal(&topLeft(area));
	LocalToGlobal(&botRight(area));
	screensDevice = GetMaxDevice(&area);
	if (screensDevice != nil) {
		(**(**offscreenGWorld1->portPixMap).pmTable).ctSeed = (**(**(**screensDevice).gdPMap).pmTable).ctSeed;
		(**(**offscreenGWorld2->portPixMap).pmTable).ctSeed = (**(**(**screensDevice).gdPMap).pmTable).ctSeed;
	}
	delay = TickCount();
	for (i=0; i< 10; i++) {
		CopyBits ((BitMap *) *offscreenGWorld2->portPixMap, &myWindow->portBits, 
					&InitWindowSize, &WinMinusScroll, srcCopy, nil);
		CopyBits ((BitMap *) *offscreenGWorld1->portPixMap, &myWindow->portBits, 
					&InitWindowSize, &WinMinusScroll, srcCopy, nil);
	}
	delay = TickCount() - delay;
	MoveTo(50,50);
	NumToString(delay, theString);
	DrawString(theString);
}

/*------------------------------------------------------*/
/*	main().												*/
/*------------------------------------------------------*/

int main(int argc,char **argv)
{
	char			key;
	Boolean			track;
	long			growResult;
	EventRecord 	myEvent;
	WindowPtr		whichWindow;
	int				yieldTime;


	init();
	yieldTime = 0;
	for ( ;; ) {
		if (DoneFlag)
			ExitToShell();
			
		if (WaitNextEvent(everyEvent, &myEvent, yieldTime, nil)) {
			switch (myEvent.what) {
				case mouseDown:
					switch (FindWindow(myEvent.where, &whichWindow)) {
						case inSysWindow:
							SystemClick(&myEvent, whichWindow);
							break;
						case inMenuBar:
							doCommand(MenuSelect(myEvent.where));
							break;
						case inContent:
							break;
						case inDrag:
							DragWindow (whichWindow, myEvent.where, &TotalRect);
							draw();
							DrawGrowIcon (whichWindow);
							break;
						case inGrow:
							growResult = GrowWindow (whichWindow, myEvent.where,
													&minRect);
							SizeWindow(whichWindow, LoWord(growResult), 
									HiWord(growResult), TRUE);
							EraseRect(&whichWindow->portRect);
							SetRect(&WinMinusScroll, whichWindow->portRect.left, 
									whichWindow->portRect.top, 
									whichWindow->portRect.right-20, 
									whichWindow->portRect.bottom - 20);
							draw();
							DrawGrowIcon (whichWindow);
							break;
						case inGoAway:
							track = TrackGoAway (whichWindow, myEvent.where);
							if (track) {
								CloseWindow (whichWindow);
								DoneFlag = TRUE;
								}
							break;
						case inZoomIn:
							track = TrackBox (whichWindow, myEvent.where, inZoomIn);
							if (track) {
								ZoomWindow (whichWindow, inZoomIn, TRUE);
								EraseRect(&whichWindow->portRect);
								SetRect(&WinMinusScroll, whichWindow->portRect.left, 
										whichWindow->portRect.top, 
										whichWindow->portRect.right-20, 
										whichWindow->portRect.bottom - 20);
								draw();
								DrawGrowIcon (whichWindow);
								}
							break;
						case inZoomOut:
							track = TrackBox (whichWindow, myEvent.where, inZoomOut);
							if (track) {
								ZoomWindow (whichWindow, inZoomOut, TRUE);
								EraseRect(&whichWindow->portRect);
								SetRect(&WinMinusScroll, whichWindow->portRect.left, 
										whichWindow->portRect.top, 
										whichWindow->portRect.right-20, 
										whichWindow->portRect.bottom - 20);
								draw();
								DrawGrowIcon (whichWindow);
								}
							break;
						default:
							break;
						}
					break;
				case keyDown:
				case autoKey:
					key = myEvent.message & charCodeMask;
					if ( myEvent.modifiers & cmdKey )
						if ( myEvent.what == keyDown )
							doCommand(MenuKey(key));
					break;
				case updateEvt:
					if ((WindowPtr) myEvent.message == myWindow) {
						BeginUpdate((WindowPtr) myWindow);
						EndUpdate((WindowPtr) myWindow);
						draw();
						}
					break;
				case diskEvt:
					break;
				case activateEvt:
					break;
				case app4Evt:
					if ((myEvent.message << 31) == 0) {
						yieldTime = 30;
						}
					else {
						yieldTime = 0;
						SetPort((WindowPtr) myWindow);
						}
					break; 
				default:
					break;
				}
			}
		}
}