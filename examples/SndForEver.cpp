#include	<Resources.h>
#include	<Sound.h>
#include	<Events.h>
#include	<Memory.h>

#define	TRUE			0xFF
#define	FALSE			0

int main(int argc,char **argv)
{
	Handle			SoundData;
	SndChannelPtr	chan;
	OSErr			err;
	SndCommand		mycmd;
	
	SoundData = GetResource ('snd ', 128);
	if (ResError() != noErr || SoundData == nil)
		Debugger();
	HLock (SoundData);
			
	mycmd.cmd = soundCmd;
	mycmd.param1 = 0;
	mycmd.param2 = (long) *SoundData;
	
	chan = nil;
	err = SndNewChannel (&chan, sampledSynth, 0, nil);
	if (err != noErr)
		Debugger();
	
	err = SndDoImmediate (chan, &mycmd);
	if (err != noErr)
		Debugger();

	mycmd.cmd = freqCmd;
	mycmd.param1 = 0;
	mycmd.param2 = 60;
	
	err = SndDoCommand (chan, &mycmd, FALSE);
	if (err != noErr)
		Debugger();

	do {
		/*mycmd.cmd = noteCmd;
		mycmd.param1 = 0x1000;
		mycmd.param2 = 60;
	
		err = SndDoCommand (chan, &mycmd, FALSE);
		if (err != noErr)
			Debugger();*/
		
		} while (!StillDown());
		
	mycmd.cmd = quietCmd;
	mycmd.param1 = 0;
	mycmd.param2 = 0;
	
	err = SndDoImmediate (chan, &mycmd);
	if (err != noErr)
		Debugger();

	err = SndDisposeChannel (chan,FALSE);
	if (err != noErr)
		Debugger();
		
	HUnlock (SoundData);
}