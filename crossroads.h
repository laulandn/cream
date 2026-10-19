#ifndef __CROSSROADS_H__
#define __CROSSROADS_H__


struct cross_display
{
  int width;
  int height;
  int depth;
  int nPlanes;
};

struct cross_window
{
  int width;
  int height;
};

struct n_event
{
  int x,y;
  int type;
};


extern cross_display *the_cross_display;
// Only one window for now...
extern cross_window *the_cross_window;


// Init things
// NOTE: Will be called multiple times!
void open_libs(int a,void *b);


// Used by slowhand
// Colors are probably XRGB, but could also be indexed
// NOTE: I may change coords from unsigned to signed
void draw_line(void *w,unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void draw_text(void *w,unsigned int x,unsigned int y,char *s);
void draw_circle(void *w,unsigned int x,unsigned int y,unsigned int r);
void draw_box(void *w,unsigned int x,unsigned int y,unsigned int wide,unsigned int high);
//
void write_pixel(void *w,unsigned int x,unsigned int y);
void write_pixelc(void *w,unsigned int x,unsigned int y,long c);
long read_pixel(void *w,unsigned int x,unsigned int y);
//
void set_foregroundc(void *w,long c);
void set_backgroundc(void *w,long c);


// Used by toolbox, only if Events are used
bool check_event(struct cross_window *w);
struct n_event *wait_event(struct cross_window *w);
struct n_event *get_next_event(struct cross_window *w);


// Used by toolbox, only if Windows are used
// Passing zeros for w, h, d should be "default" or "full screen"
struct cross_display *open_cross_display(void *a,unsigned int w, unsigned int h, unsigned d);
struct cross_window *open_cross_window(void *a,char *s,unsigned int w, unsigned int h);


#endif