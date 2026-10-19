
#include <stdio.h>
#include <string.h>


// NOTE: Do NOT use MyQuickdraw.h!
#include <Quickdraw.h>


extern "C" {


#include "../crossroads.h"


extern unsigned int shCurX,shCurY;


void draw_text(void *w,unsigned int x,unsigned int y,char *s)
{
  fprintf(stderr,"(draw_text not implemented!)\n");
  char ts[256];
  ts[0]=strlen(s);
  for(int i=0;i<ts[0];i++) ts[i+1]=s[i];
  MoveTo(shCurX,shCurY);
  DrawString((ConstStr255Param)ts);
}


void set_foregroundc(void *w,long c)
{
  fprintf(stderr,"set_foregroundc\n");
  RGBColor tc;
  tc.red=((c>>16)&0xff)<<8;
  tc.blue=((c>>8)&0xff)<<8;
  tc.green=((c>>0)&0xff)<<8;
#ifndef NO_COLOR_QUICKDRAW
  RGBForeColor((RGBColor *)&tc);
#endif
}


void set_backgroundc(void *w,long c)
{
  fprintf(stderr,"set_backgroundc\n");
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


void draw_line(void *w,unsigned int x1,unsigned int y1,unsigned int x2, unsigned int y2)
{
  fprintf(stderr,"(draw_line not implemented!)\n");
}


void draw_box(void *w,unsigned int x,unsigned int y,unsigned int wide,unsigned int high)
{
  fprintf(stderr,"(draw_box not implemented!)\n");
}


#ifdef NO_DEFAULT_DRAW
void draw_circle(void *w,unsigned int x,unsigned int y,unsigned int r)
{
  fprintf(stderr,"draw_circle\n");
  Rect rt;
  rt.left=x;
  rt.right=rt.left+(r*2);
  rt.top=y;
  rt.bottom=rt.top+(r*2);
  MoveTo(shCurX,shCurY);
  PaintOval(&rt);
}
#endif


};