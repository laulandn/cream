#include	<Quickdraw.h>
#include	<Windows.h>
#include	<Dialogs.h>
#include	<Events.h>
#include	<Memory.h>
#include	<Packages.h>
#include	<Sound.h>
#include	<SoundInput.h>
#include	<OSUtils.h>

#define	TRUE			0xFF
#define	FALSE			0

int main(int argc,char **argv)
{
	Point				where = {20,20};
	Point				whereto = {100,100};
	OSErr				err;
	SFReply				reply;
	short				refnum;
	char*				string;
	Handle				SNDHand;
	SndChannelPtr		chan;
	
	InitGraf(&qd.thePort);
	FlushEvents(everyEvent, 0);
	InitWindows();
	InitDialogs(nil);
	InitCursor();
	
	string = (char*) "\pPick a SND File";

	SFGetFile (where, (Str255) string, nil, -1, (SFTypeList) nil, nil, &reply);
	err = FSOpen(reply.fName, reply.vRefNum, &refnum);
	if (err != noErr)
		Debugger();
	
	err = SndStartFilePlay (nil, refnum, 0, 74000, nil, nil, nil, TRUE);
	if (err != noErr)
		Debugger();
		
	SNDHand = NewHandle (0xc9000);
	if (MemError() != noErr || SNDHand == nil)
		Debugger();

	err = SndRecord (nil, whereto, siBestQuality, &SNDHand);
	if (err != noErr)
		Debugger();
		
	FSClose (refnum);

	chan = nil;
	err = SndNewChannel (&chan, 0, 0, nil);
	if (err != noErr)
		Debugger();
	
	err = SndPlay (chan, SNDHand, FALSE);
	if (err != noErr)
		Debugger();

	err = SndDisposeChannel (chan,FALSE);
	if (err != noErr)
		Debugger();
}