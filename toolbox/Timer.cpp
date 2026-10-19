
#include <iostream>


extern "C" {


#include <Timer.h>


////////////////////////////////////////////////////////////////////////////////
// Timer
////////////////////////////////////////////////////////////////////////////////


void Microseconds(UnsignedWide *microTickCount)
{
  if(!microTickCount) { fprintf(stderr,"Microseconds() microTickCount is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"Microseconds() not implemented!\n");
}


}

