
// Assume for now...
#define BUILDING_FOR_CARBON 1


#include "stdio.h"
#include "stdlib.h"


//#ifdef BUILDING_FOR_CARBON
//#include "Carbon/Carbon.h"
//#else
#include "MacTypes.h"
#include "Memory.h"
#include "Quickdraw.h"
#include "Fonts.h"
#include "Events.h"
#include "Menus.h"
#include "Windows.h"
#include "TextEdit.h"
#include "Dialogs.h"
#include "Processes.h"
#include "Sound.h"
//#endif


/* may be needed dependent on compiler */ 
extern QDGlobals qd;


#define nikdrawGetRGBRed24(x)   ((unsigned int)((x >> 16)&0xff))
#define nikdrawGetRGBGreen24(x) ((unsigned int)((x >>8)&0xff))
#define nikdrawGetRGBBlue24(x)  ((unsigned int)(x&0xff))


Rect theWindowRect;


//extern void myCarbon_printrect(const char *label,const Rect *r);


// Which header is it in?
extern void ExitToShell(void);


////////////////////////////////////////////////////////////////////////////////
// Draw something
////////////////////////////////////////////////////////////////////////////////

void drawMoire(void);

void drawMoire(void)
{
  unsigned int t;
  unsigned int inc=1;
  unsigned long c;
  unsigned long numColors=0x00ffffff;
  RGBColor lineColor;
  int trueColor=0;  // Assumed...
  // TODO: clear window here
  unsigned int theWidth=theWindowRect.right;
  unsigned int theHeight=theWindowRect.bottom;
  if(trueColor) inc=(int)(numColors/theWidth);
  c=0;
  for(t=0;t < theWidth;t++) {
    lineColor.red=nikdrawGetRGBRed24(c) << 8;
    lineColor.green=nikdrawGetRGBGreen24(c) << 8;
    lineColor.blue=nikdrawGetRGBBlue24(c) << 8;
    RGBForeColor(&lineColor);
    c=c+inc;
    if(c > numColors) c=0;
    MoveTo(t,0);
    LineTo(theWidth-t,theHeight);
  }
  if(trueColor) inc=(int)(numColors/theHeight);
  c=0;
  for(t=0;t < theHeight;t++) {
    lineColor.red=nikdrawGetRGBRed24(c) << 8;
    lineColor.green=nikdrawGetRGBGreen24(c) << 8;
    lineColor.blue=nikdrawGetRGBBlue24(c) << 8;
    RGBForeColor(&lineColor);
    c=c+inc;
    if(c > numColors) c=0;
    MoveTo(0,t);
    LineTo(theWidth,theHeight-t);
  }
}


////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
  WindowPtr  theWindowPtr;
  OSErr error;
  SysEnvRec  env;
  int running=1;
  EventRecord event;
  int type,val;
  //
  error=SysEnvirons(1,&env);
  if(!env.hasColorQD) { SysBeep(50); ExitToShell(); }
  InitGraf(&qd.thePort);
  InitFonts();
  InitWindows();
  InitMenus();
  InitDialogs(nil);
  InitCursor();
  theWindowRect=qd.screenBits.bounds;
  //myCarbon_printrect("theWindowRect",&theWindowRect);
  theWindowPtr=NewCWindow(
    nil,
    &theWindowRect,
    "\pmacMoire",
    true,
    documentProc,
    (WindowPtr)-1,
    false,
    0
  );    
  SetPort(theWindowPtr);
  drawMoire();
  // TODO: Should have an actual event loop here!
  while(running) {
    GetNextEvent(everyEvent,&event);
    type=event.what;
    switch(type) {
      case nullEvent: break;
	  case mouseDown:
	  case mouseUp:
	  case keyDown:
	  case keyUp:
	    running=false;
	    break;
	  case updateEvt:
	  case activateEvt:
            printf("Going to draw...)\n");
            drawMoire();
	    break;
	  default:
	    printf("mac event.what=%d skipped!\n",type);
	    break;
	}
  }
  return 0;
}
