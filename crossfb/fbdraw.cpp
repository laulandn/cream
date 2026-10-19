
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


extern "C" {


#include "../crossroads.h"

#include "../slowhand/slowhand.h"


#include "fb.h"


////////////////////////////////////////////////////////////////////////////////


/* Useful triplet breaking macros... */
/* These are for getting 8 bit colors out of a 24 bit triplet */
#define fbDrawGetRGBRed24(x)   ((unsigned int)((x>>16)&0xff))
#define fbDrawGetRGBGreen24(x) ((unsigned int)((x>>8)&0xff))
#define fbDrawGetRGBBlue24(x)  ((unsigned int)(x&0xff))
/* These are for getting 4 bit colors out of a 12 bit triplet */
#define fbDrawGetRGBRed12(x)   ((unsigned int)((x>>8)&0xf))
#define fbDrawGetRGBGreen12(x) ((unsigned int)((x>>4)&0xf))
#define fbDrawGetRGBBlue12(x)  ((unsigned int)(x&0xf))
/* These are for getting 3 bit colors out of a 9 bit triplet */
#define fbDrawGetRGBRed9(x)    ((unsigned int)((x>>6)&0x7))
#define fbDrawGetRGBGreen9(x)  ((unsigned int)((x>>3)&0x7))
#define fbDrawGetRGBBlue9(x)   ((unsigned int)(x&0x7))
/* Build 24 bit triplet from 8 bit colors */
#define fbDrawMakeRGB24(x,y,z) ((z&0xff)+((y&0xff)<<8)+((x&0xff)<<16))
/* Build 12 bit triplet from 4 bit colors */
#define fbDrawMakeRGB12(x,y,z) ((z&0xf)+((y&0xf)<<4)+((x&0xf)<<8))
/* Build 9 bit triplet from 3 bit colors */
#define fbDrawMakeRGB9(x,y,z) ((z&0x7)+((y&0x7)<<3)+((x&0x7)<<6))


////////////////////////////////////////////////////////////////////////////////


unsigned int fbCurX=0,fbCurY=0;
unsigned int fbForeColor=1,fbBackColor=0;

char *fbPixels=NULL;
unsigned int fbPixelByteSize;
unsigned int fbMinBlit=1,fbMaxBlit=1;

unsigned int fbRowBytes=0;
unsigned int fbWidth=0,fbHeight=0,fbDepth=1;


////////////////////////////////////////////////////////////////////////////////


void draw_text(void *w,unsigned int x,unsigned int y,char *s)
{
  if(!fbPixels) {
    fprintf(stderr,"fbPixels is NULL!\n");
    // Panic!
    exit(5);
    return;    
  }
  fprintf(stderr,"(draw_text not implemented!)\n");
}


void set_foregroundc(void *w,long c)
{
  fbForeColor=c;
}


void set_backgroundc(void *w,long c)
{
  fbBackColor=c;
}


void write_pixel(void *w,unsigned int x,unsigned int y)
{
  if(!fbPixels) {
    fprintf(stderr,"fbPixels is NULL!\n");
    // Panic!
    exit(5);
    return;    
  }
  fbCurX=x;  fbCurY=y;
  write_pixelc(w,x,y,fbForeColor);
}


void write_pixelc(void *w,unsigned int x,unsigned int y,long c)
{
  if(!fbPixels) {
    fprintf(stderr,"fbPixels is NULL!\n");
    // Panic!
    exit(5);
    return;    
  }
  fbCurX=x;  fbCurY=y;
  //
  unsigned int minBlit=1;
  unsigned int maxBlit=fbRowBytes;
  unsigned int tx=(x/minBlit)*maxBlit;
  unsigned int offset=tx+(y*(fbRowBytes*fbWidth));
  //
  if(x>(unsigned int)fbWidth) x=fbWidth-1;
  if(y>(unsigned int)fbHeight) y=fbHeight-1;
  if(offset>fbPixelByteSize) { return; }
  //
  /* TODO: Very BAD!  Temporary... */
  unsigned char *dest=(unsigned char *)((long)fbPixels+offset);
  dest[2]=fbDrawGetRGBRed24(fbForeColor);
  dest[1]=fbDrawGetRGBGreen24(fbForeColor);
  dest[0]=fbDrawGetRGBBlue24(fbForeColor);
}


long read_pixel(void *w,unsigned int x,unsigned int y)
{
  if(!fbPixels) {
    fprintf(stderr,"fbPixels is NULL!\n");
    // Panic!
    exit(5);
    return 0;    
  }
  //
  unsigned int minBlit=1;
  unsigned int maxBlit=fbRowBytes;
  unsigned int tx=(x/minBlit)*maxBlit;
  unsigned int offset=tx+(y*(fbRowBytes*fbWidth));
  //
  if(x>(unsigned int)fbWidth) x=fbWidth-1;
  if(y>(unsigned int)fbHeight) y=fbHeight-1;
  if(offset>fbPixelByteSize) { return 0; }
  //
  /* TODO: Very BAD!  Temporary... */
  unsigned char *src=(unsigned char *)((long)fbPixels+offset);
  return fbDrawMakeRGB24(src[2],src[1],src[0]);
}


void draw_line(void *w,unsigned int x1,unsigned int y1,unsigned int x2, unsigned int y2)
{
  if(!fbPixels) {
    fprintf(stderr,"fbPixels is NULL!\n");
    // Panic!
    exit(5);
    return;    
  }
  fbCurX=x2;  fbCurY=y2;
  //
  default_draw_line(w,x1,y1,x2,y2);
}


void draw_box(void *w,unsigned int x,unsigned int y,unsigned int wide,unsigned int high)
{
  if(!fbPixels) {
    fprintf(stderr,"fbPixels is NULL!\n");
    // Panic!
    exit(5);
    return;    
  }
  fbCurX=x;  fbCurY=y;
  default_draw_box(w,x,y,wide,high);
 }


void draw_circle(void *w,unsigned int x,unsigned int y,unsigned int r)
{
  if(!fbPixels) {
    fprintf(stderr,"fbPixels is NULL!\n");
    // Panic!
    exit(5);
    return;    
  }
  fbCurX=x;  fbCurY=y;
  //
  default_draw_circle(w,x,y,r);
}


};