#ifndef __SLOWHAND_H__
#define __SLOWHAND_H__


#ifndef NO_DEFAULT_DRAW
void default_draw_line(void *w,unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void default_draw_box(void *w,unsigned int x,unsigned int y,unsigned int wide,unsigned int high);
void default_draw_circle(void *w,unsigned int x,unsigned int y,unsigned int r);
#endif


#if 0
extern void RestoreDeviceClut(GDHandle gd);
extern void AnimatePalette(WindowPtr dstWindow, CTabHandle srcCTab, short srcIndex, short dstEntry, short dstLength);
extern PaletteHandle GetPalette(WindowPtr srcWindow);
extern void Palette2CTab(PaletteHandle srcPalette, CTabHandle dstCTab);
extern void PmForeColor(short dstEntry);
extern void PmBackColor(short dstEntry);
extern void DisposeGWorld(GWorldPtr offscreenGWorld);
extern PixMapHandle GetGWorldPixMap(GWorldPtr offscreenGWorld);
extern QDErr NewGWorld(GWorldPtr *gw, short PixelDepth, const Rect *boundsRect, CTabHandle cTable, GDHandle aGDevice, GWorldFlags flags); 
extern void SetGWorld(CGrafPtr port, GDHandle gdh);
extern GWorldFlags UpdateGWorld(GWorldPtr *offscreenGWorld, short pixelDepth, const Rect *boundsRect, CTabHandle cTable, GDHandle aGDevice, GWorldFlags flags);
extern void InitGraf(void *globalPtr);
extern void InitCursor();
extern short QDError();
extern void BackColor(long color);
extern void CopyBits(const BitMap *srcBits, const BitMap *dstBits, const Rect *srcRect, const Rect *dstRect, short mode, RgnHandle maskRgn);
extern void DisposeRgn(RgnHandle rgn);
extern void DrawText(const void *textBuf, short firstByte, short byteCount);
extern void EraseRect(const Rect *r);
extern void InvertRect(const Rect *r);
extern void OffsetRect(Rect *r, short dh, short dv);
extern void PaintRect(const Rect *r);
extern void ForeColor(long color);
extern GDHandle GetGDevice();
extern void GlobalToLocal(Point *pt);
extern void Index2Color(long index, RGBColor *aColor);
extern void LineTo(short h, short v);
extern void MoveTo(short h, short v);
extern RgnHandle NewRgn();
extern void PenMode(short mode);
extern void RGBBackColor(const RGBColor *color);
extern void RGBForeColor(const RGBColor *color);
extern void ScrollRect(const Rect *r, short dh, short dv, RgnHandle updateRgn);
extern void SetEntries(short start, short count, CSpecArray aTable);
extern void SetPort(GrafPtr port);
extern void SetRect(Rect *r, short left, short top, short right, short bottom);
extern void TextFont(short font);
extern void TextMode(short mode);
extern void TextSize(short size);
extern short TextWidth(const void *textBuf, short firstByte, short byteCount);
extern void GetPort(GrafPtr *port);
extern long GetCTSeed(void);
extern PicHandle GetPicture(short pictureID);
extern CTabHandle GetCTable(short ctID);
extern void Line(short dh, short dv);
extern void PenPat(const Pattern *pat);
extern void ShowCursor(void);
extern void HideCursor(void);
extern void BackPat(const Pattern *pat);
extern Boolean GetPixel(short h, short v);
extern void DisposeCTable(CTabHandle cTable);
extern void InsetRect(Rect *r, short dh, short dv);
extern short Random(void);
extern void PaintOval(const Rect *r);
extern void InvertColor(RGBColor *myColor);
extern void DrawString(ConstStr255Param s);
extern void GetGWorld(CGrafPtr *port, GDHandle *gdh);
extern void InvalRgn(); 
extern Ptr GetPixBaseAddr(PixMapHandle pm); 
extern void InvalRect(); 
extern Boolean LockPixels(PixMapHandle pm); 
extern void UnlockPixels(PixMapHandle pm); 
extern void PenNormal(); 
extern void ValidRect(); 
extern void LocalToGlobal(Point *pt); 
extern Boolean PtInRect(Point pt, const Rect *r); 
extern void OffsetRgn(RgnHandle rgn, short dh, short dv); 
extern Boolean SectRect(const Rect *src1, const Rect *src2, Rect *dstRect); 
extern void ClipRect(const Rect *r); 
extern void CopyRgn(RgnHandle srcRgn, RgnHandle dstRgn); 
extern void DiffRgn(RgnHandle srcRgnA, RgnHandle srcRgnB, RgnHandle dstRgn); 
extern Boolean EmptyRgn(RgnHandle rgn); 
extern Boolean PtInRgn(Point pt, RgnHandle rgn); 
extern void RectRgn(RgnHandle rgn, const Rect *r);
extern void SectRgn(RgnHandle srcRgnA, RgnHandle srcRgnB, RgnHandle dstRgn);
extern void SetOrigin(short h, short v); 
extern void SetPt(Point *pt, short h, short v); 
extern void SetRectRgn(RgnHandle rgn, short left, short top, short right, short bottom); 
extern void FrameOval(const Rect *r); 
extern void OpenPort(GrafPtr port); 
extern void ClosePort(GrafPtr port); 
extern void SetPortBits(const BitMap *bm); 
extern void SaveEntries(CTabHandle srcTable, CTabHandle resultTable, ReqListRec *selection); 
extern void RestoreEntries(CTabHandle srcTable, CTabHandle dstTable, ReqListRec *selection);
extern GDevice** GetDeviceList();
extern GDevice** GetMainDevice();
extern GDHandle GetNextDevice(GDHandle curDevice);
extern void SetGDevice(GDHandle gd);
extern void CTabChanged(CTabHandle ctab);
extern void TextFace(short face);
#endif


#endif