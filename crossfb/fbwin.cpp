
#include <stdio.h>
#include <stdlib.h>


#ifdef MACINTOSH
#include <MyQuickdraw.h>
#endif


extern "C" {


#include "../crossroads.h"


#include "fb.h"


#ifdef MACINTOSH
extern QDGlobals qd;
#endif


void dump_fb_info(void)
{
  fprintf(stderr,"fbPixels=%08x\n",fbPixels); fflush(stderr);
  fprintf(stderr,"fbPixelByteSize=%d\n",fbPixelByteSize); fflush(stderr);
  fprintf(stderr,"fbMinBlit=%d fbMaxBlit=%d\n",fbMinBlit,fbMaxBlit); fflush(stderr);
  fprintf(stderr,"fbRowBytes=%d\n",fbRowBytes); fflush(stderr);
  fprintf(stderr,"fb is %d x %d x %d\n",fbWidth,fbHeight,fbDepth); fflush(stderr);
}


struct cross_display *open_cross_display(void *a,unsigned int w, unsigned int h, unsigned d)
{
  struct cross_display *dis=NULL;
  fprintf(stderr,"open_cross_display %d x %d x %d\n",w,h,d); fflush(stderr);
  dis=(struct cross_display *)calloc(sizeof(struct cross_display),1);
  if(!dis) {
    fprintf(stderr,"No dis!\n");
    exit(5);
  }
  if(!w) { w=512;}
  if(!h) { h=342; }
  if(!d) { d=1; }
#ifdef MACINTOSH
  fprintf(stderr,"(NOTE: MACINTOSH is set!)\n"); fflush(stderr);
  w=qd.screenBits.bounds.right-qd.screenBits.bounds.left;
  h=qd.screenBits.bounds.bottom-qd.screenBits.bounds.top;
  // TODO: Get real depth here...
#endif
  dis->width=w;
  dis->height=h;
  dis->depth=d;
  dis->nPlanes=1;
#ifdef SNIAL
  fprintf(stderr,"(NOTE: SNIAL is set!)\n"); fflush(stderr);
  // TODO Test if really MacCP...
  if(0) {
    fprintf(stderr,"This program only works on a MacCP!\n");
    exit(5);
  }
  // TODO: Blast the real pixels into slowhand's here...
  for(int ix=0; ix<16; ix++) {
    PaletteSet(15-ix, gColorPalette[15-ix]);
  }
  ModeSet(1); // now in color.
#endif
#ifdef MACINTOSH
  fbPixels=qd.thePort->portBits.baseAddr;
  // TODO: Don't hard code this
  fbRowBytes=128; // ScreenBits.rowBytes<<1;
#endif
  fbWidth=w; fbHeight=h; fbDepth=d;
  // NOTE: This assumes chunky pixels
  switch(fbDepth) {
    case 1: fbMinBlit=8; fbMaxBlit=1; break;
    case 2: fbMinBlit=4; fbMaxBlit=1; break;
    case 4: fbMinBlit=2; fbMaxBlit=1; break;
    case 8: fbMinBlit=1; fbMaxBlit=1; break;
    case 15: fbMinBlit=1; fbMaxBlit=2; break;  // Really 16
    case 16: fbMinBlit=1; fbMaxBlit=2; break;
    case 24: fbMinBlit=1; fbMaxBlit=3; break;
    case 32: fbMinBlit=1; fbMaxBlit=4; break;
    default:
      break;
  }
  fbPixelByteSize=fbWidth*fbRowBytes;
  //
  dump_fb_info();
  //
  return dis;
}


struct cross_window *open_cross_window(void *a,char *s,unsigned int w, unsigned int h)
{
  struct cross_window *win=NULL;
  fprintf(stderr,"open_cross_window %d x %d\n",w,h); fflush(stderr);
  win=(struct cross_window *)calloc(sizeof(struct cross_window),1);
  if(!win) {
    fprintf(stderr,"No win!\n");
    exit(5);
  }
  if(a) {
    fprintf(stderr,"(probably got a display)\n"); fflush(stderr);
  }
  if(!w) { w=512;}
  if(!h) { h=342; }
  win->width=w;
  win->height=h;
  // TODO
  fbWidth=w; fbHeight=h;
  return win;
}


};