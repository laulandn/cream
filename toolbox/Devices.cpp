#include <iostream>


extern "C" {


#ifdef Status
#undef Status
#endif

#include <Devices.h>


////////////////////////////////////////////////////////////////////////////////
// Devices
////////////////////////////////////////////////////////////////////////////////


short OpenDeskAcc(ConstStr255Param deskAccName)
{
  if(!deskAccName) { fprintf(stderr,"OpenDeskAcc() deskAccName is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"OpenDeskAcc() not implemented!\n"); 
  return 0;
}


void CloseDeskAcc(short refNum)
{
  fprintf(stderr,"CloseDeskAcc() not implemented!\n"); 
}


}
