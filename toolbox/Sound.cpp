
#include <iostream>


extern "C" {


#include <Sound.h>


////////////////////////////////////////////////////////////////////////////////
// Types
////////////////////////////////////////////////////////////////////////////////


NumVersion myVersion;


////////////////////////////////////////////////////////////////////////////////
// Sound
////////////////////////////////////////////////////////////////////////////////


NumVersion SndSoundManagerVersion()
{
//#ifdef _LATITUDE_
//  myVersion=0;
//#else
/*
  myVersion.majorRev=0;
  myVersion.minorAndBugRev=0;
  myVersion.stage=0;
  myVersion.nonRelRev=0;
*/
//#endif
  return myVersion;
}


OSErr SndNewChannel(SndChannelPtr *chan, short synth, long init, SndCallBackUPP userRoutine)
{
  if(!chan) { fprintf(stderr,"SndNewChannel() chan is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"SndNewChannel() not implemented!\n");
  return 0;
}


OSErr SndDoImmediate(SndChannelPtr chan, const SndCommand *cmd)
{
  fprintf(stderr,"SndDoImmediate() not implemented!\n");
  if(!chan) { fprintf(stderr,"SndDoImmediate() chan is NULL!\n"); exit(EXIT_FAILURE); }
  if(!cmd) { fprintf(stderr,"SndDoImmediate() cmd is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"SndDoImmediate() not implemented!\n");
  return 0;
}


OSErr SndDisposeChannel(SndChannelPtr chan, Boolean quietNow)
{
  if(!chan) { fprintf(stderr,"SndDisposeChannel() chan is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"SndDisposeChannel() not implemented!\n");
  return 0;
}


OSErr SndDoCommand(SndChannelPtr chan, const SndCommand *cmd, Boolean noWait)
{
  if(!cmd) { fprintf(stderr,"SndDoCommand() cmd is NULL!\n"); exit(EXIT_FAILURE); }
  if(!chan) { fprintf(stderr,"SndDoCommand() chan is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"SndDoCommand() not implemented!\n");
  return 0;
}


OSErr SndPlayDoubleBuffer(SndChannelPtr chan, SndDoubleBufferHeaderPtr theParams)
{
  if(!chan) { fprintf(stderr,"SndPlayDoubleBuffer() chan is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"SndPlayDoubleBuffer() not implemented!\n");
  return 0;
}


void GetSoundVol(short *level)
{
  if(!level) { fprintf(stderr,"GetSoundVol() level is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"GetSoundVol() not implemented!\n");
}


void SetSoundVol(short level)
{
  fprintf(stderr,"SetSoundVol() not implemented!\n");
}


}


