
#include <iostream>


extern "C" {


#include <Perf.h>


////////////////////////////////////////////////////////////////////////////////
// Perf
////////////////////////////////////////////////////////////////////////////////


pascal Boolean InitPerf(TP2PerfGlobals *thePerfGlobals,short timerCount,
	short codeAndROMBucketSize,Boolean doROM,Boolean doAppCode,const Str255 appCodeType,
	short romID,const Str255 romName,Boolean doRAM,long ramLow,long ramHigh,
	short ramBucketSize)
{
  fprintf(stderr,"InitPerf() not implemented!\n");
  return FALSE;
}


pascal void TermPerf(TP2PerfGlobals thePerfGlobals)
{
  fprintf(stderr,"TermPerf() not implemented!\n");
}



pascal Boolean PerfControl(TP2PerfGlobals thePerfGlobals,Boolean turnOn)
{
  fprintf(stderr,"PerfControl() not implemented!\n");
  return FALSE;
}


pascal short PerfDump(TP2PerfGlobals thePerfGlobals,const Str255 reportFile,
	Boolean doHistogram,short rptFileColumns)
{
  fprintf(stderr,"PerfDump() not implemented!\n");
  return 0;
}


}
