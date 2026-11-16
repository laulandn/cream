
#include <stdio.h>


extern "C" {


#include "../crossroads.h"


static bool already_open=false;


void open_libs(int a,void *b)
{
  if(already_open) {
    fprintf(stderr,"(already_open)\n"); fflush(stderr);
  }
  else {
    fprintf(stderr,"(open_libs not implemented!)\n"); fflush(stderr);
    already_open=true;
  }
}


};