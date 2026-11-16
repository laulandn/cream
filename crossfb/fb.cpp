
#include <stdio.h>


#ifdef MACINTOSH
#include <MyQuickdraw.h>
#endif


extern "C" {


#include "../crossroads.h"


#include "fb.h"


#ifdef MACINTOSH
extern pascal void InitGraf(void *globalPtr)
 ONEWORDINLINE(0xA86E);
#endif


#ifdef MACINTOSH
extern QDGlobals qd;
#endif


static bool already_open=false;


void open_libs(int a,void *b)
{
  if(already_open) {
    fprintf(stderr,"(already_open)\n"); fflush(stderr);
  }
  else {
    freopen ("stdout.txt", "w", stdout);
    freopen ("stderr.txt", "w", stderr);
    fprintf(stderr,"open_libs\n"); fflush(stderr);
#ifdef MACINTOSH
    InitGraf(&qd.thePort);
#endif
    already_open=true;
  }
}


};