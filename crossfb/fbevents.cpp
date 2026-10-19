
#include <stdio.h>


extern "C" {


#include "../crossroads.h"


#include "fb.h"


#ifndef NO_DEFAULT_EVENTS
bool check_event(struct cross_window *w)
{
  fprintf(stderr,"(check_event not implemented!)\n");
  return false;
}
#endif


#ifndef NO_DEFAULT_EVENTS
struct n_event *wait_event(struct cross_window *w)
{
  fprintf(stderr,"(wait_event not implemented!)\n");
  return NULL;
}
#endif


#ifndef NO_DEFAULT_EVENTS
struct n_event *get_next_event(struct cross_window *w)
{
  fprintf(stderr,"(get_next_event not implemented!)\n");
  return NULL;
}
#endif


};