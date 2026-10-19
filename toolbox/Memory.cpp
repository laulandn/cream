
#include <iostream>


extern "C" {


#include <Memory.h>
#include <Errors.h>


////////////////////////////////////////////////////////////////////////////////
// Memory
////////////////////////////////////////////////////////////////////////////////


void MaxApplZone(void) {
  // NOTE: Ok to skip...
  //fprintf(stderr,"MaxApplZone() not implemented!\n");
}


OSErr HandToHand(Handle *theHndl) {
  if(!theHndl) { fprintf(stderr,"HandToHand() theHndl is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"HandToHand() not implemented!\n");
  return memFullErr;
}


Handle NewHandle(Size byteCount)
{
  fprintf(stderr,"NewHandle() not implemented!\n");
  return NULL;
}


void BlockMove(const void *srcPtr, void *destPtr, Size byteCount)
{
  fprintf(stderr,"BlockMove() not implemented!\n");
}


void DisposeHandle(Handle h)
{
  fprintf(stderr,"DisposeHandle() not implemented!\n");
}


THz ApplicationZone(void) { fprintf(stderr,"ApplicationZone() not implemented!\n"); return NULL; }


void DisposePtr(Ptr p)
{
  fprintf(stderr,"DisposePtr() not implemented!\n");
}


Ptr GetApplLimit(void) { fprintf(stderr,"GetApplLimit() not implemented!\n"); return NULL; }


Size GetHandleSize(Handle h)
{
  fprintf(stderr,"GetHandleSize() not implemented!\n");
  return 0;
}


Ptr NewPtr(Size byteCount) { fprintf(stderr,"NewPtr() not implemented!\n"); return NULL; }


void PurgeSpace(long *total, long *contig) { fprintf(stderr,"PurgeSpace() not implemented!\n"); }


long FreeMem(void) { fprintf(stderr,"FreeMem() not implemented!\n"); return 0; }


THz GetZone(void) { fprintf(stderr,"GetZone() not implemented!\n"); return NULL; }


void HLock(Handle h)
{
  fprintf(stderr,"HLock() not implemented!\n");
}


void HUnlock(Handle h)
{
  fprintf(stderr,"HUnlock() not implemented!\n");
}


Size MaxMem(Size *grow) { fprintf(stderr,"MaxMem() not implemented!\n"); return 0; }


void SetZone(THz hz) { fprintf(stderr,"SetZone() not implemented!\n"); }


THz SystemZone(void) { fprintf(stderr,"SystemZone() not implemented!\n"); return NULL; }


OSErr MemError(void) { fprintf(stderr,"MemError() not implemented!\n"); return noErr; }


void SetHandleSize(Handle h, Size newSize)
{
  fprintf(stderr,"SetHandleSize() not implemented!\n");
}


void SetApplLimit(void *zoneLimit) { fprintf(stderr,"SetApplLimit() not implemented!\n"); }


}
