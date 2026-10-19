
#include <iostream>


extern "C" {


#include <Files.h>
#include <Errors.h>




////////////////////////////////////////////////////////////////////////////////
// Files
////////////////////////////////////////////////////////////////////////////////

OSErr OpenWD(short vRefNum, long dirID, long procID, short *wdRefNum) {
  if(!wdRefNum) { fprintf(stderr,"OpenWD() wdRefNum is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"OpenWD() not implemented!\n");
  return fnOpnErr;
}


OSErr CloseWD(short wdRefNum) {
  fprintf(stderr,"CloseWD() not implemented!\n");
  return fnOpnErr;
}


void GetFNum()
{
  fprintf(stderr,"GetFNum() not implemented!\n");
}


OSErr SetVol(ConstStr63Param volName, short vRefNum)
{
  fprintf(stderr,"SetVol() not implemented!\n");
  return noErr;
}


OSErr PBHGetVInfoSync(HParmBlkPtr paramBlock)
{
  fprintf(stderr,"PBHGetVInfoSync() not implemented!\n");
  if(!paramBlock) { fprintf(stderr,"PBHGetVInfoSync() paramBlock is NULL!\n"); exit(EXIT_FAILURE); }
  return noErr;
}


OSErr PBGetFInfoSync(ParmBlkPtr paramBlock)
{
  fprintf(stderr,"PBGetFInfoSync() not implemented!\n");
  if(!paramBlock) { fprintf(stderr,"PBGetFInfoSync() paramBlock is NULL!\n"); exit(EXIT_FAILURE); }
  return noErr;
}


OSErr PBGetVInfoSync(ParmBlkPtr paramBlock)
{
  fprintf(stderr,"PBGetVInfoSync() not implemented!\n");
  if(!paramBlock) { fprintf(stderr,"PBGetVInfoSync() paramBlock is NULL!\n"); exit(EXIT_FAILURE); }
  return noErr;
}


OSErr PBWriteASync(ParmBlkPtr paramBlock)
{
  fprintf(stderr,"PBWriteASync() not implemented!\n");
  if(!paramBlock) { fprintf(stderr,"PBWriteASync() paramBlock is NULL!\n"); exit(EXIT_FAILURE); }
  return noErr;
}


OSErr FSOpen(ConstStr255Param n, short int i, short int *i2)
{
  fprintf(stderr,"FSOpen() not implemented!\n");
  if(!n) { fprintf(stderr,"FSOpen() n is NULL!\n"); exit(EXIT_FAILURE); }
  return noErr;
}


OSErr FSClose(short refNum)
{
  fprintf(stderr,"FSClose() not implemented!\n");
  return noErr;
}


OSErr FSRead(short refNum, long *count, void *buffPtr)
{
  fprintf(stderr,"FSRead() not implemented!\n");
  if(!count) { fprintf(stderr,"FSRead() count is NULL!\n"); exit(EXIT_FAILURE); }
  if(!buffPtr) { fprintf(stderr,"FSRead() buffPtr is NULL!\n"); exit(EXIT_FAILURE); }
  return noErr;
}



}
