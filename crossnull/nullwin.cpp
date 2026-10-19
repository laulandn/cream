
#include <stdio.h>
#include <stdlib.h>


extern "C" {


#include "../crossroads.h"


struct cross_display *open_cross_display(void *a,unsigned int w, unsigned int h, unsigned d)
{
  struct cross_display *dis=NULL;
  fprintf(stderr,"open_cross_display %d x %d x %d\n",w,h,d); fflush(stderr);
  dis=(struct cross_display *)calloc(sizeof(struct cross_display),1);
  if(!dis) {
    fprintf(stderr,"No dis!\n");
    exit(5);
  }
  if(!w) {
  }
  if(!h) {
  }
  if(!d) {
  }
  dis->width=w;
  dis->height=h;
  dis->depth=d;
  dis->nPlanes=1;
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
  if(!w) {
  }
  if(!h) {
  }
  win->width=w;
  win->height=h;
  return win;
}


};