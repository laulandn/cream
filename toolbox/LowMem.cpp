
#include <iostream>


extern "C" {


#include <LowMem.h>


////////////////////////////////////////////////////////////////////////////////
// LowMem
////////////////////////////////////////////////////////////////////////////////


UInt8 LMGetCPUFlag()
{
  fprintf(stderr,"LMGetCPUFlag() not implemented!\n");
  return 0;
}


SInt32 LMDefltStack()
{
  fprintf(stderr,"LMDefltStack() not implemented!\n");
  return 0;
}


RgnHandle LMGetGrayRgn()
{
  fprintf(stderr,"LMGetGrayRgn() not implemented!\n");
  return NULL;
}


}

