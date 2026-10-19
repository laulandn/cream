#ifndef __FB_H__
#define __FB_H__


extern char *fbPixels;
extern unsigned int fbPixelByteSize;
extern unsigned int fbMinBlit,fbMaxBlit;

extern unsigned int fbRowBytes;
extern unsigned int fbWidth,fbHeight,fbDepth;


#ifdef SNIAL

const unsigned short gColorPalette[16]={
	0x000, 0x400, 0x080, 0x480,
	0x008, 0x408, 0x088, 0x255,
	0x4aa, 0x700, 0x0f0, 0x7f0,
	0x00f, 0x70f, 0x0ff, 0x7ff,
	
};


#define gVidModeSet(aVal) *((unsigned short*)0xbffffa)=(aVal)
#define PaletteSet(aEntry, aColor) gVidModeSet(0x8000|(aEntry<<11)|aColor)
#define ModeSet(aMode) gVidModeSet(aMode)

#endif


#endif