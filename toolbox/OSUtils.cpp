
#include <iostream>

#include <stdlib.h>


extern "C" {


#include <OSUtils.h>
#include <Gestalt.h>


#include "../crossroads.h"


////////////////////////////////////////////////////////////////////////////////
// OSUtils
////////////////////////////////////////////////////////////////////////////////


void SysBeep(short l) {
  fprintf(stderr,"(SysBeep)\n");
}


OSErr SysEnvirons(short n, SysEnvRec *e) {
  fprintf(stderr,"(SysEnvirons)\n");
  if(!e) { fprintf(stderr,"SysEnvirons() e is NULL!\n"); exit(EXIT_FAILURE); }
  e->environsVersion=curSysEnvVers;
  e->machineType=envMachUnknown;
  e->systemVersion=0x0700;
  e->processor=envCPUUnknown;
  e->hasFPU=true;
  e->hasColorQD=true;
  e->keyBoardType=envUnknownKbd;
  e->atDrvrVersNum=0;
  e->sysVRefNum=0;
  return 0;
}


void GetDateTime(unsigned long *secs)
{
  if(!secs) { fprintf(stderr,"GetDateTime() secs is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"GetDateTime() not implemented!\n");
  *secs=rand();
}


extern pascal UniversalProcPtr GetToolTrapAddress(UInt16 trapNum)
//UniversalProcPtr GetToolTrapAddress(SInt16 trapNum)
{
  fprintf(stderr,"GetToolTrapAddress() not implemented!\n");
  return NULL;
}


UniversalProcPtr NGetTrapAddress(UInt16 trapNum, TrapType tTyp)
{
  fprintf(stderr,"NGetTrapAddress() not implemented!\n");
  return NULL;
}


}
