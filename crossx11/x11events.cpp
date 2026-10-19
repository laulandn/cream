
#include <stdio.h>


extern "C" {


#include "../crossroads.h"


bool check_event(struct cross_window *w)
{
  fprintf(stderr,"(check_event not implemented!)\n");
  return false;
}


struct n_event *wait_event(struct cross_window *w)
{
  fprintf(stderr,"(wait_event not implemented!)\n");
  return NULL;
}


struct n_event *get_next_event(struct cross_window *w)
{
  fprintf(stderr,"(get_next_event not implemented!)\n");
  return NULL;
}


};