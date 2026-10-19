/*______________________________________________________*/
/*					  	  C Shell						*/
/*                          by                          */
/*                  RICHARD P. COLLYER                  */
/*                       10/14/91                       */
/*______________________________________________________*/

#include	<Quickdraw.h>
#include	<Windows.h>
#include	<Memory.h>
#include	<Menus.h>
#include	<Events.h>
#include	<Resources.h>
#include	<SegLoad.h>
#include	<Sound.h>
#include	<ToolUtils.h>

extern _DataInit();

#define	TRUE			0xFF
#define	FALSE			0

#define	appleID			128			
#define	appleMenu		0
#define	aboutMeCommand	1

#define	fileID			129
#define	startCommand	1
#define printCommand	3
#define	quitCommand 	5

#define	editID			130

int					yieldTime, err;
MenuHandle			mymenu1, mymenu2, mymenu0;
EventRecord 		myEvent;
Boolean				DoneFlag;
WindowPtr			whichWindow;
short				GlobalFlag;
	
/*______________________________________________________*/
/*                What ever You want                    */
/*______________________________________________________*/
pascal void CallBack(SndChannelPtr chan, SndCommand cmd)
{
	long	myA5;
	
	myA5 = SetA5 (cmd.param2);
	GlobalFlag = TRUE;
	myA5 = SetA5 (myA5);
	
	return;
}
	
/*______________________________________________________*/
/*                What ever You want                    */
/*______________________________________________________*/
void start()
{
	Handle			Sound;
	SndChannelPtr	chan;
	SndCommand		mycmd;
	
	GlobalFlag = FALSE;
	
	Sound = GetResource ('snd ', 100);
	if (ResError() != noErr || Sound == nil)
		Debugger();
	
	chan = nil;
	err = SndNewChannel (&chan, sampledSynth, initNoInterp + initMono, (SndCallBackProcPtr) CallBack);
	if (err != noErr)
		Debugger();
	
	do {
		mycmd.cmd = bufferCmd;
		mycmd.param1 = 0;
		mycmd.param2 = (long) *Sound;
		err = SndDoCommand (chan, &mycmd, FALSE);
		if (err != noErr)
			Debugger();
		
		mycmd.cmd = callBackCmd;
		mycmd.param1 = 0;
		mycmd.param2 = SetCurrentA5();
		err = SndDoCommand (chan, &mycmd, FALSE);
		if (err != noErr)
			Debugger();
		
		do{
			}while (GlobalFlag == FALSE);

		GlobalFlag = FALSE;

		} while (!Button());
		
	err = SndDisposeChannel (chan,FALSE);
	if (err != noErr)
		Debugger();

	return;
}

/*______________________________________________________*/
/*                 About Prog Dialog                    */
/*______________________________________________________*/
void showAboutMeDialog()
{
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
				case printCommand:
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
	UnloadSeg(_DataInit);
	InitGraf(&qd.thePort);
	FlushEvents(everyEvent, 0);
	InitMenus();
	InitCursor();
	MaxApplZone();
/*______________________________________________________*/
/*                    Set menus                         */
/*______________________________________________________*/
	mymenu0 = GetMenu(appleID);
	AddResMenu(mymenu0, 'DRVR');
	InsertMenu(mymenu0,0);
	mymenu1 = newmenu(129,"File");
	appendmenu(mymenu1,"Start;-;Print;-;Quit");
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
						default:
							break;
						}
					break;
				case app4Evt:
					if ((myEvent.message << 31) == 0) { /* Suspend */
						yieldTime = 30;
						}
					else { /* Resume */
						yieldTime = 0;
						}
					break;
				default:
					break;
				}
			}
		}
}