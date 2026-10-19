
#include <stdio.h>

#include <MyQuickdraw.h>


extern "C" {


#include "../crossroads.h"


extern pascal void InitGraf(void *globalPtr)
 ONEWORDINLINE(0xA86E);


void open_libs(int a,void *b)
{
  fprintf(stderr,"open_libs\n");
  InitGraf(&qd.thePort);
}


};