#include	<Resources.h>
#include	<Sound.h>

#define	TRUE			0xFF
#define	FALSE			0

int main(int argc,char **argv)
{
	Handle			Sound;
	SndChannelPtr	chan;
	int				i;
	OSErr			err;
	
	Sound = GetResource ('snd ', 100);
	if (ResError() != noErr || Sound == nil)
		Debugger();
	
	for (i = 0; i < 3; ++i) {
		chan = nil;
		err = SndNewChannel (&chan, 0, 0, nil);
		if (err != noErr)
			Debugger();
	
		err = SndPlay (chan, Sound, FALSE);
		if (err != noErr)
			Debugger();

		err = SndDisposeChannel (chan,FALSE);
		if (err != noErr)
			Debugger();
		}
}