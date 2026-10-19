#ifndef __OVERRIDE_H__
#define __OVERRIDE_H__


#ifdef NO_QD_TRAPS
#define ONEWORDINLINE(x)
#endif


#ifndef DO_NOT_OVERRIDE


// From Palettes
#define RestoreDeviceClut SHRestoreDeviceClut
#define AnimatePalette SHAnimatePalette
#define GetPalette SHGetPalette
#define Palette2CTab SHPalette2CTab
#define PmForeColor SHPmForeColor
#define PmBackColor SHPmBackColor


// From QuickdrawText
#define TextFace SHTextFace


// From QDOffscreen
#define DisposeGWorld SHDisposeGWorld
#define GetGWorldPixMap SHGetGWorldPixMap
#define NewGWorld SHNewGWorld
#define SetGWorld SHSetGWorld
#define UpdateGWorld SHUpdateGWorld


// From Quickdraw
#define InitGraf SHInitGraf
#define InitCursor SHInitCursor
#define QDError SHQDError
#define BackColor SHBackColor
#define CopyBits SHCopyBits
#define DisposeRgn SHDisposeRgn
#define DrawText SHDrawText
#define EraseRect SHEraseRect
#define InvertRect SHInvertRect
#define OffsetRect SHOffsetRect
#define PaintRect SHPaintRect
#define ForeColor SHForeColor
#define GetGDevice SHGetGDevice
#define GlobalToLocal SHGlobalToLocal
#define Index2Color SHIndex2Color
#define LineTo SHLineTo
#define MoveTo SHMoveTo
#define NewRgn SHNewRgn
#define PenMode SHPenMode
#define RGBBackColor SHRGBBackColor
#define RGBForeColor SHRGBForeColor
#define ScrollRect SHScrollRect
#define SetEntries SHSetEntries
#define SetPort SHSetPort
#define SetRect SHSetRect
#define TextFont SHTextFont
#define TextMode SHTextMode
#define TextSize SHTextSize
#define TextWidth SHTextWidth
#define GetPort SHGetPort
#define GetCTSeed SHGetCTSeed
#define GetPicture SHGetPicture
#define GetCTable SHGetCTable
#define Line SHLine
#define PenPat SHPenPat
#define ShowCursor SHShowCursor
#define HideCursor SHHideCursor
#define BackPat SHBackPat
#define GetPixel SHGetPixel
#define DisposeCTable SHDisposeCTable
#define InsetRect SHInsetRect
#define Random SHRandom
#define PaintOval SHPaintOval
#define InvertColor SHInvertColor
#define DrawString SHDrawString
#define GetGWorld SHGetGWorld
#define InvalRgn SHInvalRgn
#define GetPixBaseAddr SHGetPixBaseAddr
#define InvalRect SHInvalRect
#define LockPixels SHLockPixels
#define UnlockPixels SHUnlockPixels
#define PenNormal SHPenNormal
#define ValidRect SHValidRect
#define LocalToGlobal SHLocalToGlobal
#define PtInRect SHPtInRect
#define OffsetRgn SHOffsetRgn
#define SectRect SHSectRect
#define ClipRect SHClipRect
#define CopyRgn SHCopyRgn
#define DiffRgn SHDiffRgn
#define EmptyRgn SHEmptyRgn
#define PtInRgn SHPtInRgn
#define RectRgn SHRectRgn
#define SectRgn SHSectRgn
#define SetOrigin SHSetOrigin
#define SetPt SHSetPt
#define SetRectRgn SHSetRectRgn
#define FrameOval SHFrameOval
#define OpenPort SHOpenPort
#define ClosePort SHClosePort
#define SetPortBits SHSetPortBits
#define SaveEntries SHSaveEntries
#define RestoreEntries SHRestoreEntries
#define GetDeviceList SHGetDeviceList
#define GetMainDevice SHGetMainDevice
#define GetNextDevice SHGetNextDevice
#define SetGDevice SHSetGDevice
#define CTabChanged SHCTabChanged


#endif


#endif