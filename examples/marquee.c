/*
** James "im" Beninghaus
*/

/*
** MPW 3.0 C source to marquee!
*/

#include <QuickDraw.h>
#include <Events.h>
#include <Fonts.h>
#include <Menus.h>
#include <Windows.h>
#include <Packages.h>

Resume() {
	ExitToShell();
}
	
main () {
	auto	EventRecord		event;
	auto	WindowRecord	window;
	auto	Rect			bounds,
							rect;
	auto	Point			start;
	
	/*
	** initialize the macintosh
	*/
	InitGraf((Ptr) &qd.thePort);
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs((ResumeProcPtr) Resume);
	InitCursor();
		
	SetRect(&bounds, 50, 50, 500, 300);
	NewWindow((Ptr)&window, &bounds, "\pApple Computer Inc.", true, noGrowDocProc, (WindowPtr)-1L, false, 0L);
	SetPort((GrafPtr)&window);
	
	SetRect(&rect, 0, 0, 0, 0);
	while (true) {
		GetNextEvent(everyEvent, &event);
		switch (event.what) {
			case mouseDown :
				start = event.where;
				GlobalToLocal(&start);
				TrackMarquee(start, &rect);
				break;
			case keyDown :
				ExitToShell();
				break;
		}
	}
}

/*
** Description
**		TrackMarquee will display a marquee similar to the 
**		selection rectangle tool in MacPaintª. It is assumed that the
**		current port has been set before calling. 
**
** Parameters
**		start		: the local coordinates where the mouse down occured.
**		resultRect	: the final rectangle that was selected.
*/

#define	TICKDELAY	2

TrackMarquee(start, resultRect)
	Point	start;
	Rect	*resultRect;
{	
	/*
	** there are fifteen patterns defined here 
	** each one eight bytes long starting at :
	** patterns[0], patterns[1], patterns[2], patterns[3],
	** patterns[4], patterns[5], patterns[6], patterns[7]
	*/
	static	unsigned char	patterns[] = {
		0xF8, 0xF1, 0xE3, 0xC7, 0x8F, 
		0x1F, 0x3E, 0x7C, 0xF8, 0xF1, 
		0xE3, 0xC7, 0x8F, 0x1F, 0x3E
	};
	
	auto		Point		mouse;		/* the current mouse location */
	register	short		index;		/* the index of the current patterns array */
	auto		Rect		nowRect,	/* the current rectangle to be framed */
							thenRect;	/* the last rectangle to be framed */
	auto		long		nowTicks,	/* the current tick count */
							thenTicks;	/* the last tick count */
	auto		PenState	penState;	/* the saved pen state on entry to procedure */

	
	thenTicks = 0;
	index = 0;
	
	GetPenState(&penState);
	PenMode(patXor);
	
	PenPat(&patterns[index]);
	SetRect(&nowRect, start.h, start.v, start.h, start.v);
	FrameRect(&nowRect);
	thenRect = nowRect;
	
	while (StillDown()) {
		nowTicks = TickCount();
		GetMouse(&mouse);
		SetMobiusRect(&nowRect, start.h, start.v, mouse.h, mouse.v);
		if (((thenTicks + TICKDELAY) < nowTicks ? thenTicks = nowTicks, true : false) || 
		(!EqualRect(&nowRect, &thenRect))) {
			FrameRect(&thenRect);
			index = index < 7 ? index + 1 : 0;
			PenPat(&patterns[index]);
			FrameRect(&nowRect);
			thenRect = nowRect;
		}
	}
	FrameRect(&thenRect);
	
	SetPenState(&penState);
	*resultRect = thenRect;
}

SetMobiusRect(rect, left, top, right, bottom)
	Rect	*rect;
	short	left, top, right, bottom;
{
	if (left > right) {
		rect->left = right;
		rect->right = left;
	} else {
		rect->left = left;
		rect->right = right;
	}
	if (top > bottom) {
		rect->top = bottom;
		rect->bottom = top;
	} else {
		rect->top = top;
		rect->bottom = bottom;
	}
}
