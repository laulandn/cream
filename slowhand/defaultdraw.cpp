
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


extern "C" {


#include "slowhand.h"

#include "../crossroads.h"


#ifndef NO_DEFAULT_DRAW
void default_draw_line(void *w,unsigned int xu1,unsigned int yu1,unsigned int xu2,unsigned int yu2)
{
  int x1=xu1, x2=xu2, y1=yu1, y2=yu2;
  unsigned int x,y;
  int deltax,deltay,temp,err,i,swap,s1,s2;
  x=x1;  y=y1;
/* FIXME: The unsigned coords might mess the next 4 lines up a little... */
  deltax=abs(x2-x1);
  deltay=abs(y2-y1);
  if((x2-x1)<0) s1= -1; else s1=1;
  if((y2-y1)<0) s2= -1; else s2=1;
  if(deltay>deltax) {
    temp=deltax;  deltax=deltay;  deltay=temp;  swap=1;
  }
  else swap=0;
  err=2*deltay-deltax;
  for(i=1;i<=deltax;i++) {
    write_pixel(w,x,y);
    while(err>=0) {
      if(swap) x+=s1; else y+=s2;
      err-=2*deltax;
    }
    if(swap) y+=s2; else x+=s1;
    err+=2*deltay;
  }
}
#endif


#ifndef NO_DEFAULT_DRAW
void default_draw_box(void *w,unsigned int x,unsigned int y,unsigned int wide,unsigned int high)
{
  // TODO
 }
#endif


#ifndef NO_DEFAULT_DRAW
void default_draw_circle(void *w,unsigned int x, unsigned int y, unsigned int r)
{
	unsigned int i, j, k, rs, lj;
	rs = (lj = r)*r;
	for(i=0; i <= r; ++i) {
		j = k = sqrt(rs - (i*i));
		do {
		    // The four quadrents...
			write_pixel(w,x+i, y+j);
			write_pixel(w,x+i, y-j);
			write_pixel(w,x-i, y+j);
			write_pixel(w,x-i, y-j);
		}
		while(++j < lj);
		lj = k;
	}
}
#endif


};