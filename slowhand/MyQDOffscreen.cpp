
#include <iostream>


extern "C" {


#include <MacTypes.h>
#include <MyQuickdraw.h>
#include <MyQDOffscreen.h>


#include "../crossroads.h"


#define _SH_FUNK


////////////////////////////////////////////////////////////////////////////////
// From Quickdraw...
////////////////////////////////////////////////////////////////////////////////


extern void __initCGrafPort(CGrafPort *cgp);
extern PixMapPtr __newPixMap(const Rect *r,unsigned int d);


////////////////////////////////////////////////////////////////////////////////
// QDOffscreen
////////////////////////////////////////////////////////////////////////////////


_SH_FUNK void DisposeGWorld(GWorldPtr offscreenGWorld)
{ 
  if(!offscreenGWorld) { fprintf(stderr,"DisposeGWorld() offscreenGWorld is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"DisposeGWorld() not implemented!\n"); 
}


_SH_FUNK PixMapHandle GetGWorldPixMap(GWorldPtr offscreenGWorld)
{
  if(!offscreenGWorld) { fprintf(stderr,"GetGWorldPixMap() offscreenGWorld is NULL!\n"); exit(EXIT_FAILURE); }
  //fprintf(stderr,"GetGWorldPixMap() not implemented!\n"); 
  CGrafPtr cgp=(CGrafPtr)offscreenGWorld;
  return cgp->portPixMap; 
}


_SH_FUNK QDErr NewGWorld(GWorldPtr *gw, short PixelDepth, const Rect *boundsRect, CTabHandle cTable, GDHandle aGDevice, GWorldFlags flags) 
{ 
  if(!gw) { fprintf(stderr,"NewGWorld() gw is NULL!\n"); exit(EXIT_FAILURE); }
  if(!boundsRect) { fprintf(stderr,"NewGWorld() boundsRect is NULL!\n"); exit(EXIT_FAILURE); }
  if(!PixelDepth) { fprintf(stderr,"NewGWorld() PixelDepth is 0!\n"); exit(EXIT_FAILURE); }
  //fprintf(stderr,"NewGWorld() not implemented!\n"); 
  *gw=(GWorldPtr)malloc(sizeof(CGrafPort));
  __initCGrafPort(*gw);
  (*gw)->portRect.left=boundsRect->left;
  (*gw)->portRect.right=boundsRect->right;
  (*gw)->portRect.top=boundsRect->top;
  (*gw)->portRect.bottom=boundsRect->bottom;
  PixMapPtr *pm=(PixMapPtr *)malloc(sizeof(PixMapPtr));
  *pm=__newPixMap(boundsRect,PixelDepth);
  (*gw)->portPixMap=pm;
  return noErr; 
}


_SH_FUNK void SetGWorld(CGrafPtr port, GDHandle gdh)
{
  if(!port) { fprintf(stderr,"SetGWorld() port is NULL!\n"); exit(EXIT_FAILURE); }
  if(!gdh) { fprintf(stderr,"SetGWorld() gdh is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"SetGWorld() not implemented!\n"); 
}


_SH_FUNK GWorldFlags UpdateGWorld(GWorldPtr *offscreenGWorld, short pixelDepth, const Rect *boundsRect, CTabHandle cTable, GDHandle aGDevice, GWorldFlags flags)
{ 
  if(!offscreenGWorld) { fprintf(stderr,"UpdateGWorld() color is NULL!\n"); exit(EXIT_FAILURE); }
  if(!boundsRect) { fprintf(stderr,"UpdateGWorld() boundsRect is NULL!\n"); exit(EXIT_FAILURE); }
  if(!pixelDepth) { fprintf(stderr,"UpdateGWorld() pixelDepth is 0!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"UpdateGWorld() not implemented!\n"); 
  return noErr; 
}


}
