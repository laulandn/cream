
#include <iostream>


extern "C" {


#include <Resources.h>


////////////////////////////////////////////////////////////////////////////////
// Resources
////////////////////////////////////////////////////////////////////////////////


void CloseResFile(short refNum)
{
  fprintf(stderr,"CloseResFile() not implemented!\n");
}


short OpenResFile(ConstStr255Param fileName)
{
  if(!fileName) { fprintf(stderr,"OpenResFile() fileName is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"OpenResFile() not implemented!\n");
  return 0;
}


Handle GetResource(ResType theType, short theID)
{
  fprintf(stderr,"GetResource() not implemented!\n");
  return NULL;
}


void ChangedResource(Handle theResource)
{
  if(!theResource) { fprintf(stderr,"ChangedResource() theResource is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"OpenResFile() not implemented!\n");
}


void DetachResource(Handle theResource)
{
  if(!theResource) { fprintf(stderr,"DetachResource() theResource is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"DetachResource() not implemented!\n");
}


Handle GetIndResource(ResType theType, short index)
{
  fprintf(stderr,"GetIndResource() not implemented!\n");
  return NULL;
}


void ReleaseResource(Handle theResource)
{
  if(!theResource) { fprintf(stderr,"ReleaseResource() theResource is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"ReleaseResource() not implemented!\n");
}


void WriteResource(Handle theResource)
{
  if(!theResource) { fprintf(stderr,"WriteResource() theResource is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"WriteResource() not implemented!\n");
}


void SetResLoad(Boolean load)
{
  fprintf(stderr,"SetResLoad() not implemented!\n");
}


}
