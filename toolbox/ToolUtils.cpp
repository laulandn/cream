
#include <iostream>


extern "C" {


#include <ToolUtils.h>


////////////////////////////////////////////////////////////////////////////////
// ToolUtils
////////////////////////////////////////////////////////////////////////////////


Boolean BitTst(const void *bytePtr, long bitNum)
{
  if(!bytePtr) { fprintf(stderr,"BitTst() bytePtr is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"BitTst() not implemented!\n");
  return FALSE;
}


void BitClr(void *bytePtr, long bitNum)
{
  if(!bytePtr) { fprintf(stderr,"BitClr() bytePtr is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"BitClr() not implemented!\n");
}


void BitSet(void *bytePtr, long bitNum)
{
  if(!bytePtr) { fprintf(stderr,"BitSet() bytePtr is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"BitSet() not implemented!\n");
}


}
