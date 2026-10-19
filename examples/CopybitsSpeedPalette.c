/*--------------------------------------------------------------------------*/
/*																			*/
/*		CopybitsSpeedPalette.												*/
/*			by John Wang													*/
/*																			*/
/*		Description:	This program demostrates ways to increase copybits	*/
/*			speed when using palettes.										*/
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
#include	<QDOffscreen.h>


extern void ExitToShell();


#define Gestalttest		0xA1AD
#define NoTrap			0xA89F

#define	appleID			128			
#define	appleMenu		0
#define	aboutMeCommand	1

#define	fileID			129
#define	quitCommand 	1

#define	PictID			128
#define	clutID			150

#define	aboutMeDLOG		128
#define	okButton		1

/*------------------------------------------------------*/
/*	Global Variables.									*/
/*------------------------------------------------------*/

Rect				TotalRect, minRect, WinMinusScroll, InitWindowSize;
WindowPtr			myWindow;
CTabHandle			mycolors;
PaletteHandle		srcPalette;
Boolean				DoneFlag;
MenuHandle			mymenu0, mymenu1;
PicHandle			ThePict;
GWorldPtr			offscreenGWorld;

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
	long				QDfeature, OSfeature;
	GDHandle			SaveGD;
	CGrafPtr			SavePort;

	/*	Initialize Managaer.	*/
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
	DoneFlag = FALSE;
	ThePict = GetPicture(PictID);
	if (ThePict == nil)
		DoneFlag = TRUE;

	/*	Use Gestalt to find is QuickDraw is avaiable.	*/
	if ((GetTrapAddress(Gestalttest) != GetTrapAddress(NoTrap))) {
		err = Gestalt(gestaltQuickdrawVersion, &QDfeature);
		if (err)
			DoneFlag = TRUE;
		err = Gestalt(gestaltSystemVersion, &OSfeature);
		if (err)
			DoneFlag = TRUE;
		if (!DoneFlag && (QDfeature & 0x0f00) != 0x0200 && OSfeature < 0x0605)
			DoneFlag = TRUE;
		}
	else
		DoneFlag = TRUE;

	/*	Set Rects.	*/
	SetRect(&BaseRect, 40, 60, 472, 282);
	SetRect(&WinMinusScroll, BaseRect.left-40, BaseRect.top-60, BaseRect.right-60, 
				BaseRect.bottom - 80);
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
	mycolors = GetCTable (clutID);
	(*mycolors)->ctFlags |= 0x4000;

	myWindow = NewCWindow(nil, &BaseRect, "", TRUE, zoomDocProc, 
							(WindowPtr) -1, TRUE, 150);
	SetGWorld((CGrafPtr)myWindow, SaveGD);
	DrawGrowIcon (myWindow);

	srcPalette = NewPalette (((**mycolors).ctSize)+1, mycolors,
			pmTolerant + pmExplicit + pmAnimated, 0);
	SetPalette ((WindowPtr) myWindow, srcPalette, TRUE);
	
	GetGWorld (&SavePort, &SaveGD);
	err = NewGWorld (&offscreenGWorld, 8, &InitWindowSize, mycolors, nil, 0);
	if (err)
		Debugger();
	SetGWorld (offscreenGWorld, nil);
	EraseRect (&InitWindowSize);
	DrawPicture (ThePict, &InitWindowSize);
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
/*	draw.												*/
/*------------------------------------------------------*/

void draw()
{
	RGBColor		black = {0,0,0}, White = {65535,65535,65535};
	long			before, delay;
	char theString[255];
	GDHandle		screensDevice;
	Rect			area;
		
	RGBForeColor (&black);
	RGBBackColor (&White);
	EraseRect(&WinMinusScroll);
	
	/* This is the only change made to support a faster copybits on one screen.
		ctFlags is still set above. */
	area = WinMinusScroll;
	LocalToGlobal(&topLeft(area));
	LocalToGlobal(&botRight(area));
	screensDevice = GetMaxDevice(&area);
	if (screensDevice != nil)
		(**(**offscreenGWorld->portPixMap).pmTable).ctSeed = (**(**(**screensDevice).gdPMap).pmTable).ctSeed;
		
	CopyBits ((BitMap *) *offscreenGWorld->portPixMap, &myWindow->portBits, 
					&InitWindowSize, &WinMinusScroll, srcCopy, nil);
	EraseRect(&WinMinusScroll);
	CopyBits ((BitMap *) *offscreenGWorld->portPixMap, &myWindow->portBits, 
					&InitWindowSize, &WinMinusScroll, srcCopy, nil);
	EraseRect(&WinMinusScroll);
	CopyBits ((BitMap *) *offscreenGWorld->portPixMap, &myWindow->portBits, 
					&InitWindowSize, &WinMinusScroll, srcCopy, nil);
	EraseRect(&WinMinusScroll);
	CopyBits ((BitMap *) *offscreenGWorld->portPixMap, &myWindow->portBits, 
					&InitWindowSize, &WinMinusScroll, srcCopy, nil);
	EraseRect(&WinMinusScroll);
	before = TickCount();
	CopyBits ((BitMap *) *offscreenGWorld->portPixMap, &myWindow->portBits, 
					&InitWindowSize, &WinMinusScroll, srcCopy, nil);
	delay = TickCount() - before;
/*	DebugStr("\PIt's me!!!");	*/
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
			return 0;
			
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
		return 0;
}