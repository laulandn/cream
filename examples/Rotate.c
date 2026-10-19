#include 	<Gestalt.h>
#include	<Palettes.h>
#include	<Dialogs.h>
#include	<ToolUtils.h>
#include	<Devices.h>
#include	<Processes.h>
#include	<Fonts.h>
#include	<QDOffscreen.h>


extern void ExitToShell();


#define	BIT00   	0x01
#define	BIT01   	0x02
#define	BIT02   	0x04
#define	BIT03   	0x08
#define	BIT04   	0x10
#define	BIT05   	0x20
#define	BIT06   	0x40
#define	BIT07   	0x80

BitMap		*macBitMap;

rotate()
{
	Ptr				srcPtr, destPtr, destColPtr, myBaseAddr;
	BitMap			*newBitmap;
	short			destRowBytes, bitchar, srcByte, srcH, srcV, srccols, srcrows, i;
	Rect			destRect;

	/* rotate boundary rectangle */
	srcH = macBitMap->bounds.right - macBitMap->bounds.left;
	srcV = macBitMap->bounds.bottom - macBitMap->bounds.top;
	SetRect(&destRect, macBitMap->bounds.left, macBitMap->bounds.top,
			macBitMap->bounds.left+srcV, macBitMap->bounds.top+srcH);

	/* allocate destination buffer */
	destRowBytes  = (srcV+7) >> 3;
	newBitmap = (BitMap *) NewPtrClear(sizeof(BitMap));
	myBaseAddr = (Ptr) NewPtrClear(destRowBytes*srcH);

	/* set-up src to rotated destination scan */
	srccols = macBitMap->rowBytes;
	srcrows = srcV;
	srcPtr = macBitMap->baseAddr;
	bitchar = BIT00;
	destColPtr= myBaseAddr + destRowBytes - 1;
	destPtr   = destColPtr;

	/* scan src row and rotate into dest col */
	while (srcrows-- > 0) {
		for (i=0; i<srccols; i++) {
			srcByte = *(srcPtr++);
			if (srcByte & BIT07) *destPtr |= bitchar;
			destPtr += destRowBytes;
			if (srcByte & BIT06) *destPtr |= bitchar;
			destPtr += destRowBytes;
			if (srcByte & BIT05) *destPtr |= bitchar;
			destPtr += destRowBytes;
			if (srcByte & BIT04) *destPtr |= bitchar;
			destPtr += destRowBytes;
			if (srcByte & BIT03) *destPtr |= bitchar;
			destPtr += destRowBytes;
			if (srcByte & BIT02) *destPtr |= bitchar;
			destPtr += destRowBytes;
			if (srcByte & BIT01) *destPtr |= bitchar;
			destPtr += destRowBytes;
			if (srcByte & BIT00) *destPtr |= bitchar;
			destPtr += destRowBytes;
		}
		if (bitchar==BIT07) {
			bitchar = BIT00;
			destColPtr--;
		} else {
			bitchar <<= 1;
		}

		destPtr = destColPtr;
	}
	
	/* remove src bitmap (portbits for offscreen port) */
	DisposPtr((Ptr)macBitMap->baseAddr);
	DisposPtr((Ptr)macBitMap);
	
	/* store new src in object */
	macBitMap   = (BitMap *)newBitmap;
	macBitMap->baseAddr = (Ptr) myBaseAddr;
	macBitMap->rowBytes = destRowBytes;
	macBitMap->bounds   = destRect;
}

int main(int argc,char **argv)
{
	WindowPtr		mainWinPtr;
	OSErr			error;
	SysEnvRec		theWorld;
	Rect			windRect, ovalRect, myBounds;
	short			myRowBytes;
	
	/* Make sure ColorQD exists. */
	error = SysEnvirons(1, &theWorld);
	if (theWorld.hasColorQD == false) {
		SysBeep(50);
		ExitToShell();
	}
	
	/* Initialize all the needed managers. */
	InitGraf(&qd.thePort);
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(nil);
	InitCursor();

	/* Define output window with an inset clip region. */
	SetRect(&windRect, 100, 100, 404, 404);
	mainWinPtr = NewWindow(nil, &windRect, "\pJohn", true, documentProc, (WindowPtr) -1, false, 0);
	SetPort(mainWinPtr);

	SetRect(&windRect, 0, 0, 304, 304);
	EraseRect(&windRect);
	SetRect(&ovalRect, 15, 15, 60, 120);
	PaintOval(&ovalRect);
	MoveTo(10,140);
	DrawString("\PHi there.");
	MoveTo(0,0);
	LineTo(303,303);
	MoveTo(0,303);
	LineTo(303,0);

	/* rotate boundary rectangle */
	myBounds = windRect;
	myRowBytes = (myBounds.right - myBounds.left + 7) >> 3;
	macBitMap = (BitMap *) NewPtrClear(sizeof(BitMap));
	macBitMap->baseAddr = (Ptr) NewPtrClear(myRowBytes * (myBounds.bottom - myBounds.top));
	macBitMap->bounds = myBounds;
	macBitMap->rowBytes = myRowBytes;

	CopyBits(&(*mainWinPtr).portBits, macBitMap, &windRect, &macBitMap->bounds, 0, nil);

	/* Wait until user clicks button. */
	do {
	} while (!Button());

	/* Wait until user clicks button. */
	do {
	rotate();
	CopyBits(macBitMap, &(*mainWinPtr).portBits, &macBitMap->bounds, &windRect, 0, nil);
	} while (!Button());
}

