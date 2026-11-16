
#include <iostream>


extern "C" {


#include <Gestalt.h>
#include <Errors.h>


#include "../crossroads.h"


////////////////////////////////////////////////////////////////////////////////
// Gestalt
////////////////////////////////////////////////////////////////////////////////


OSErr Gestalt(OSType selector, long *response)
{
  open_libs(0,NULL);  // Because this might be called before anything else...
  fprintf(stderr,"(Gestalt)\n"); fflush(stderr);
  if(!response) { fprintf(stderr,"Gestalt() response is NULL!\n"); fflush(stderr);  exit(EXIT_FAILURE); }
  OSErr ret=gestaltUndefSelectorErr;  // gestaltUnknownErr or gestaltUndefSelectorErr?
  char s[5];
  bool done=FALSE;
  s[0]=(selector>>24)&0xff;
  s[1]=(selector>>16)&0xff;
  s[2]=(selector>>8)&0xff;
  s[3]=selector&0xff;
  s[4]=0;
  // TODO: Handle here...
  if(!done) {
    fprintf(stderr,"Gestalt \"");
    fprintf(stderr,"%s",s);
    fprintf(stderr,"\" called...");
    fprintf(stderr,"ignoring.\n"); fflush(stderr);
    *response=0;
  }
  return ret;
}


OSErr NewGestalt(OSType selector, SelectorFunctionUPP gestaltFunction)
{
  OSErr ret=gestaltUnknownErr;
  fprintf(stderr,"NewGestalt() not implemented!\n"); 
  return ret;
}


OSErr ReplaceGestalt(OSType selector, SelectorFunctionUPP gestaltFunction, SelectorFunctionUPP *oldGestaltFunction)
{
  OSErr ret=gestaltUnknownErr;
  fprintf(stderr,"ReplaceGestalt() not implemented!\n"); 
  return ret;
}


}
