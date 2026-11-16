
#include <stdio.h>

#include <MyQuickdraw.h>


extern "C" {


#include "../crossroads.h"


extern pascal void InitGraf(void *globalPtr)
 ONEWORDINLINE(0xA86E);


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
    InitGraf(&qd.thePort);    
    already_open=true;
  }
}


};