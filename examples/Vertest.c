/*______________________________________________________*/
/*					  	  C Shell						*/
/*                          by                          */
/*                  RICHARD P. COLLYER                  */
/*                       09/02/87                       */
/*______________________________________________________*/

#include	<CType.h>
#include	<Quickdraw.h>
#include	<Windows.h>
#include	<OSUtils.h>
#include	<Controls.h>
#include	<Dialogs.h>
#include	<Events.h>
#include	<Files.h>
#include	<Memory.h>
#include	<Menus.h>
#include	<Notification.h>
#include	<Events.h>
#include	<Packages.h>
#include	<Palettes.h>
#include	<Resources.h>
#include	<SANE.h>
#include	<SegLoad.h>
#include	<Sound.h>
#include	<String.h>
#include	<ToolUtils.h>
#include	<StdIO.h>
#include	<math.h>

extern _DataInit();

#define	TRUE			0xFF
#define	FALSE			0
#define	VERSION			1
#define	SR_BIT			0

#define	appleID			128			
#define	appleMenu		0
#define	aboutMeCommand	1

#define	fileID			129
#define	startCommand	1
#define	quitCommand 	2

#define	editID			130

#define	aboutMeDLOG		128
#define	okButton		1
#define	authorItem		2
#define	languageItem	3

#define	nocolorID		130
#define	no68020			133
#define	no68881			132
#define	no256			134
#define	nosys6			135

long				Tick;
int					yieldTime, err, numcolor, offLeft, offTop, offRight, offBottom;
Rect				screenRect, BaseRect, TotalRect, minRect;
WindowPtr			whichWindow, myWindow;
CTabHandle			mycolors;
PaletteHandle		srcPalette;
MenuHandle			mymenu1, mymenu2, mymenu0;
EventRecord 		myEvent;
Boolean				DoneFlag;
GDHandle			theGDevice;
SysEnvRec			theWorld;
OSErr				OSys;
	
/*______________________________________________________*/
/*                What ever You want                    */
/*______________________________________________________*/
void start()
{

	VersRecHndl version;
	StringPtr messagePtr;
	Str255 longMessage, countsstrPtr;
	//char *countsstrPtr, countstr;
	
	//countsstrPtr = &countstr;

	version = (VersRecHndl) GetResource ('vers', 1);
	
	messagePtr = (StringPtr) (((unsigned long) &(**version).shortVersion[1]) + 
			((**version).shortVersion[0]));

	BlockMove((Ptr) messagePtr, &longMessage, ((unsigned char) &messagePtr) + 1);

//short version
	MoveTo(10,10);
	DrawString((**version).shortVersion);
//long version
	MoveTo(10,30);
	DrawString(longMessage);
//country code
	MoveTo(10,50);
	NumToString((**version).countryCode,countsstrPtr);
	DrawString(countsstrPtr);
//major rev
	MoveTo(10,70);
	NumToString((**version).numericVersion.majorRev,countsstrPtr);
	DrawString(countsstrPtr);
// minor rev
	MoveTo(10,90);
	NumToString((**version).numericVersion.minorRev,countsstrPtr);
	DrawString(countsstrPtr);
//bug fix rev
	MoveTo(10,110);
	NumToString((**version).numericVersion.bugFixRev,countsstrPtr);
	DrawString(countsstrPtr);
//stage
	MoveTo(10,130);
	NumToString((**version).numericVersion.stage,countsstrPtr);
	DrawString(countsstrPtr);
// non rel rev
	MoveTo(10,150);
	NumToString((**version).numericVersion.nonRelRev,countsstrPtr);
	DrawString(countsstrPtr);
	
	return;
}

/*______________________________________________________*/
/*                 About Prog Dialog                    */
/*______________________________________________________*/
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

/*______________________________________________________*/
/*                 Do Menu Function                     */
/*______________________________________________________*/
void doCommand(mResult)
	long	mResult;
{
	int 					theMenu, theItem;
	char					daName[256];
	GrafPtr 				savePort;

	theItem = LoWord(mResult);
	theMenu = HiWord(mResult);
	
	switch (theMenu) {
/*______________________________________________________*/
/*                    Do Apple Menu                     */
/*______________________________________________________*/
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
/*______________________________________________________*/
/*                     Do File Menu                     */
/*______________________________________________________*/
		case fileID:
			switch (theItem) {
				case startCommand:
					start();
					break;
				case quitCommand:
					DoneFlag = TRUE;
					break;
				default:
					break;
				}
			break;
/*______________________________________________________*/
/*                     Do Edit Menu                     */
/*______________________________________________________*/
		case editID:
			switch (theItem) {
				default:
					break;
				}
			break;
		default:
			break;
		}
	HiliteMenu(0);
	return;
}

