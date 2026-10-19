
#include <stdio.h>


extern "C" {


#include "../crossroads.h"



void draw_text(void *w,unsigned int x,unsigned int y,char *s)
{
  fprintf(stderr,"(draw_text not implemented!)\n");
}


void set_foregroundc(void *w,long c)
{
  fprintf(stderr,"(set_foregroundc not implemented!)\n");
}


void set_backgroundc(void *w,long c)
{
  fprintf(stderr,"(set_backgroundc not implemented!)\n");
}


void write_pixel(void *w,unsigned int x,unsigned int y)
{
  fprintf(stderr,"(write_pixel not implemented!)\n");
}


void write_pixelc(void *w,unsigned int x,unsigned int y,long c)
{
  fprintf(stderr,"(write_pixelc not implemented!)\n");
}


long read_pixel(void *w,unsigned int x,unsigned int y)
{
  fprintf(stderr,"(read_pixel not implemented!)\n");
  return 0;
}


#if 0
void draw_line(void *w,unsigned int x1,unsigned int y1,unsigned int x2, unsigned int y2)
{
  fprintf(stderr,"(draw_line not implemented!)\n");
}
#endif


void draw_box(void *w,unsigned int x,unsigned int y,unsigned int wide,unsigned int high)
{
  fprintf(stderr,"(draw_box not implemented!)\n");
}


#if 0
void draw_circle(void *w,unsigned int x,unsigned int yunsigned ,int r)
{
  fprintf(stderr,"(draw_circle not implemented!)\n");
}
#endif


};