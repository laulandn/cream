
#include <iostream>

#include <stdlib.h>


extern "C" {


#include <Events.h>


#include "../crossroads.h"


////////////////////////////////////////////////////////////////////////////////
// globals
////////////////////////////////////////////////////////////////////////////////


extern cross_window *the_cross_window;


////////////////////////////////////////////////////////////////////////////////
// my Event internals
////////////////////////////////////////////////////////////////////////////////


void mynlib2mac_event(n_event *nlibEvent, EventRecord *macEvent) {
  fprintf(stderr,"(SysEnvirons)\n");
  if(!nlibEvent) { fprintf(stderr,"mynlib2mac_event() nlibEvent is NULL!\n"); exit(EXIT_FAILURE); }
  if(!macEvent) { fprintf(stderr,"mynlib2mac_event() macEvent is NULL!\n"); exit(EXIT_FAILURE); }
  /*
  switch(nlibEvent->type) {
    case N_EVENT_NONE:
      macEvent->what=nullEvent;
      break;
    case N_EVENT_UNKNOWN:
      macEvent->what=nullEvent;
      break;
    case N_EVENT_CLICK:
      macEvent->what=mouseUp;
      // TODO: Doesn't set x,y, etc!
      break;
    case N_EVENT_KEYPRESS:
      macEvent->what=keyUp;
      // TODO: Doesn't set code, etc!
      break;
    case N_EVENT_MOVED:
      macEvent->what=updateEvt;
      break;
    case N_EVENT_REDRAW:
      macEvent->what=updateEvt;
      break;
    case N_EVENT_FOCUS:
      // NOTE: Need to set something to say this is an activate (not a deactivate)
      macEvent->what=activateEvt;
      // TODO: Update thw window rect?
      break;
    case N_EVENT_UNFOCUS:
      // NOTE: Need to set something to say this is an deactivate (not a activate)
      macEvent->what=activateEvt;
      break;
    case N_EVENT_RESIZE:
      macEvent->what=updateEvt;
      // TODO: Update thw window rect!
      break;
    default:
      fprintf(stderr,"WaitNextEvent() unhandled event type!\n");
      dump_event(nlibEvent);
      break;
  }
  */
  macEvent->message=0;
  macEvent->when=0;
  macEvent->where.h=nlibEvent->x;  macEvent->where.v=nlibEvent->y;
  macEvent->modifiers=0;
}


////////////////////////////////////////////////////////////////////////////////
// Events
////////////////////////////////////////////////////////////////////////////////


// NOTE: eventMask is ignored...
Boolean GetNextEvent(EventMask eventMask, EventRecord *theEvent)
{
  fprintf(stderr,"(GetNextEvent)\n");
  if(!the_cross_window) {
    fprintf(stderr,"GetNextEvent() the_cross_window is NULL!\n");
    return false;
    //exit(EXIT_FAILURE);
  }
  if(!theEvent) { fprintf(stderr,"GetNextEvent() theEvent is NULL!\n"); exit(EXIT_FAILURE); }
  n_event *myEvent=get_next_event(the_cross_window);
  if(myEvent) {
    mynlib2mac_event(myEvent,theEvent);
    return true;
  }
  return false;
}


// NOTE: eventMask is ignored...
Boolean WaitNextEvent(EventMask eventMask, EventRecord *theEvent, UInt32 sleep, RgnHandle mouseRgn)
{
  fprintf(stderr,"(WaitNextEvent)\n");
  if(!the_cross_window) { fprintf(stderr,"WaitNextEvent() the_cross_window is NULL!\n"); exit(EXIT_FAILURE); }
  if(!theEvent) { fprintf(stderr,"WaitNextEvent() theEvent is NULL!\n"); exit(EXIT_FAILURE); }
  struct n_event *myevent=NULL;
  myevent=wait_event(the_cross_window);
  mynlib2mac_event(myevent,theEvent);
  return false;
}


// NOTE: eventMask is ignored...
Boolean EventAvail(EventMask eventMask, EventRecord *theEvent)
{
  fprintf(stderr,"(EventAvail)\n");
  if(!theEvent) { fprintf(stderr,"EventAvail() theEvent is NULL!\n"); exit(EXIT_FAILURE); }
  theEvent->what=0;
  theEvent->message=0;
  theEvent->when=0;
  theEvent->where.h=0;  theEvent->where.v=0;
  theEvent->modifiers=0;
  fprintf(stderr,"EventAvail() not implemented!\n");
  //exit(EXIT_FAILURE);
  return false;
}


UInt32 TickCount()
{
  fprintf(stderr,"TickCount() not implemented!\n");
  return rand();
}


void FlushEvents(EventMask whichMask, EventMask stopMask)
{
  // NOTE: Ok to skip...
  //fprintf(stderr,"FlushEvents() not implemented!\n");
}


void SetEventMask(EventMask value)
{
  fprintf(stderr,"SetEventMask() not implemented!\n");
}


void SystemTask(void)
{
  // NOTE: Ok to skip...
  //fprintf(stderr,"SystemTask() not implemented!\n");
}


Boolean Button(void)
{
  fprintf(stderr,"(Button)\n");
  struct n_event *myevent;
  /*int isThereAnEvent=check_event(the_cross_window);
  if(isThereAnEvent) {
    myevent=wait_event(the_cross_window);
    // TODO: don't ignore other important events!
    switch(myevent->type) {
      case N_EVENT_CLICK:
        return true;
        break;
      default:
        return false;
        break;
    }
  }*/
  return false;
}


Boolean OSEventAvail(EventMask mask, EventRecord *theEvent)
{
  fprintf(stderr,"OSEventAvail() not implemented!\n");
  return FALSE;
}


void SystemClick(const EventRecord *theEvent, WindowRef theWindow)
{
  if(!theEvent) { fprintf(stderr,"SystemClick() theEvent is NULL!\n"); exit(EXIT_FAILURE); }
  if(!theWindow) { fprintf(stderr,"SystemClick() theWindow is NULL!\n"); exit(EXIT_FAILURE); }
  //fprintf(stderr,"SystemClick() not implemented!\n");
}


Boolean GetOSEvent(EventMask mask, EventRecord *theEvent)
{
  if(!theEvent) { fprintf(stderr,"GetOSEvent() theEvent is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"GetOSEvent() not implemented!\n");
  return FALSE;
}


}
