
#include <iostream>


extern "C" {


#include <MyPalettes.h>


#include "../crossroads.h"


#define _SH_FUNK


////////////////////////////////////////////////////////////////////////////////
// Palettes
////////////////////////////////////////////////////////////////////////////////


_SH_FUNK void RestoreDeviceClut(GDHandle gd) {
  if(!gd) { fprintf(stderr,"RestoreDeviceClut() gd is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"RestoreDeviceClut() not implemented!\n");
}


_SH_FUNK void AnimatePalette(WindowPtr dstWindow, CTabHandle srcCTab, short srcIndex, short dstEntry, short dstLength)
{
  if(!dstWindow) { fprintf(stderr,"AnimatePalette() dstWindow is NULL!\n"); exit(EXIT_FAILURE); }
  if(!srcCTab) { fprintf(stderr,"AnimatePalette() srcCTab is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"AnimatePalette() not implemented!\n");
}


_SH_FUNK PaletteHandle GetPalette(WindowPtr srcWindow)
{
  if(!srcWindow) { fprintf(stderr,"GetPalette() srcWindow is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"GetPalette() not implemented!\n");
  return NULL;
}


_SH_FUNK void Palette2CTab(PaletteHandle srcPalette, CTabHandle dstCTab)
{
  if(!srcPalette) { fprintf(stderr,"Palette2CTab() srcPalette is NULL!\n"); exit(EXIT_FAILURE); }
  if(!dstCTab) { fprintf(stderr,"Palette2CTab() dstCTab is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"Palette2CTab() not implemented!\n");
}


_SH_FUNK void PmForeColor(short dstEntry)
{
  fprintf(stderr,"PmForeColor() not implemented!\n");
}


_SH_FUNK void PmBackColor(short dstEntry)
{
  fprintf(stderr,"PmBackColor() not implemented!\n");
}
  
  
}
