
#include <iostream>


extern "C" {


#include <Gestalt.h>
#include <Errors.h>


////////////////////////////////////////////////////////////////////////////////
// Gestalt
////////////////////////////////////////////////////////////////////////////////


OSErr Gestalt(OSType selector, long *response)
{
  fprintf(stderr,"(Gestalt)\n");
  if(!response) { fprintf(stderr,"Gestalt() response is NULL!\n"); exit(EXIT_FAILURE); }
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
    fprintf(stderr,"ignoring.\n");
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
