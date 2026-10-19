
#include <iostream>

#include <string.h>


extern "C" {


#include <Windows.h>


#include "../crossroads.h"



////////////////////////////////////////////////////////////////////////////////
// globals
////////////////////////////////////////////////////////////////////////////////


// Only a single one for now...
static WindowRef theMacCWindowRef=NULL;
static CWindowRecord theMacCWindow;
static bool theMacCWindowInited=FALSE;

extern void __initPixMap(PixMap *pm);
//extern void __initGrafPort(GrafPort *gp);
extern void __initCGrafPort(CGrafPort *cgp);
extern void __initGDevice(GDevice *gd);
extern void __printrect(const char *label,const Rect *r);


////////////////////////////////////////////////////////////////////////////////
// my Windows internals
////////////////////////////////////////////////////////////////////////////////


// NOTE: Only inits, does NOT fill in valid values!
void __initCWindowRecord(CWindowRecord *cwr)
{
  fprintf(stderr,"(__initCWindowRecord)\n");
  if(theMacCWindowInited) {
    fprintf(stderr,"(__initCWindowRecord already set!)\n");
    return;
  }
  memset(cwr,0,sizeof(CWindowRecord));
	__initCGrafPort(&cwr->port);
	cwr->windowKind=0;
	cwr->visible=true;
	cwr->hilited=true;
	cwr->goAwayFlag=true;
	cwr->spareFlag=false;
	cwr->strucRgn=NULL;
	cwr->contRgn=NULL;
	cwr->updateRgn=NULL;
	cwr->windowDefProc=NULL;
	cwr->dataHandle=NULL;
	cwr->titleHandle=NULL;
	cwr->titleWidth=0;
	cwr->controlList=NULL;
	cwr->nextWindow=NULL;
	cwr->windowPic=NULL;
	cwr->refCon=0;
  theMacCWindowInited=TRUE;
}


////////////////////////////////////////////////////////////////////////////////
// Windows
////////////////////////////////////////////////////////////////////////////////


void InitWindows() {
  fprintf(stderr,"(InitWindows)\n");
  open_libs(0,NULL);
}


WindowRef NewWindow(void *wStorage, const Rect *boundsRect, ConstStr255Param title, Boolean visible, short theProc, WindowRef behind, Boolean goAwayFlag, long refCon)
{
  fprintf(stderr,"(NewWindow)\n");
  if(!boundsRect) { fprintf(stderr,"NewWindow() boundsRect is NULL!\n"); exit(EXIT_FAILURE); }
  unsigned int w,h;
  open_libs(0,NULL);  // Because maybe they didn't call InitWindows, or even InitGraf...
  //
  __printrect("boundsRect",boundsRect);
  w=boundsRect->right-boundsRect->left;
  if(!w) w=the_cross_display->width;
  h=boundsRect->bottom-boundsRect->top;
  if(!h) w=the_cross_display->height;
  //
  the_cross_window=open_cross_window(the_cross_display,(char *)title+1,w,h);
  if(!the_cross_window) { fprintf(stderr,"In NewWindow, open_cross_window failed!\n"); exit(EXIT_FAILURE); }
  //the_cross_display=the_cross_window->parent;
  //
  __initCWindowRecord(&theMacCWindow);
  theMacCWindowRef=(WindowRef)&theMacCWindow;  // Is this type coercion ok?
  //
  unsigned int np=0;
  unsigned int d=0;
  unsigned int rowBytes=0;
  unsigned int planeSize=0;
  np=the_cross_display->nPlanes;
  d=the_cross_display->depth;
  if(np>1) rowBytes=the_cross_window->width/8;
  else {
    rowBytes=the_cross_window->width*d;
  }
  planeSize=rowBytes*the_cross_window->height;
  //
  CGrafPort *theWinPort=&theMacCWindow.port;
  theWinPort->portRect.right=the_cross_window->width;
  theWinPort->portRect.bottom=the_cross_window->height;
  __printrect("theMacCWindow.port.portRect",&theWinPort->portRect);
  PixMapPtr mypmp=NULL;
  if(theMacCWindow.port.portPixMap) {
    mypmp=*theMacCWindow.port.portPixMap;
    mypmp->bounds.right=the_cross_window->width;
    mypmp->bounds.bottom=the_cross_window->height;
    mypmp->rowBytes=rowBytes;
    mypmp->planeBytes=planeSize;
    mypmp->pixelSize=d;
    __printrect("mypmp->bounds",&mypmp->bounds);
  }
  //
  fprintf(stderr,"NewWindow done.\n");
  return theMacCWindowRef;
}


WindowRef NewCWindow(void *wStorage, const Rect *boundsRect, ConstStr255Param title, Boolean visible, short procID, WindowRef behind, Boolean goAwayFlag, long refCon)
{
  fprintf(stderr,"(NewCWindow)\n");
  // NOTE: Mixing procID and theProc...
  return NewWindow(wStorage,boundsRect,title,visible,procID,behind,goAwayFlag,refCon);
}


void DisposeWindow(WindowRef theWindow)
{
  if(!theWindow) { fprintf(stderr,"DisposeWindow() theWindow is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"DisposeWindow() not implemented!\n");
}


void DragWindow(WindowRef theWindow, Point startPt, const Rect *boundsRect)
{
  if(!theWindow) { fprintf(stderr,"DragWindow() theWindow is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"DragWindow() not implemented!\n");
}


short FindWindow(Point thePoint, WindowRef *theWindow)
{
  if(!theWindow) { fprintf(stderr,"FindWindow() theWindow is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"FindWindow() not implemented!\n");
  return 0;
}


WindowRef FrontWindow()
{
  //fprintf(stderr,"FrontWindow() not implemented!\n");
  return theMacCWindowRef;
}


long GrowWindow(WindowRef theWindow, Point startPt, const Rect *bBox)
{
  if(!theWindow) { fprintf(stderr,"GrowWindow() theWindow is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"GrowWindow() not implemented!\n");
  return 0;
}


void MoveWindow(WindowRef theWindow, short hGlobal, short vGlobal, Boolean front)
{
  if(!theWindow) { fprintf(stderr,"MoveWindow() theWindow is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"MoveWindow() not implemented!\n");
}


void SelectWindow(WindowRef theWindow)
{
  if(!theWindow) { fprintf(stderr,"SelectWindow() theWindow is NULL!\n"); exit(EXIT_FAILURE); }
  //fprintf(stderr,"SelectWindow() not implemented!\n");
}


void ShowWindow(WindowRef theWindow)
{
  if(!theWindow) { fprintf(stderr,"ShowWindow() theWindow is NULL!\n"); exit(EXIT_FAILURE); }
  //fprintf(stderr,"ShowWindow() not implemented!\n");
}


void SizeWindow(WindowRef theWindow, short w, short h, Boolean fUpdate)
{
  if(!theWindow) { fprintf(stderr,"SizeWindow() theWindow is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"SizeWindow() not implemented!\n");
}


void ZoomWindow(WindowRef theWindow, short partCode, Boolean front)
{
  if(!theWindow) { fprintf(stderr,"ZoomWindow() theWindow is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"ZoomWindow() not implemented!\n");
}


void BeginUpdate(WindowRef theWindow)
{
  if(!theWindow) { fprintf(stderr,"BeginUpdate() theWindow is NULL!\n"); exit(EXIT_FAILURE); }
  //fprintf(stderr,"BeginUpdate() not implemented!\n");
}


void DrawGrowIcon(WindowRef theWindow)
{
  fprintf(stderr,"DrawGrowIcon() not implemented!\n");
  // ok to skip
}


void EndUpdate(WindowRef theWindow)
{
  if(!theWindow) { fprintf(stderr,"EndUpdate() theWindow is NULL!\n"); exit(EXIT_FAILURE); }
  //fprintf(stderr,"EndUpdate() not implemented!\n");
}


Boolean TrackBox(WindowRef theWindow, Point thePt, short partCode)
{
  if(!theWindow) { fprintf(stderr,"TrackBox() theWindow is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"TrackBox() not implemented!\n");
  return FALSE;
}


Boolean TrackGoAway(WindowRef theWindow, Point thePt)
{
  if(!theWindow) { fprintf(stderr,"TrackGoAway() theWindow is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"TrackGoAway() not implemented!\n");
  return FALSE;
}


WindowRef GetNewCWindow(short windowID, void *wStorage, WindowRef behind)
{
  fprintf(stderr,"GetNewCWindow() not implemented!\n");
  return NULL;
}


WindowRef GetNewWindow(short windowID, void *wStorage, WindowRef behind)
{
  fprintf(stderr,"GetNewWindow() not implemented!\n");
  return NULL;
}


void CloseWindow(WindowRef theWindow)
{
  //fprintf(stderr,"CloseWindow() not implemented!\n");
}


UInt32 GetCaretTime(void)
{
  fprintf(stderr,"GetCaretTime() not implemented!\n");
  return 0;
}


}
