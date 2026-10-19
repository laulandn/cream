
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


void open_libs(int a,void *b)
{
  fprintf(stderr,"open_libs\n");
#ifdef MACINTOSH
  InitGraf(&qd.thePort);
#endif
}


};