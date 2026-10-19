
#include <stdio.h>
#include <stdlib.h>


extern "C" {


#include "../crossroads.h"


#ifndef NO_DEFAULT_EVENTS
bool check_event(struct cross_window *w)
{
  fprintf(stderr,"DO NOT USE check_event() on real mac!\n");
  exit(5);
  return false;
}
#endif


#ifndef NO_DEFAULT_EVENTS
struct n_event *wait_event(struct cross_window *w)
{
  fprintf(stderr,"DO NOT USE wait_event() on real mac!\n");
  exit(5);
  return NULL;
}
#endif


#ifndef NO_DEFAULT_EVENTS
struct n_event *get_next_event(struct cross_window *w)
{
  fprintf(stderr,"DO NOT USE get_next_event() on real mac!\n");
  exit(5);
  return NULL;
}
#endif


};