/*______________________________________________________*/
/*               Initialization traps                   */
/*______________________________________________________*/
void init()
{
	RgnHandle		tempRgn;
	
	UnloadSeg(_DataInit);
	InitGraf(&qd.thePort);
	FlushEvents(everyEvent, 0);
	InitWindows();
	InitDialogs(nil);
	InitCursor();
/*______________________________________________________*/
/*            If not right Machine then stop            */
/*______________________________________________________*/
	OSys = SysEnvirons(VERSION,&theWorld);
	/*if(!theWorld.hasColorQD) {
		crashDia = GetNewDialog (nocolorID, nil, (WindowPtr) -1);
		DrawDialog (crashDia);
		Delay (300, tick);
		ExitToShell();
		}
	if(theWorld.processor != env68020) {
		crashDia = GetNewDialog (no68020, nil, (WindowPtr) -1);
		DrawDialog (crashDia);
		Delay (300, tick);
		ExitToShell();
		}
	if(!theWorld.hasFPU) {
		crashDia = GetNewDialog (no68881, nil, (WindowPtr) -1);
		DrawDialog (crashDia);
		Delay (300, tick);
		ExitToShell();
		}
	if(theWorld.systemVersion < 0x0600) {
		crashDia = GetNewDialog (nosys6, nil, (WindowPtr) -1);
		DrawDialog (crashDia);
		Delay (300, tick);
		ExitToShell();
		}*/
			
/*______________________________________________________*/
/*                     Set Rects                        */
/*______________________________________________________*/
	screenRect = qd.screenBits.bounds;
	offLeft = 0;
	offTop = 0;
	offRight = screenRect.right;
	offBottom = screenRect.bottom;
	SetRect(&BaseRect, 40, 60, 472, 282);
	tempRgn = GetGrayRgn();
	HLock ((Handle) tempRgn);
	TotalRect = (**tempRgn).rgnBBox;
	SetRect(&minRect, 80, 80, (**tempRgn).rgnBBox.right - 40, 
				(**tempRgn).rgnBBox.bottom - 40);
	HUnlock ((Handle) tempRgn);

/*______________________________________________________*/
/*        Open Window & set Palette & Picture           */
/*______________________________________________________*/
	theGDevice = GetMainDevice();
	HLock ((Handle) theGDevice);
	mycolors = (**(**theGDevice).gdPMap).pmTable;
	numcolor = (**(**theGDevice).gdPMap).pixelSize;
	HUnlock((Handle) theGDevice);
	switch (numcolor) {
		case 1:
			numcolor = 2;
			break;
		case 2:
			numcolor = 4;
			break;
		case 4:
			numcolor = 16;
			break;
		case 8:
			numcolor = 256;
			break;
		}
	
	myWindow = NewCWindow(nil, &BaseRect, "", TRUE, zoomDocProc, 
							(WindowPtr) -1, TRUE, 150);
	SetPort((WindowPtr) myWindow);
	DrawGrowIcon (myWindow);

	srcPalette = NewPalette (numcolor, mycolors, pmCourteous, 0);
	SetPalette ((WindowPtr) myWindow, srcPalette, TRUE);
	
/*______________________________________________________*/
/*                    Set menus                         */
/*______________________________________________________*/
	mymenu0 = GetMenu(appleID);
	AddResMenu(mymenu0, 'DRVR');
	InsertMenu(mymenu0,0);
	mymenu1 = newmenu(129,"File");
	appendmenu(mymenu1,"Start;Quit");
	InsertMenu(mymenu1,0);
	mymenu2 = newmenu(130,"Edit");
	InsertMenu(mymenu2,0);
	DrawMenuBar();

/*______________________________________________________*/
/*                  Init variables                      */
/*______________________________________________________*/
	DoneFlag = FALSE;
	yieldTime = 0;
	return;
}

int main(int argc,char **argv)
{
	Boolean		track;
	long		growResult;
/*______________________________________________________*/
/*                   Main Event loop                    */
/*______________________________________________________*/
	init();
	for ( ;; ) {
		if (DoneFlag) {
			ExitToShell();
			}
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
							EraseRect (&whichWindow->portRect);
							DrawGrowIcon (whichWindow);
							break;
						case inGrow:
							growResult = GrowWindow (whichWindow, myEvent.where,
													&minRect);
							SizeWindow(whichWindow, LoWord(growResult), 
									HiWord(growResult), TRUE);
							EraseRect (&whichWindow->portRect);
							DrawGrowIcon (whichWindow);
							break;
						case inGoAway:
							track = TrackGoAway (whichWindow, myEvent.where);
							if (track)
								CloseWindow (whichWindow);
							break;
						case inZoomIn:
							track = TrackBox (whichWindow, myEvent.where, inZoomIn);
							if (track) {
								ZoomWindow (whichWindow, inZoomIn, TRUE);
								EraseRect (&whichWindow->portRect);
								DrawGrowIcon (whichWindow);
								}
							break;
						case inZoomOut:
							track = TrackBox (whichWindow, myEvent.where, inZoomOut);
							if (track) {
								ZoomWindow (whichWindow, inZoomOut, TRUE);
								EraseRect (&whichWindow->portRect);
								DrawGrowIcon (whichWindow);
								}
							break;
						default:
							break;
						}
					break;
				case keyDown:
					break;
				case autoKey:
					break;
				case updateEvt:
					if ((WindowPtr) myEvent.message == myWindow) {
						BeginUpdate((WindowPtr) myWindow);
						EndUpdate((WindowPtr) myWindow);
						}
					break;
				case diskEvt:
					break;
				case activateEvt:
					break;
				case app4Evt:
					if ((myEvent.message << 31) == 0) { /* Suspend */
						yieldTime = 30;
						HideWindow((WindowPtr) myWindow);
						}
					else { /* Resume */
						yieldTime = 0;
						ShowWindow((WindowPtr) myWindow);
						SetPort((WindowPtr) myWindow);
						}
					break;
				default:
					break;
				}
			}
		}
}