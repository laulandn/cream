
#include <iostream>

#include <stdlib.h>
#include <string.h>


// NOTE: Doesn't work yet
//#define REAL_QD_GLOBALS 1


extern "C" {


#include <MyQuickdraw.h>


#include "slowhand.h"


#include "../crossroads.h"


#define _SH_FUNK


////////////////////////////////////////////////////////////////////////////////
// globals
////////////////////////////////////////////////////////////////////////////////


cross_display *the_cross_display;
// Only one window for now...
cross_window *the_cross_window;

unsigned int shCurX=0,shCurY=0;

static short qderror=0;

static GDevice theMacGDevice;
static GDevice *theMacGDevicePtr=NULL;
static bool theMacGDeviceInited=FALSE;


#ifdef REAL_QD_GLOBALS
// Quickdraw globals
// Privates...
long fontAdj=0;
Point patAlign;
short polyMax=0;
PolyHandle thePoly=NULL;
short playIndex=0;
PicHandle playPic=NULL;
short rgnMax=0;
short rgnIndex=0;
Handle rgnBuf=NULL;
Region wideData;
RgnPtr wideMaster=NULL;
RgnHandle wideOpen=NULL;
//
// Publics...
long randSeed=0;
BitMap screenBits;
Cursor arrow;
Pattern dkGray;
Pattern ltGray;
Pattern gray;
Pattern black;
Pattern white;
GrafPtr thePort=NULL;
#endif

QDGlobals qd;

static bool qdInited=FALSE;


void __printrect(const char *label,const Rect *r);
void __rectSetDefault(Rect *r);

void __initPixMap(PixMap *pm);
void __initGDevice(GDevice *gd);
void __initGrafPort(GrafPort *gp);
void __initCGrafPort(CGrafPort *cgp);
PixMapPtr __newPixMap(const Rect *r,unsigned int d);
bool __isPixMap(const BitMap *bm);
bool __isCGrafPort(const GrafPort *gp);

#ifdef REAL_QD_GLOBALS
void __syncQDGlobals(QDGlobals *qdGlobals);
#endif

void __syncGrafPortAndCGrafPort(CGrafPort *cgp);


////////////////////////////////////////////////////////////////////////////////
// my Quickdraw internals
////////////////////////////////////////////////////////////////////////////////


bool __isPixMap(const BitMap *bm)
{
  bool ret=true;
  fprintf(stderr,"rowBytes is %x\n",bm->rowBytes);
  if(bm->rowBytes&0xa000) ret=false;
  PixMap *pm=(PixMap *)bm;
  fprintf(stderr,"pmVersion is %x\n",pm->pmVersion);
  fprintf(stderr,"pmReserved is %lx\n",pm->pmReserved);
  if(!ret) fprintf(stderr,"Not a PixMap...\n");
  return ret;
}


bool __isCGrafPort(const GrafPort *gp)
{
  bool ret=true;
  CGrafPort *cgp=(CGrafPort *)cgp;
  fprintf(stderr,"portVersion is %x\n",cgp->portVersion);
  if(cgp->portVersion<0x3ffe) ret=false;
  if(!ret) fprintf(stderr,"Not a CGrafPort...\n");
  return ret;
}


#ifdef REAL_QD_GLOBALS
void __sync_QDGlobals(QDGlobals *qdGlobals)
{
  fprintf(stderr,"(__sync_QDGlobals)\n");
  if(!qdGlobals) { fprintf(stderr,"__sync_QDGlobals() qdGlobals is NULL!\n"); exit(EXIT_FAILURE); }
  qdGlobals->screenBits.bounds.top=screenBits.bounds.top;
  qdGlobals->screenBits.bounds.bottom=screenBits.bounds.bottom;
  qdGlobals->screenBits.bounds.left=screenBits.bounds.left;
  qdGlobals->screenBits.bounds.right=screenBits.bounds.right;
  // TODO: sync arrow
  // TODO: sync dkGray...
  // TODO: sync ltGray...
  // TODO: sync gray...
  // TODO: sync black...
  // TODO: sync white...
  //thePort=(GrafPort *)&theMacCGrafPort;
  //__initGrafPort(thePort);
  qdGlobals->thePort=thePort;
#ifdef DEBUG
  __printrect("__sync_QDGlobals qd.screenbits.bounds",&qd.screenBits.bounds);
  __printrect("__sync_QDGlobals screenbits.bounds",&screenBits.bounds);
#endif
}
#endif


// NOTE: Only inits, does NOT fill in valid values!
// ...but for now, set to old classic mac screen size...
void __init_QDGlobals(QDGlobals *qdGlobals)
{
  fprintf(stderr,"(__init_QDGlobals)\n");
  if(!qdGlobals) { fprintf(stderr,"__init_s() qdGlobals is NULL!\n"); exit(EXIT_FAILURE); }
  if(qdInited) {
    fprintf(stderr,"(qdInited already set!)\n");
    return;
  }
  memset(qdGlobals,0,sizeof(QDGlobals));
#ifdef REAL_QD_GLOBALS
  fprintf(stderr,"(REAL_QD_GLOBALS is set)\n");
  fontAdj=0;
  //patAlign;
  polyMax=0;
  thePoly=NULL;
  playIndex=0;
  playPic=NULL;
  rgnMax=0;
  rgnIndex=0;
  rgnBuf=NULL;
  //wideData;
  wideMaster=NULL;
  wideOpen=NULL;
  //
  randSeed=0;
  //
  //arrow
	//dkGray={ 128,128,128,128,128,128,128,128 };  // THIS ISN't RIGHT;
	//ltGray={ 128,128,128,128,128,128,128,128 };  // THIS ISN't RIGHT;
	//gray={ 128,128,128,128,128,128,128,128 };  // THIS ISN't RIGHT
	//black={ 0,0,0,0,0,0,0,0 };
	//white={ 255,255,255,255,255,255,255,255 };
  thePort=NULL;
  //
  screenBits.bounds.top=0;
  screenBits.bounds.bottom=384;
  screenBits.bounds.left=512;
  screenBits.bounds.right=0;
  __rectSetDefault(&screenBits.bounds);
  //
  if(the_cross_display) { screenBits.bounds.right=the_cross_display->width; screenBits.bounds.bottom=the_cross_display->height; }
  else fprintf(stderr,"no the_cross_display!!!!\n");
  //
#ifdef DEBUG
  __printrect("__init_QDGlobals screenbits.bounds",&screenBits.bounds);
#endif
  __sync_QDGlobals(&qd);
#else
  fprintf(stderr,"(REAL_QD_GLOBALS is NOT set)\n");
  //qd.fontAdj=0;
  //qd.patAlign;
  //qd.polyMax=0;
  //qd.thePoly=NULL;
  //qd.playIndex=0;
  //qd.playPic=NULL;
  //qd.rgnMax=0;
  //qd.rgnIndex=0;
  //qd.rgnBuf=NULL;
  //qd.wideData;
  //qd.wideMaster=NULL;
  //qd.wideOpen=NULL;
  //
  qd.randSeed=0;
  //
  //qd.arrow
	//qd.dkGray={ 128,128,128,128,128,128,128,128 };  // THIS ISN't RIGHT;
	//qd.ltGray={ 128,128,128,128,128,128,128,128 };  // THIS ISN't RIGHT;
	//qd.gray={ 128,128,128,128,128,128,128,128 };  // THIS ISN't RIGHT
	//qd.black={ 0,0,0,0,0,0,0,0 };
	//qd.white={ 255,255,255,255,255,255,255,255 };
  qd.thePort=NULL;
  //
  qd.screenBits.bounds.top=0;
  qd.screenBits.bounds.bottom=384;
  qd.screenBits.bounds.left=512;
  qd.screenBits.bounds.right=0;
  __rectSetDefault(&qd.screenBits.bounds);
  //
  if(the_cross_display) { qd.screenBits.bounds.right=the_cross_display->width; qd.screenBits.bounds.bottom=the_cross_display->height; }
  else fprintf(stderr,"no the_cross_display!!!!\n");
  //
#endif
  qdInited=TRUE;
  fprintf(stderr,"(__init_QDGlobals done)\n");
}


// NOTE: Only inits, does NOT fill in valid values!
// ...but for now, set to old classic mac screen size...
void __initPixMap(PixMap *pm)
{
  fprintf(stderr,"(__initPixMap)\n");
  if(!pm) { fprintf(stderr,"__initPixMap() pm is NULL!\n"); exit(EXIT_FAILURE); }
  memset(pm,0,sizeof(PixMap));
  pm->baseAddr=0;
  pm->rowBytes=0;  // ???
  //
  pm->bounds.top=0;
  pm->bounds.left=0;
  pm->bounds.right=0;
  pm->bounds.bottom=0;
  __rectSetDefault(&pm->bounds);
  //
  if(the_cross_display) { pm->bounds.right=the_cross_display->width; pm->bounds.bottom=the_cross_display->height; }
  else fprintf(stderr,"no the_cross_display!!!!\n");
  if(the_cross_window) { pm->bounds.right=the_cross_window->width; pm->bounds.bottom=the_cross_window->height; }
  else fprintf(stderr,"no the_cross_window!!!\n");
  //
#ifdef DEBUG
  __printrect("__initPixMap pm->bounds",&pm->bounds);
#endif
  //
  pm->pmVersion=1; // ???
  pm->packType=0;
  pm->packSize=0;
  pm->hRes=75;
  pm->vRes=75;
  pm->pixelType=1; // ???
  pm->pixelSize=1; // ???
  pm->cmpCount=4; // ???
  pm->cmpSize=1; // ???
  pm->planeBytes=0;
  pm->pmTable=NULL;
  pm->pmReserved=0;
  if(the_cross_display) { pm->pixelSize=the_cross_display->depth; }
  else fprintf(stderr,"no the_cross_display!!!!\n");
}


// TODO: This init's it, not sync's it, needs to be done
void __syncGrafPortAndCGrafPort(CGrafPort *cgp)
{
  fprintf(stderr,"(__syncGrafPortAndCGrafPort)\n");
  if(!cgp) { fprintf(stderr,"__syncGrafPortAndCGrafPort() gp is NULL!\n"); exit(EXIT_FAILURE); }
  GrafPort *gp=(GrafPort *)cgp;
	gp->device=0;
  gp->portBits.baseAddr=0;
  gp->portBits.rowBytes=0;
  //
  gp->portBits.bounds.top=0;
  gp->portBits.bounds.left=0;
  gp->portBits.bounds.right=0;
  gp->portBits.bounds.bottom=0;
  __rectSetDefault(&gp->portBits.bounds);
  //
  if(the_cross_display) { gp->portBits.bounds.right=the_cross_display->width; gp->portBits.bounds.bottom=the_cross_display->height; }
  else fprintf(stderr,"no the_cross_display!!!\n");
  if(the_cross_window) { gp->portBits.bounds.right=the_cross_window->width; gp->portBits.bounds.bottom=the_cross_window->height; }
  else fprintf(stderr,"no the_cross_window!!!\n");
  //
#ifdef DEBUG
  __printrect("__syncGrafPortAndCGrafPort gp->portBits.bounds",&gp->portBits.bounds);
#endif
  //
	gp->visRgn=NULL;
	gp->clipRgn=NULL;
  unsigned int t=0;
  for(t=0;t<8;t++) gp->bkPat.pat[t]=0;
  for(t=0;t<8;t++) gp->fillPat.pat[t]=0;
  gp->pnLoc.h=0; gp->pnLoc.v=0;
	gp->pnSize.h=32; gp->pnSize.v=32;
	gp->pnMode=1; // ???
#ifdef __LATITUDE_LONG_ALIGNED__
  gp->filler1=0;
#endif
  for(t=0;t<8;t++) gp->pnPat.pat[t]=0;
	gp->pnVis=1;
	gp->txFont=1; // ???
	gp->txFace=1; // ???
	//gp->filler=0;
	gp->txMode=1; // ???
	gp->txSize=12;
	gp->spExtra=0;
	gp->fgColor=0;
	gp->bkColor=1;
	gp->colrBit=0; // ???
	gp->patStretch=0;
	gp->picSave=NULL;
	gp->rgnSave=NULL;
	gp->polySave=NULL;
	gp->grafProcs=NULL;
 
}


// NOTE: Only inits, does NOT fill in valid values!
// ...but for now, set to old classic mac screen size...
void __initGrafPort(GrafPort *gp)
{
  fprintf(stderr,"(__initGrafPort)\n");
  if(!gp) { fprintf(stderr,"__initGrafPort() gp is NULL!\n"); exit(EXIT_FAILURE); }
  memset(gp,0,sizeof(GrafPort));
	gp->device=0;
  gp->portBits.baseAddr=0;
  gp->portBits.rowBytes=0;
  //
  gp->portBits.bounds.top=0;
  gp->portBits.bounds.left=0;
  gp->portBits.bounds.right=0;
  gp->portBits.bounds.bottom=0;
  __rectSetDefault(&gp->portBits.bounds);
  //
  if(the_cross_display) { gp->portBits.bounds.right=the_cross_display->width; gp->portBits.bounds.bottom=the_cross_display->height; }
  else fprintf(stderr,"no the_cross_display!!!\n");
  if(the_cross_window) { gp->portBits.bounds.right=the_cross_window->width; gp->portBits.bounds.bottom=the_cross_window->height; }
  else fprintf(stderr,"no the_cross_window!!!\n");
  //
#ifdef DEBUG
  __printrect("__initGrafPort gp->portBits.bounds",&gp->portBits.bounds);
#endif
  //
	gp->visRgn=NULL;
	gp->clipRgn=NULL;
  unsigned int t=0;
  for(t=0;t<8;t++) gp->bkPat.pat[t]=0;
  for(t=0;t<8;t++) gp->fillPat.pat[t]=0;
  gp->pnLoc.h=0; gp->pnLoc.v=0;
	gp->pnSize.h=32; gp->pnSize.v=32;
	gp->pnMode=1; // ???
#ifdef __LATITUDE_LONG_ALIGNED__
  gp->filler1=0;
#endif
  for(t=0;t<8;t++) gp->pnPat.pat[t]=0;
	gp->pnVis=1;
	gp->txFont=1; // ???
	gp->txFace=1; // ???
	//gp->filler=0;
	gp->txMode=1; // ???
	gp->txSize=12;
	gp->spExtra=0;
	gp->fgColor=0;
	gp->bkColor=1;
	gp->colrBit=0; // ???
	gp->patStretch=0;
	gp->picSave=NULL;
	gp->rgnSave=NULL;
	gp->polySave=NULL;
	gp->grafProcs=NULL;
}


// NOTE: Only inits, does NOT fill in valid values!
// ...but for now, set to old classic mac screen size...
void __initCGrafPort(CGrafPort *cgp)
{
  fprintf(stderr,"(__initCGrafPort)\n");
  if(!cgp) { fprintf(stderr,"__initCGrafPort() cgp is NULL!\n"); exit(EXIT_FAILURE); }
  memset(cgp,0,sizeof(CGrafPort));
  cgp->device=0;
  cgp->portPixMap=NULL;
  cgp->grafVars=NULL;
	cgp->portVersion=0xa000;
	cgp->chExtra=0;
	cgp->pnLocHFrac=0;
  //
  cgp->portRect.top=0;
  cgp->portRect.left=0;
  cgp->portRect.right=0;
  cgp->portRect.bottom=0;
  __rectSetDefault(&cgp->portRect);
  //
  if(the_cross_display) { cgp->portRect.right=the_cross_display->width; cgp->portRect.bottom=the_cross_display->height; }
  else fprintf(stderr,"no the_cross_display!!!\n");
  if(the_cross_window) { cgp->portRect.right=the_cross_window->width; cgp->portRect.bottom=the_cross_window->height; }
  else fprintf(stderr,"no the_cross_window!!!\n");
  //
#ifdef DEBUG
  __printrect("__initCGrafPort cgp->portRect",&cgp->portRect);
#endif
  //
	cgp->bkPixPat=NULL;
  cgp->rgbFgColor.red=0x0000;  cgp->rgbFgColor.green=0x0000;  cgp->rgbFgColor.blue=0x0000;
  cgp->rgbBkColor.red=0xffff;  cgp->rgbBkColor.green=0xffff;  cgp->rgbBkColor.blue=0xffff;
	cgp->visRgn=NULL;
	cgp->clipRgn=NULL;
  cgp->pnLoc.h=0; cgp->pnLoc.v=0;
	cgp->pnSize.h=32; cgp->pnSize.v=32;
	cgp->pnMode=1;
	cgp->pnPixPat=NULL;
	cgp->fillPixPat=NULL;
	cgp->pnVis=1;
	cgp->txFont=1;
	cgp->txFace=1;
	//cgp->filler=0;
	cgp->txMode=1;
	cgp->txSize=12;
	cgp->spExtra=0;
	cgp->fgColor=0;
	cgp->bkColor=1;
	cgp->colrBit=0;
	cgp->patStretch=0;
	cgp->picSave=NULL;
	cgp->rgnSave=NULL;
	cgp->polySave=NULL;
	cgp->grafProcs=NULL;
  // So the struct is a both a lgeal GrafPort AND CGrafPort
  //__initGrafPort((GrafPort *)cgp);
}


// NOTE: Only inits, does NOT fill in valid values!
// ...but for now, set to old classic mac screen size...
void __initGDevice(GDevice *gd)
{
  fprintf(stderr,"(__initGDevice)\n");
  if(!gd) { fprintf(stderr,"__initGDevice() gd is NULL!\n"); exit(EXIT_FAILURE); }
  if(theMacGDeviceInited) {
    fprintf(stderr,"(theMacGDeviceInited already set!)\n");
    return;
  }
  memset(gd,0,sizeof(GDevice));
  gd->gdRefNum=1;
  gd->gdID=1;
  gd->gdType=0; // is this index vs direct?
  gd->gdITable=NULL;
  gd->gdResPref=1; // ???
  gd->gdSearchProc=NULL;
  gd->gdCompProc=NULL;
  gd->gdFlags=1; // is this index vs direct?
  gd->gdRefCon=0;
  gd->gdNextGD=NULL;
  //
  gd->gdRect.top=0;
  gd->gdRect.left=0;
  gd->gdRect.right=0;
  gd->gdRect.bottom=0;
  __rectSetDefault(&gd->gdRect);
  //
  if(the_cross_display) { gd->gdRect.right=the_cross_display->width; gd->gdRect.bottom=the_cross_display->height; }
  else fprintf(stderr,"no the_cross_display!!!\n");
  if(the_cross_window) { gd->gdRect.right=the_cross_window->width; gd->gdRect.bottom=the_cross_window->height; }
  else fprintf(stderr,"no the_cross_window!!!\n");
  //
#ifdef DEBUG
  __printrect("__initGDevice gd->gdRect",&gd->gdRect);
#endif
  unsigned int d=32;  // BAD BAD BAD BAD
  /*
  PixMapPtr *pm=(PixMapPtr *)malloc(sizeof(PixMapPtr));  
  *pm=__newPixMap(boundsRect,PixelDepth);
  gd->gdPMap=pm;
  */
  gd->gdPMap=NULL;
  //
  gd->gdMode=1; // is this index vs direct?
  gd->gdCCBytes=0;
  gd->gdCCDepth=0;
  gd->gdCCXData=NULL;
  gd->gdCCXMask=NULL;
  gd->gdReserved=0;
  theMacGDeviceInited=TRUE;
}


void __printrect(const char *label,const Rect *r)
{
//#ifdef DEBUG
  //fprintf(stderr,"(__printrect)\n");
  if(!label) { fprintf(stderr,"__printrect label is NULL!\n"); exit(EXIT_FAILURE); }
  if(!r) { fprintf(stderr,"__printrect r is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"%s Rect is %d,%d,%d,%d\n",label,r->top,r->bottom,r->left,r->right);
//#endif
}


// Set some random rect to sane values...
void __rectSetDefault(Rect *r)
{
#ifdef DEBUG
  fprintf(stderr,"(__rectSetDefault)\n");
#endif
  if(!r) { fprintf(stderr,"__rectSetDefault r is NULL!\n"); exit(EXIT_FAILURE); }
  r->top=0;
  r->bottom=0;
  //if(the_cross_window) r->bottom=the_cross_window->height; else r->bottom=384;
  r->left=0;
  if(the_cross_window) r->right=the_cross_window->width; else r->right=512;
  r->right=0;
  ////__printrect("__rectSetDefault r",r);
}


PixMapPtr __newPixMap(const Rect *r,unsigned int d)
{
  if(!r) { fprintf(stderr,"__newPixMap r is NULL!\n"); exit(EXIT_FAILURE); }
  PixMapPtr p=(PixMapPtr)malloc(sizeof(PixMap));
  if(!p) { fprintf(stderr,"__newPixMap p is NULL!\n"); exit(EXIT_FAILURE); }
#ifdef DEBUG
  fprintf(stderr,"(__newPixMap)\n");
#endif
  __initPixMap(p);
  p->bounds.left=r->left;
  p->bounds.right=r->right;
  p->bounds.top=r->top;
  p->bounds.bottom=r->bottom;
  unsigned int w=r->right-r->left;
  unsigned int h=r->bottom-r->top;
  //
  // TODO: Much more to be filled in here...
  //
  p->pixelSize=d;
  switch(d) {
    case 1: p->rowBytes=w/8; break;
    case 2: p->rowBytes=w/4; break;
    case 4: p->rowBytes=w/2; break;
    case 8: p->rowBytes=w; break;
    case 16: p->rowBytes=w*1; break;
    case 24: p->rowBytes=w*3; break;
    case 32: p->rowBytes=w*4; break;
    default:
      fprintf(stderr,"__newPixMap bad d!\n"); 
      exit(EXIT_FAILURE);
      break;
  }
  unsigned int s=p->rowBytes*h;
  p->packSize=s;
  p->baseAddr=(Ptr)malloc(s);
  if(!p->baseAddr) { fprintf(stderr,"__newPixMap p->baseAddr is NULL!\n"); exit(EXIT_FAILURE); }
  return p;
}


////////////////////////////////////////////////////////////////////////////////
// Quickdraw
////////////////////////////////////////////////////////////////////////////////


_SH_FUNK void InitGraf(void *globalPtr)
{
  fprintf(stderr,"(InitGraf)\n");
  if(!globalPtr) { fprintf(stderr,"InitGraf() globalPtr is NULL!\n"); exit(EXIT_FAILURE); }
  open_libs(0,NULL);
  if(!the_cross_display) the_cross_display=open_cross_display(NULL,0,0,0);
  if(the_cross_display) {
    __init_QDGlobals(&qd);
    //
    qd.screenBits.bounds.top=0;
    qd.screenBits.bounds.bottom=the_cross_display->height;
    qd.screenBits.bounds.left=0;
    qd.screenBits.bounds.right=the_cross_display->width;
    //
    //__printrect("InitGraf screenbits",&qd.screenBits.bounds);
  }
#ifdef REAL_QD_GLOBALS
  thePort=(GrafPtr)malloc(sizeof(CGrafPort));
  __initCGrafPort((CGrafPtr)thePort);
  qd.thePort=(GrafPtr)thePort;
#else
  qd.thePort=(GrafPtr)malloc(sizeof(CGrafPort));
  __initCGrafPort((CGrafPtr)qd.thePort);
#endif
  if(!qd.thePort) { fprintf(stderr,"InitGraf() thePort is NULL!\n"); exit(EXIT_FAILURE); }
  //
#ifdef REAL_QD_GLOBALS
  // TODO: Fill out ALL globals...
  __sync_QDGlobals(&qd);
#endif
}


_SH_FUNK void InitCursor()
{
  fprintf(stderr,"(InitCursor)\n");
  open_libs(0,NULL);
  // NOTE: Ok to skip...
}


_SH_FUNK short QDError()
{
  fprintf(stderr,"(QDError)\n");
  return qderror;
}


// NOTE: This probably isn't right...is color really an rgb?
_SH_FUNK void BackColor(long color)
{
  fprintf(stderr,"(BackColor)\n");
  //fprintf(stderr,"BackColor() color is "<<color<<".\n");
  set_backgroundc(the_cross_window,color);
}


_SH_FUNK void CopyBits(const BitMap *srcBits, const BitMap *dstBits, const Rect *srcRect, const Rect *dstRect, short mode, RgnHandle maskRgn)
{
  fprintf(stderr,"(CopyBits)\n");
  if(!srcBits) { fprintf(stderr,"CopyBits() srcBits is NULL!\n"); exit(EXIT_FAILURE); }
  if(!dstBits) { fprintf(stderr,"CopyBits() dstBits is NULL!\n"); exit(EXIT_FAILURE); }
  if(!srcRect) { fprintf(stderr,"CopyBits() srcRect is NULL!\n"); exit(EXIT_FAILURE); }
  if(!dstRect) { fprintf(stderr,"CopyBits() dstRect is NULL!\n"); exit(EXIT_FAILURE); }
  PixMap *sp=NULL;
  if(__isPixMap(srcBits)) sp=(PixMap *)srcBits;
  else {
    if(__isCGrafPort((GrafPort *)srcBits)) {
    }
  }
  if(!sp) { fprintf(stderr,"CopyBits() sp is NULL!\n"); exit(EXIT_FAILURE); }
  /*
  long srev=((PixMapPtr)srcBits)->pmReserved;
  n_image *src=(n_image *)srev;
  if(!src) { fprintf(stderr,"CopyBits() src n_image is NULL!\n"); return; }
  dump_image(src);
  */
}


_SH_FUNK void DisposeRgn(RgnHandle rgn)
{
  if(!rgn) { fprintf(stderr,"DisposeRgn() rgn is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"DisposeRgn() not implemented!\n");
}


_SH_FUNK void DrawText(const void *textBuf, short firstByte, short byteCount)
{
  fprintf(stderr,"(DrawText)\n");
  if(!textBuf) { fprintf(stderr,"DrawText() textBuf is NULL!\n"); exit(EXIT_FAILURE); }
  draw_text(the_cross_window,shCurX,shCurY,(char *)textBuf);
}


_SH_FUNK void EraseRect(const Rect *r)
{
  if(!r) { fprintf(stderr,"EraseRect() r is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"EraseRect() not implemented!\n");
}


_SH_FUNK void InvertRect(const Rect *r)
{
  if(!r) { fprintf(stderr,"InvertRect() r is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"InvertRect() not implemented!\n");
}


_SH_FUNK void OffsetRect(Rect *r, short dh, short dv)
{
  if(!r) { fprintf(stderr,"OffsetRect() r is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"OffsetRect() not implemented!\n");
}


_SH_FUNK void PaintRect(const Rect *r)
{
  if(!r) { fprintf(stderr,"PaintRect() r is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"PaintRect() not implemented!\n");
}


// NOTE: This probably isn't right...is color really an rgb?
_SH_FUNK void ForeColor(long color)
{
  fprintf(stderr,"(ForeColor)\n");
  //fprintf(stderr,"ForeColor() color is "<<color<<".\n");
  set_foregroundc(the_cross_window,color);
}


_SH_FUNK GDHandle GetGDevice()
{
  fprintf(stderr,"(GetGDevice)\n");
  if(!the_cross_display) the_cross_display=open_cross_display(NULL,0,0,0);
  if(!the_cross_display) { fprintf(stderr,"In GetGDevice, open_cross_display failed!\n"); exit(EXIT_FAILURE); }
  if(!theMacGDevicePtr) {
    theMacGDevicePtr=&theMacGDevice;
    __initGDevice(&theMacGDevice);
    //
    unsigned int np=0;
    unsigned int d=0;
    unsigned int rowBytes=0;
    unsigned int planeSize=0;
    np=the_cross_display->nPlanes;
    d=the_cross_display->depth;
    if(np>1) rowBytes=the_cross_display->width/8;
    else {
      rowBytes=the_cross_display->width*d;
    }
    planeSize=rowBytes*the_cross_display->height;
    //
    theMacGDevice.gdRect.right=the_cross_display->width;
    theMacGDevice.gdRect.bottom=the_cross_display->height;
    //
    if(d>8) theMacGDevice.gdMode=0x80;
    else theMacGDevice.gdMode=0;  // ???
    PixMapPtr *pm=(PixMapPtr *)malloc(sizeof(PixMapPtr));  
    *pm=__newPixMap(&theMacGDevice.gdRect,d);
    theMacGDevicePtr->gdPMap=pm;
 }
  return &theMacGDevicePtr;
}


_SH_FUNK void GlobalToLocal(Point *pt)
{
  fprintf(stderr,"(GlobalToLocal)\n");
  if(!pt) { fprintf(stderr,"GlobalToLocal() pt is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"GlobalToLocal() not implemented!\n");
}


_SH_FUNK void Index2Color(long index, RGBColor *aColor)
{
  if(aColor) { fprintf(stderr,"Index2Color() aColor is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"Index2Color() not implemented!\n");
  exit(EXIT_FAILURE);
}


// How is this different from Line?
_SH_FUNK void LineTo(short h, short v)
{
  fprintf(stderr,"(LineTo)\n");
  //fprintf(stderr,"LineTo() line will be "<<shCurX<<","<<shCurY<<" to "<<h<<","<<v<<".\n");
  default_draw_line(the_cross_window,shCurX,shCurY,h,v);
  shCurX=h; shCurY=v;
}


_SH_FUNK void MoveTo(short h, short v)
{
  fprintf(stderr,"(MoveTo)\n");
  //fprintf(stderr,"MoveTo() h,v is "<<h<<","<<v<<".\n");
  shCurX=h; shCurY=v;
}


_SH_FUNK RgnHandle NewRgn()
{
  fprintf(stderr,"NewRgn() not implemented!\n"); 
  return NULL;
}


_SH_FUNK void PenMode(short mode)
{
  fprintf(stderr,"PenMode() not implemented!\n");
}


_SH_FUNK void RGBBackColor(const RGBColor *color)
{
  fprintf(stderr,"(RGBBackColor)\n");
  if(!color) { fprintf(stderr,"RGBBackColor() color is NULL!\n"); exit(EXIT_FAILURE); }
  unsigned int red=(color->red)>>8;
  unsigned int green=(color->green)>>8;
  unsigned int blue=(color->blue)>>8;
  unsigned long theColor=(red<<16)+(blue<<8)+green;
  //fprintf(stderr,"RGBBackColor() rgb is "<<red<<","<<green<<","<<blue<<".\n");
  //fprintf(stderr,"RGBBackColor() theColor is "<<theColor<<".\n");
  //fprintf(stderr,"theColor is "); debug_hexl(theColor); debug_nl();
  //set_background(the_cross_window,red,green,blue);
  set_backgroundc(the_cross_window,theColor);
}


_SH_FUNK void RGBForeColor(const RGBColor *color)
{
  fprintf(stderr,"(RGBForeColor)\n");
  if(!color) { fprintf(stderr,"RGBForeColor() color is NULL!\n"); exit(EXIT_FAILURE); }
  unsigned int red=(color->red)>>8;
  unsigned int green=(color->green)>>8;
  unsigned int blue=(color->blue)>>8;
  unsigned long theColor=(red<<16)+(blue<<8)+green;
  //fprintf(stderr,"RGBForeColor() rgb is "<<red<<","<<green<<","<<blue<<".\n");
  //fprintf(stderr,"RGBForeColor() theColor is "<<theColor<<".\n");
  //fprintf(stderr,"theColor is "); debug_hexl(theColor); debug_nl();
  //set_foreground(the_cross_window,red,green,blue);
  set_foregroundc(the_cross_window,theColor);
}


_SH_FUNK void ScrollRect(const Rect *r, short dh, short dv, RgnHandle updateRgn)
{
  fprintf(stderr,"(ScrollRect)\n");
  if(!r) { fprintf(stderr,"ScrollRect() r is NULL!\n"); exit(EXIT_FAILURE); }
  //__printrect("ScrollRect",r);
  fprintf(stderr,"ScrollRect() not implemented!\n");
}


_SH_FUNK void SetEntries(short start, short count, CSpecArray aTable)
{
  fprintf(stderr,"SetEntries() not implemented!\n");
}


// We only support a single port...
_SH_FUNK void SetPort(GrafPtr port)
{
  fprintf(stderr,"(SetPort)\n");
  if(!port) { fprintf(stderr,"SetPort() port is NULL!\n"); exit(EXIT_FAILURE); }
#ifdef REAL_QD_GLOBALS
  thePort=port;
#else
  qd.thePort=port;
#endif
}


_SH_FUNK void SetRect(Rect *r, short left, short top, short right, short bottom)
{
  fprintf(stderr,"(SetRect)\n");
  if(!r) { fprintf(stderr,"SetRect() r is NULL!\n"); exit(EXIT_FAILURE); }
  //fprintf(stderr,"SetRect() left,top,right,bottom is "<<left<<","<<top<<","<<right<<","<<bottom<<".\n");
  r->left=left; r->top=top; r->right=right; r->bottom=bottom;
  ////__printrect("SetRect",r);
}


_SH_FUNK void TextFont(short font)
{
  fprintf(stderr,"(TextFont)\n");
#ifdef REAL_QD_GLOBALS
  thePort->txFont=font;
#else
  qd.thePort->txFont=font;
#endif
}


_SH_FUNK void TextMode(short mode)
{
  fprintf(stderr,"(TextMode)\n");
#ifdef REAL_QD_GLOBALS
  thePort->txMode=mode;
#else
  qd.thePort->txMode=mode;
#endif
}


_SH_FUNK void TextSize(short size)
{
  fprintf(stderr,"(TextSize)\n");
#ifdef REAL_QD_GLOBALS
  thePort->txSize=size;
#else
  qd.thePort->txSize=size;
#endif
}


_SH_FUNK short TextWidth(const void *textBuf, short firstByte, short byteCount)
{
  fprintf(stderr,"(TextWidth)\n");
  if(!textBuf) { fprintf(stderr,"TextWidth() textBuf is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"TextWidth() not implemented!\n");
  return 0;
}


_SH_FUNK void GetPort(GrafPtr *port)
{
  if(!port) { fprintf(stderr,"GetPort() port is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"GetPort() not implemented!\n");
}


_SH_FUNK long GetCTSeed(void)
{
  fprintf(stderr,"(GetCTSeed)\n");
  // TODO: Is this good enough?
  return rand();
}


_SH_FUNK PicHandle GetPicture(short pictureID)
{
  fprintf(stderr,"GetPicture() not implemented!\n");
  return 0;
}


_SH_FUNK CTabHandle GetCTable(short ctID)
{
  fprintf(stderr,"GetCTable() not implemented!\n");
  return 0;
}


// How is this different from LineTo?
_SH_FUNK void Line(short dh, short dv)
{
  fprintf(stderr,"(Line)\n");
  default_draw_line(the_cross_window,shCurX,shCurY,dh,dv);
  shCurX=dh; shCurY=dv;
}


_SH_FUNK void PenPat(const Pattern *pat)
{
  fprintf(stderr,"(PenPat)\n");
  if(!pat) { fprintf(stderr,"PenPat() pat is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"PenPat() not implemented!\n");
}


_SH_FUNK void ShowCursor(void)
{
  fprintf(stderr,"(ShowCursor)\n");
  // do nothing...
}


_SH_FUNK void HideCursor(void)
{
  fprintf(stderr,"(HideCursor)\n");
  // do nothing...
}


_SH_FUNK void SetCursor(const Cursor *crsr)
{
  fprintf(stderr,"(SetCursor)\n");
  // do nothing...
}


_SH_FUNK CursHandle GetCursor(short cursorID)
{
  fprintf(stderr,"GetCursor() not implemented!\n");
  return NULL;
}


_SH_FUNK void BackPat(const Pattern *pat)
{
  if(!pat) { fprintf(stderr,"BackPat() pat is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"BackPat() not implemented!\n");
}


_SH_FUNK Boolean GetPixel(short h, short v)
{
  fprintf(stderr,"GetPixel() not implemented!\n");
  return FALSE;
}


_SH_FUNK void DisposeCTable(CTabHandle cTable)
{
  fprintf(stderr,"DisposeCTable() not implemented!\n");
}


// I think this is right...
_SH_FUNK void InsetRect(Rect *r, short dh, short dv)
{
  fprintf(stderr,"(InsetRect)\n");
  if(!r) { fprintf(stderr,"InsetRect() r is NULL!\n"); exit(EXIT_FAILURE); }
  //__printrect("InsetRect pre",r);
  r->top=r->top+dh;
  r->bottom=r->bottom-dh;
  r->left=r->left+dv;
  r->right=r->right-dv;
  //fprintf(stderr,"InsetRect() h,v is "<<dh<<","<<dv<<".\n");
  //__printrect("InsetRect post",r);
}


_SH_FUNK short Random(void)
{
  fprintf(stderr,"(Random)\n");
  int t=rand();
  // TODO: Don't know if this is right...
  t=t&0xffff;
  t=t-32767;
  return t;
}


// TODO: Just a circle for now...this should be an oval and should be filled
_SH_FUNK void PaintOval(const Rect *r)
{
  fprintf(stderr,"(PaintOval)\n");
  if(!r) { fprintf(stderr,"PaintOval() r is NULL!\n"); exit(EXIT_FAILURE); }
  unsigned int radius=r->right-r->left;
  unsigned int x=r->left+(radius/2);
  unsigned int y=r->top+(radius/2);
  /*
  default_draw_line(the_cross_window,r->left,r->top,r->right,r->top);
  default_draw_line(the_cross_window,r->right,r->top,r->right,r->bottom);
  default_draw_line(the_cross_window,r->right,r->bottom,r->left,r->bottom);
  default_draw_line(the_cross_window,r->left,r->bottom,r->left,r->top);
  */
  default_draw_circle(the_cross_window,x,y,radius);
  shCurX=r->left; shCurY=r->top;  //  Is this right or should it be the center?
}


// Is this right?
_SH_FUNK void InvertColor(RGBColor *myColor)
{
  fprintf(stderr,"(InvertColor)\n");
  if(!myColor) { fprintf(stderr,"InvertColor() myColor is NULL!\n"); exit(EXIT_FAILURE); }
  myColor->red=65535-myColor->red;
  myColor->green=65535-myColor->green;
  myColor->blue=65535-myColor->blue;
}


_SH_FUNK void DrawString(ConstStr255Param s)
{
  fprintf(stderr,"(DrawString)\n");
  // TODO: Process s in case it really is a pascal string!
  draw_text(the_cross_window,shCurX,shCurY,(char *)(s+1));
}


_SH_FUNK void GetGWorld(CGrafPtr *port, GDHandle *gdh)
{
  fprintf(stderr,"(GetGWorld)\n");
  if(!port) { fprintf(stderr,"GetGWorld() port is NULL!\n"); exit(EXIT_FAILURE); }
  if(!gdh) { fprintf(stderr,"GetGWorld() gdh is NULL!\n"); exit(EXIT_FAILURE); }
  if(!theMacGDevicePtr) {
    theMacGDevicePtr=&theMacGDevice;
    __initGDevice(&theMacGDevice);
    GetGDevice();
  }
  *gdh=&theMacGDevicePtr;
#ifdef REAL_QD_GLOBALS
  *port=(CGrafPtr)thePort;
#else
  *port=(CGrafPtr)qd.thePort;
#endif
}


_SH_FUNK void InvalRgn() 
{ 
  fprintf(stderr,"InvalRgn() not implemented!\n"); 
}


_SH_FUNK Ptr GetPixBaseAddr(PixMapHandle pm) 
{ 
  fprintf(stderr,"(GetGWorld)\n");
  // TODO: Does this work?  Is it valid?  Do we actually care?
  return (*pm)->baseAddr;
}


_SH_FUNK void InvalRect() 
{ 
  fprintf(stderr,"InvalRect() not implemented!\n"); 
}


_SH_FUNK Boolean LockPixels(PixMapHandle pm) 
{ 
  //fprintf(stderr,"LockPixels() not implemented!\n");
  return true;
}


_SH_FUNK void UnlockPixels(PixMapHandle pm) 
{ 
  //fprintf(stderr,"UnlockPixels() not implemented!\n");
}


_SH_FUNK void PenNormal() 
{ 
  fprintf(stderr,"PenNormal() not implemented!\n"); 
}


_SH_FUNK void ValidRect() 
{ 
  fprintf(stderr,"ValidRect() not implemented!\n"); 
}


_SH_FUNK void LocalToGlobal(Point *pt) 
{ 
  if(!pt) { fprintf(stderr,"LocalToGlobal() pt is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"LocalToGlobal() not implemented!\n"); 
}


_SH_FUNK Boolean PtInRect(Point pt, const Rect *r) 
{ 
  if(!r) { fprintf(stderr,"PtInRect() r is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"PtInRect() not implemented!\n"); 
  return FALSE; 
}


_SH_FUNK void OffsetRgn(RgnHandle rgn, short dh, short dv) 
{ 
  if(!rgn) { fprintf(stderr,"OffsetRgn() port is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"OffsetRgn() not implemented!\n"); 
}


_SH_FUNK Boolean SectRect(const Rect *src1, const Rect *src2, Rect *dstRect) 
{
  if(!src1) { fprintf(stderr,"SectRect() src1 is NULL!\n"); exit(EXIT_FAILURE); }
  if(!src2) { fprintf(stderr,"SectRect() src2 is NULL!\n"); exit(EXIT_FAILURE); }
  if(!dstRect) { fprintf(stderr,"SectRect() dstRect is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"SectRect() not implemented!\n"); 
  return FALSE; 
}


_SH_FUNK void ClipRect(const Rect *r) 
{ 
  if(!r) { fprintf(stderr,"ClipRect() r is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"ClipRect() not implemented!\n"); 
}


_SH_FUNK void CopyRgn(RgnHandle srcRgn, RgnHandle dstRgn) 
{
  if(!srcRgn) { fprintf(stderr,"CopyRgn() srcRgn is NULL!\n"); exit(EXIT_FAILURE); }
  if(!dstRgn) { fprintf(stderr,"CopyRgn() dstRgn is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"CopyRgn() not implemented!\n"); 
}


_SH_FUNK void DiffRgn(RgnHandle srcRgnA, RgnHandle srcRgnB, RgnHandle dstRgn) 
{ 
  if(!srcRgnA) { fprintf(stderr,"DiffRgn() srcRgnA is NULL!\n"); exit(EXIT_FAILURE); }
  if(!srcRgnB) { fprintf(stderr,"DiffRgn() srcRgnB is NULL!\n"); exit(EXIT_FAILURE); }
  if(!dstRgn) { fprintf(stderr,"DiffRgn() dstRgn is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"DiffRgn() not implemented!\n"); 
}


_SH_FUNK Boolean EmptyRgn(RgnHandle rgn) 
{ 
  if(!rgn) { fprintf(stderr,"EmptyRgn() rgn is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"EmptyRgn() not implemented!\n"); 
  return FALSE; 
}


_SH_FUNK Boolean PtInRgn(Point pt, RgnHandle rgn) 
{ 
  if(!rgn) { fprintf(stderr,"PtInRgn() rgn is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"PtInRgn() not implemented!\n"); 
  return FALSE; 
}


_SH_FUNK void RectRgn(RgnHandle rgn, const Rect *r)
{
  if(!rgn) { fprintf(stderr,"RectRgn() rgn is NULL!\n"); exit(EXIT_FAILURE); }
  if(!r) { fprintf(stderr,"RectRgn() r is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"RectRgn() not implemented!\n");
}


_SH_FUNK void SectRgn(RgnHandle srcRgnA, RgnHandle srcRgnB, RgnHandle dstRgn)
{
  if(!srcRgnA) { fprintf(stderr,"SectRgn() port is NULL!\n"); exit(EXIT_FAILURE); }
  if(!srcRgnB) { fprintf(stderr,"SectRgn() srcRgnB is NULL!\n"); exit(EXIT_FAILURE); }
  if(!dstRgn) { fprintf(stderr,"SectRgn() dstRgn is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"SectRgn() not implemented!\n");
}


_SH_FUNK void SetOrigin(short h, short v) 
{
  fprintf(stderr,"SetOrigin() not implemented!\n"); 
}


_SH_FUNK void SetPt(Point *pt, short h, short v) 
{
  if(!pt) { fprintf(stderr,"SetPt() pt is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"SetPt() not implemented!\n"); 
}


_SH_FUNK void SetRectRgn(RgnHandle rgn, short left, short top, short right, short bottom) 
{
  if(!rgn) { fprintf(stderr,"SetRectRgn() rgn is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"SetRectRgn() not implemented!\n"); 
}


_SH_FUNK void FrameOval(const Rect *r) 
{
  fprintf(stderr,"(FrameOval)\n");
  if(!r) { fprintf(stderr,"FrameOval() r is NULL!\n"); exit(EXIT_FAILURE); }
  unsigned int radius=r->right-r->left;
  unsigned int x=r->left+(radius/2);
  unsigned int y=r->top+(radius/2);
  /*
  default_draw_line(the_cross_window,r->left,r->top,r->right,r->top);
  default_draw_line(the_cross_window,r->right,r->top,r->right,r->bottom);
  default_draw_line(the_cross_window,r->right,r->bottom,r->left,r->bottom);
  default_draw_line(the_cross_window,r->left,r->bottom,r->left,r->top);
  */
  default_draw_circle(the_cross_window,x,y,radius);
  shCurX=r->left; shCurY=r->top;  //  Is this right or should it be the center?
}


_SH_FUNK void OpenPort(GrafPtr port) 
{
  if(!port) { fprintf(stderr,"OpenPort() port is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"OpenPort() not implemented!\n"); 
}


_SH_FUNK void ClosePort(GrafPtr port) 
{
  if(!port) { fprintf(stderr,"ClosePort() port is NULL!\n"); exit(EXIT_FAILURE); }
  //fprintf(stderr,"ClosePort() not implemented!\n"); 
}


_SH_FUNK void SetPortBits(const BitMap *bm) 
{
  if(!bm) { fprintf(stderr,"SetPortBits() bm is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"SetPortBits() not implemented!\n"); 
}


_SH_FUNK void SaveEntries(CTabHandle srcTable, CTabHandle resultTable, ReqListRec *selection) 
{
  if(!srcTable) { fprintf(stderr,"SaveEntries() srcTable is NULL!\n"); exit(EXIT_FAILURE); }
  if(!resultTable) { fprintf(stderr,"SaveEntries() resultTable is NULL!\n"); exit(EXIT_FAILURE); }
  if(!selection) { fprintf(stderr,"SaveEntries() selection is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"SaveEntries() not implemented!\n"); 
}


_SH_FUNK void RestoreEntries(CTabHandle srcTable, CTabHandle dstTable, ReqListRec *selection)
{
  if(!srcTable) { fprintf(stderr,"RestoreEntries() srcTable is NULL!\n"); exit(EXIT_FAILURE); }
  if(!dstTable) { fprintf(stderr,"RestoreEntries() dstTable is NULL!\n"); exit(EXIT_FAILURE); }
  if(!selection) { fprintf(stderr,"RestoreEntries() selection is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"RestoreEntries() not implemented!\n"); 
}


_SH_FUNK GDevice** GetDeviceList()
{
  fprintf(stderr,"GetDeviceList() not implemented!\n"); 
  return NULL;
}


_SH_FUNK GDevice** GetMainDevice()
{
  fprintf(stderr,"(GetMainDevice)\n");
  // NOTE: theMacGDevicePtr may be NULL...
  return &theMacGDevicePtr;
}


_SH_FUNK GDHandle GetNextDevice(GDHandle curDevice)
{
  if(!curDevice) { fprintf(stderr,"GetNextDevice() curDevice is NULL!\n"); exit(EXIT_FAILURE); }
  //fprintf(stderr,"GetNextDevice() not implemented!\n"); 
  return NULL;
}


_SH_FUNK void SetGDevice(GDHandle gd)
{
  if(!gd) { fprintf(stderr,"SetGDevice() gd is NULL!\n"); exit(EXIT_FAILURE); }
  //fprintf(stderr,"SetGDevice() not implemented!\n"); 
}


_SH_FUNK void CTabChanged(CTabHandle ctab)
{
  if(!ctab) { fprintf(stderr,"CTabChanged() ctab is NULL!\n"); exit(EXIT_FAILURE); }
  //fprintf(stderr,"CTabChanged() not implemented!\n");
}


}  // extern "C"
