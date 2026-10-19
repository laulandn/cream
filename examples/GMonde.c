/******************************************************************************\
*
*	Apple Macintosh Developer Technical Support
*
*	Source file for the GMonde application
*
*	Program:	GMonde
*	File:		GMonde.c - C Implementation
*
*	by:			Forrest Tanaka
*
*	Copyright © 1988-1991 Apple Computer, Inc.
*	All rights reserved.
*
* ------------------------------------------------------------------------------
*
*     GMonde is a sample program showing a fairly typical use of GWorlds.  A
* single window shows up and displays a set of color ramps.  When the user
* clicks and drags in the window, a selection marquee follows the mouse with one
* corner at the location that the mouse was first pressed and the opposite
* corner at the current location of the mouse.  As the user drags the selection
* marquee, the classic “marching ants” animation occurs.  The color ramp image
* isn’t damaged by the selection marquee being dragged over it.  When the user
* releases the mouse button, the selection marquee remains at the location and
* size that the user selected, and the marching ants animation continues.
*
*     The color ramp image is stored in an off-screen GWorld stored in the
* gOffScreen global variable.  During any update events, this GWorld is copied
* to the window specified by the gDemoWindow global variable.
*
*     When the user begins the drag the dragger, a temporary off-screen GWorld
* is created and the source image in gOffScreen is copied to it.  The selection
* marquee is drawn into the temporary GWorld, and then the temporary GWorld is
* copied to the window.  When the user moves the mouse with the mouse button
* still held down, the original GWorld is copied into the temporary GWorld, the
* selection marquee is drawn into the temporary GWorld at the location of the
* mouse, and then the temporary GWorld is copied to the window.  This is done as
* long as the mouse button is held down and the user moves the mouse.  Once the
* user releases the mouse, the temporary GWorld is disposed of.
*
*     The GWorld routines can assure that an off-screen GWorld is set up so that
* a CopyBits call between the GWorld and a portion of a screen is as fast as
* possible.  To do this, the color table of the GWorld must be equal to the
* color table of the destination screen on a Color QuickDraw machine, the pixel
* depth of the GWorld must equal the pixel depth of the destination screen, and
* the alignment of the pixels in memory in the GWorld must be the same as the
* alignment of the pixels on the Screen.  If you pass 0 for a pixel depth,
* NewGWorld creates a GWorld that satisfies all three of these requirements.
* UpdateGWorld maintains this if you pass it 0 for a pixel depth.  This
* application takes advantage of this capability.  When gOffScreen is created,
* the portRect of gDemoWindow is converted to global coordinates and passed to
* NewGWorld, along with a pixel depth of 0.  gOffScreen can then be copied to
* gDemoWindow at the fastest possible speed.
*
*     When gDemoWindow is dragged to a new screen location, the pixels of
* gOffScreen might not have the same memory alignment as the pixels of
* gDemoWindow.  To bring gOffScreen back into alignment, UpdateGWorld is called
* with a pixel depth of 0 after gDemoWindow is dragged to a new location.
*
*     If the user switches out of GMonde and uses the Monitors control panel to
* switch to a new pixel depth or to switch between Grays mode and Colors mode,
* gOffScreen must be updated to these new characteristics so that a CopyBits
* call from gOffScreen to gDemoWindow remains as fast as possible.  UpdateGWorld
* is again used to do this.  But how can you tell that the screen depth or color
* table has changed?  An update event can tell you this.  When the user switches
* the depth or color table, the entire screen is redrawn and every window gets
* an update event.  When gDemoWindow receives an update event, UpdateGWorld is
* called just in case the event was caused by a change in the screen depth or
* color table.  You might want to put more intelligence in this by saving the
* current screen depth of the deepest screen the gDemoWindow intersects, then
* checking the screen depth on every update event.  If the screen depth changes,
* then call UpdateGWorld.  For GMonde, I decided not to do this.
*
*     Under system software version 7.0, this application can run on a Macintosh
* without Color QuickDraw.  All the GWorld routines work, albeit only a pixel
* depth of 1 is supported.  RGBForeColor works, although the specified colors
* are thresholded to black or white.  Also, version 2 PICTs can be drawn on a
* Macintosh without Color QuickDraw, though GMonde doesn’t show that.  Version
* 1.0 did though.  I debated about that decision, but figured all it took was a
* DrawPicture to prove that it works.  I thought it was more interesting that
* RGBForeColor works.  Nodamene?
\******************************************************************************/

//#ifdef applec
#include <AppleEvents.h>
#include <Events.h>
#include <Fonts.h>
#include <Gestalt.h>
#include <Memory.h>
#include <Menus.h>
#include <OSUtils.h>
#include <QDOffscreen.h>
#include <Quickdraw.h>
#include <Script.h>
#include <ToolUtils.h>
#include <Windows.h>
/*#else
#include <AppleEvents.h>
#include <ColorToolbox.h>
#include <EventMgr.h>
#include <Gestalt.h>
#include <GraphDevMgr.h>
#include <ToolboxUtil.h>
#include <WindowMgr.h>
#endif*/


/******************************************************************************
* Constant Declarations
******************************************************************************/

/* Resource IDs */
#define rDemoWindID    128 /* Demonstration window WIND resource ID */
#define rMarqueePattID 128 /* Marquee pattern PAT  resource ID */

/* Specifications for drawing the color ramps */
#define kMaxRampCount 256 /* Number of steps in a ramp */
#define kRampWidth    16  /* Width of a ramp in pixels */

/* Miscellaneous constants */
#define kMinScrollTime 1    /* # ticks between marquee pattern scrolls */
#define kResumeMask    1    /* Mask used to tell if got suspend/resume event */
#define kUseTempMem    true /* Pass to CreateOffScreen; GWorld in temp mem */

#define keyMissedKeywordAttr 'miss'

#ifndef applec
#define nil 0 /* Generic nil */
#endif

#ifdef applec
#define thePort    qd.thePort
#define gray       qd.gray
#define screenBits qd.screenBits
#endif

/* Macros to get the top-left and bottom-right corners of a rectangle */
#ifdef applec
#define topLeft(r) (*((Point *) &(r).top))
#define botRight(r) (*((Point *) &(r).bottom))
#endif

 
/******************************************************************************
* Global Variable Definitions
******************************************************************************/

WindowPtr gDemoWindow;  /* Pointer to display window */
GDHandle  gDemoDevice;  /* Handle to the normal GDevice */
GWorldPtr gOffScreen;   /* Pointer to off-screen GWorld */
Rect      gMarqueeRect; /* Position of the dragger in window coordinates */
Pattern   gMarqueePatt; /* Marquee diagonal stripe pattern */
Boolean   gQuitting;    /* True if GMonde is quitting */
Boolean   gHasGWorlds;  /* True if GWorlds are implemented */
Boolean   gHasCQD;      /* True if Color QuickDraw is implemented */


/******************************************************************************
* Function Prototypes
******************************************************************************/

void        main (void);
static void        StartUp (void);
static void        MainEventLoop (void);
static pascal void HandleAEquit (AppleEvent *quitAppleEvent,
                          AppleEvent *reply,
                          long       handlerRefCon);
static OSErr       DoneRequiredParams (AppleEvent *anAppleEvent);
static WindowPtr   CreateDemoWindow (void);
static GWorldPtr   CreateOffScreen (GrafPtr baseWindow,
                             short   depth,
                             Boolean temporary);
static void        PaintImage (void);
static void        TrackMarquee (GWorldPtr srcImage,
                          WindowPtr dstImage,
                          Point     startPt,
                          Rect      *finalRect);
static void        ScrollPattern (Pattern aPattern);
static QDErr       UpdateOffScreen (GWorldPtr   *offScreen,
                             GWorldFlags *returnFlags,
                             WindowPtr   baseWindow);


/******************************************************************************
* Main Program
******************************************************************************/

void
int main(int argc,char **argv)
	{
	StartUp ();
	if (gHasGWorlds)
		{
		gDemoWindow = CreateDemoWindow ();
		if (gDemoWindow != nil)
			{
			SetPort (gDemoWindow);
			gOffScreen = CreateOffScreen (gDemoWindow, 0, !kUseTempMem);
			if (gOffScreen != nil)
				{
				GetGWorld (/*<*/(CGrafPtr *) &gDemoWindow, /*<*/&gDemoDevice);
				SetGWorld (gOffScreen, (GDHandle) nil);
				if (LockPixels (GetGWorldPixMap (gOffScreen)))
					{
					PaintImage ();
					UnlockPixels (GetGWorldPixMap (gOffScreen));
					}
				SetGWorld ((GWorldPtr) gDemoWindow, gDemoDevice);
				MainEventLoop ();
				}
			}
		}
	}


/******************************************************************************\
* StartUp - Start up our application
*
*     The usual things that have to be done as soon as an application starts are
* done here.  A few of the managers are initialized, and then the graphics
* environment is checked.
*
*     The global variable gHasCQD is set to true if the machine GMonde is
* running on has Color QuickDraw, false if not.  This is determined through
* Gestalt.  If Gestalt isn’t working for some reason, then SysEnvirons is used
* instead.  The global variable gHasGWorlds is set to true if the GWorlds are
* implemented on the machine that GMonde is running on.  GWorlds are implemented
* if Gestalt returns gestalt32BitQD or greater, and if Gestalt returns a value
* between gestaltOriginalQD and gestalt8BitQD, exclusive.  Gestalt returns a
* value in this range if GMonde is running on a classic QuickDraw machine
* running system software version 7.0 or greater.
*
*     The AppleEvent ‘quit’ handler is installed here.  When an incoming ‘quit’
* AppleEvent is processed, control is routed to the HandleAEquit function that’s
* defined elsewhere in this program.
\******************************************************************************/

static void
StartUp ()
	{
	long      qdVersion;  /* Version of QuickDraw on this machine */
	PatHandle marquee;    /* Diagonal striped marquee pattern  resource */
	long      aeAttr;     /* AppleEvents attributes */
	OSErr     result;

	InitGraf (&thePort);
	InitFonts ();
	InitWindows ();
	InitMenus ();

	/* Find out if GWorlds and CQD are implemented on this machine */
	(void) Gestalt (gestaltQuickdrawVersion, /*<*/&qdVersion);
	gHasGWorlds = (qdVersion > gestaltOriginalQD && qdVersion < gestalt8BitQD)
	              || qdVersion >= gestalt32BitQD;
	gHasCQD = qdVersion >= gestalt8BitQD;

	/* Copy the marquee pattern into gMarqueePatt, or 50% gray if not found */
	marquee = GetPattern (rMarqueePattID);
	if (marquee != nil)
		BlockMove ((Ptr) *marquee, (Ptr) gMarqueePatt, sizeof (Pattern));
	else
		BlockMove ((Ptr) gray, (Ptr) gMarqueePatt, sizeof (Pattern));

	/* Install the AppleEvent handler */
	result = Gestalt (gestaltAppleEventsAttr, /*<*/&aeAttr);
	if (result == noErr && (aeAttr & (1L << gestaltAppleEventsPresent)))
		result = AEInstallEventHandler (kCoreEventClass, kAEQuitApplication,
				(EventHandlerProcPtr) HandleAEquit, 0, false);

	/* The initial marquee rectangle is empty */
	SetRect (/*<*/&gMarqueeRect, 0, 0, 0, 0);
	}


/******************************************************************************\
* MainEventLoop - Main event loop
*
*     This function collects events, does something on events that it knows
* about, and exits when the user clicks on the close box of the window.
*
*     If the user clicks in the content region of the window, then TrackMarquee
* is called to allow the user to drag out a selection marquee.
*
*     If the user clicks in the title bar, the user is allowed to drag the
* window around in the usual manner.  After the window is moved, UpdateOffScreen
* is called so that if the window was moved to a screen with a depth different
* than the one it was on, then the off-screen GWorld containing the image of the
* ramps is updated to the depth of that screen.  If the window is moved so that
* it crosses more than one screen, then the off-screen GWorld is set to the
* depth and color table of the deepest screen that’s crossed.  That leads to an
* interesting situation if the window is moved so that it’s partially on a
* screen of a deeper depth than the one it was entirely contained on because the
* depth of the GWorld changes, yet the part of the image that remains on the old
* screen is from the old depth of the GWorld.  That can lead to some weird
* window updating problems.  So in that case, I invalidate the entire window so
* that all of the image is redrawn from the new GWorld depth.
*
*     If an update event comes in for the display window, UpdateOffScreen is
* called in case the update event is caused by the user using the Monitors
* control panel to change the depth or color table of the screen that the window
* is on.  Similar to the case in which the window is dragged so that it’s partly
* on a deeper screen, the entire window is invalidated if the GWorld’s depth or
* color table changes.
*
*     If a suspend event comes in, then the GWorld is copied to the window so
* that the selection marquee, if any, is erased.  When GMonde is resumed, the
* usual marching ants animation resumes.
*
*     If there’s an active selection marquee, then the marquee pattern is
* rotated one pixel and the marquee rectangle is redrawn using this rotated
* pattern.  This is done once per iteration of the main event loop.  To make the
* ants march as smoothly as possible, the WaitNextEvent sleep time is set to 0
* to give GMonde as much time as possible.  If there is no active selection,
* then it’s kind of impolite to ask for that much time, so the sleep time is set
* to 60 if there’s no selection.
\******************************************************************************/

static void
MainEventLoop ()
	{
	EventRecord anEvent;     /* Incoming events */
	WindowPtr   evtWind;     /* Pointer to window relevant to event */
	short       clickArea;   /* Area that mouse was clicked (i.e. menu bar) */
	Point       clickPos;    /* Location of mouse click in local coords */
	Byte        osEvtKind;   /* Kind of OSEvent; mouse-moved or suspend/resume */
	GWorldFlags resultFlags; /* GWorld flags on return of UpdateGWorld call */
	Rect        screenRect;  /* Rectangle of screen in global coordinates */
	long        scrollTime;  /* TickCount @ time of last marquee patt scroll */
	long        dreamTime;   /* # ticks that WaitNextEvent can hold us up */
	Boolean     wereInFront; /* True if GMonde is front-most application */
	QDErr       error;       /* Result of QuickDraw operations */

	/* Initialize our variables for the main event loop */
	gQuitting = false;
	error = noErr;
	scrollTime = 0;
	wereInFront = true;

	/* Continue the main event loop until user quits or an error occurs */
	InitCursor ();
	while ((!gQuitting) && (error == noErr))
		{
		/* If marquee is active, then need more time to march the ants */
		if (EmptyRect (&gMarqueeRect))
			dreamTime = 60;
		else
			dreamTime = 0;

		/* If a non-null event, handle it; otherwise, just march the ants */
		if (WaitNextEvent (everyEvent, /*<*/&anEvent, dreamTime,
				(RgnHandle) nil))
			{
			if (anEvent.what == mouseDown)
				{
				/* React to content clicks, title clicks, close box clicks */
				clickArea = FindWindow (anEvent.where, /*<*/&evtWind);
				if (clickArea == inContent)
					{
					/* Click in content region; let user drag marquee around */
					clickPos = anEvent.where;
					GlobalToLocal (/*◊*/&clickPos);
					TrackMarquee (gOffScreen, evtWind, clickPos,
							/*<*/&gMarqueeRect);
					}

				else if (clickArea == inDrag)
					{
					/* Let the user drag the window around */
					screenRect = (**GetGrayRgn ()).rgnBBox;
					DragWindow (evtWind, anEvent.where, &screenRect);

					/* In case window moved to other screen, update GWorld */
					error = UpdateOffScreen (/*◊*/&gOffScreen,
							/*<*/&resultFlags, evtWind);
					if (error == noErr)
						if ((newDepth & resultFlags) || (mapPix & resultFlags))
							{
							/* Window now in screen w/ diff depth; redraw */
							SetPort (evtWind);
							InvalRect (&evtWind->portRect);
							}
					}

				else if (clickArea == inGoAway)
					/* Click in close box means we’re outta here */
					if (TrackGoAway (evtWind, anEvent.where))
						gQuitting = true;
				}

			else if (anEvent.what == updateEvt)
				{
				evtWind = (WindowPtr) anEvent.message;

				/* Might be update because of screen depth or clut changing */
				error = UpdateOffScreen (/*◊*/&gOffScreen, /*<*/&resultFlags,
						evtWind);
				if (error == noErr)
					/* If screen depth/clut changed, redraw whole window */
					if ((newDepth & resultFlags) || (mapPix & resultFlags))
						{
						SetPort (evtWind);
						InvalRect (&evtWind->portRect);
						}

				BeginUpdate (evtWind);
				SetPort (evtWind);

				/* Now draw the GWorld to the window */
				if (LockPixels (GetGWorldPixMap (gOffScreen)))
					{
					CopyBits (&((GrafPtr) gOffScreen)->portBits, &evtWind->
							portBits, &gOffScreen->portRect, &evtWind->
							portRect, srcCopy | ditherCopy, nil);
					UnlockPixels (GetGWorldPixMap (gOffScreen));
					}

				EndUpdate (evtWind);
				}

			else if (anEvent.what == osEvt)
				{
				osEvtKind = (anEvent.message >> 24) & 0x00FF;
				if ((osEvtKind == suspendResumeMessage) && (anEvent.message &
						kResumeMask))
					wereInFront = true;
				else
					{
					evtWind = FrontWindow ();
					SetPort (evtWind);
					if (LockPixels (GetGWorldPixMap (gOffScreen)))
						{
						CopyBits (&((GrafPtr) gOffScreen)->portBits, &evtWind->
								portBits, &gOffScreen->portRect, &evtWind->
								portRect, srcCopy | ditherCopy, nil);
						UnlockPixels (GetGWorldPixMap (gOffScreen));
						}
					wereInFront = false;
					}

				}

			else if (anEvent.what == kHighLevelEvent)
				(void) AEProcessAppleEvent (&anEvent);
			}

		/* If have marquee and enough time since last ant march, march ants */
		if (!EmptyRect (&gMarqueeRect) && (TickCount () - scrollTime >
				kMinScrollTime) && wereInFront)
			{
			ScrollPattern (gMarqueePatt);
			PenPat (gMarqueePatt);
			FrameRect (&gMarqueeRect);
			scrollTime = TickCount ();
			}
		}
	}


/******************************************************************************\
* HandleAEquit - Handler for 'quit' AppleEvent
*
*     This is the AppleEvent handler for the 'quit' AppleEvent as passed in the
* quitAppleEvent parameter by the AppleEvent Manager.  The DoQuit routine is
* called which causes this application to quit at the start of the next
* iteration of the main event loop.
*
*     Though the quit AppleEvent doesn’t contain any parameters, the standard
* thing to do in reaction to any AppleEvent is to check to see if there are any
* required parameters in the AppleEvent that this routine doesn’t recognise.
* DoneRequiredParms checks for this condition and returns an error if there are
* in fact required parameters in the AppleEvent or if some other error occurs
* during the check.
\******************************************************************************/

static pascal void
HandleAEquit (quitAppleEvent, reply, handlerRefCon)
	AppleEvent *quitAppleEvent; /* Contains the ‘quit’ AppleEvent */
	AppleEvent *reply;          /* Returns reply; ignored */
	long       handlerRefCon;   /* Application-defined parameter; ignored */
	{
#pragma unused (reply, handlerRefCon)
	/* quit AE has no parms, but check in case the client requires any */
	if (DoneRequiredParams (quitAppleEvent) == noErr)
		gQuitting = true;
	}


/******************************************************************************\
* DoneRequiredParams - Done processing required params; OK?
*
*     DoneRequiredParams checks to see if the AppleEvent specified by the
* anAppleEvent parameter has any required parameters that we haven’t yet
* processed.  If there aren’t any left, then noErr is returned.  If there are
* required parameters that haven’t been processed yet, then errAEEventNotHandled
* is returned.  If any other errors occur, then that error code is returned.
\******************************************************************************/

static OSErr
DoneRequiredParams (anAppleEvent)
	AppleEvent *anAppleEvent; /* AppleEvent being checked */
	{
	DescType typeCode;   /* Type of AppleEvent attribute found; ignored */
	Size     actualSize; /* Actual size of parameters; ignored */
	OSErr    error;

	/* Are there any required parameters in AppleEvent we didn’t process? */
   	error = AEGetAttributePtr (anAppleEvent, keyMissedKeywordAttr, typeWildCard,
			/*<*/&typeCode, nil, 0, /*<*/&actualSize);
	if (error == errAEDescNotFound)
		/* No required parameters left, so no error */
		error = noErr;
	else if (error == noErr)
		/* There was at least one required parameter we didn’t process */
		error = errAEEventNotHandled;
	return error;
	}


/******************************************************************************\
* CreateDemoWindow - Create a centered demonstration window
*
*     A new window is created and displayed on the main screen.  It is
* horizontally centered and on the upper third of the main screen.  A pointer to
* this window is returned.  This window is also set as the current GrafPort
* before it returns.  If the window couldn’t be created for some reason, nil is
* returned.
*
*     On Color QuickDraw machines, a color window is created.  On machines
* without Color QuickDraw, a normal window is created.  The gHasCQD global
* variable is used to determine whether Color QuickDraw is implemented on this
* machine or not, so gHasCQD must be properly initialized before
* CreateDemoWindow is called.
\******************************************************************************/

static WindowPtr
CreateDemoWindow ()
	{
	WindowPtr aWindow;   /* Pointer to new Marching Ants window */
	Ptr       windStore; /* Pointer to WindowRecord memory */

	/* Assume failure, as I always do in life */
	aWindow = nil;

	/* Create window storage before GetNewWindow to avoid heap fragmentation */
	windStore = NewPtr (sizeof (WindowRecord));
	if (windStore != nil)
		{
		/* If CQD, make color window, otherwise make normal window */
		if (gHasCQD)
			aWindow = GetNewCWindow (rDemoWindID, windStore, (WindowPtr) -1);
		else
			aWindow = GetNewWindow (rDemoWindID, windStore, (WindowPtr) -1);

		if (aWindow != nil)
			{
			/* Center window on the main screen */
			MoveWindow (aWindow, ((screenBits.bounds.right - screenBits.
					bounds.left) - (aWindow->portRect.right - aWindow->portRect.
					left)) / 2, ((screenBits.bounds.bottom - screenBits.
					bounds.top - GetMBarHeight ()) - (aWindow->portRect.bottom -
					aWindow->portRect.top)) / 3 + GetMBarHeight (), false);

			/* Make the window visible and make it the current GrafPort */
			ShowWindow (aWindow);
			SetPort (aWindow);
			}
		else
			DisposPtr (windStore);
		}
	return aWindow;
	}


/******************************************************************************\
* CreateOffScreen - Create an off-screen GWorld to buffer a window’s image
*
*     An off-screen GWorld is created so that it can be used to buffer the image
* in the GrafPort specified by basePort.  To do that, I take advantage of
* NewGWorld’s pixel-alignment capabilities.  If you pass zero as the pixel depth
* in the depth parameter and convert the portRect of basePort to global
* coordinates, the GWorld is created a pixel depth equal to the deepest screen
* that the portRect intersects, a color table to match that of the screen’s, and
* pixels aligned so that a CopyBits call to copy pixels between the screen and
* this GWorld is as fast as possible.
*
*     Starting with 32-Bit QuickDraw 1.2 (which was first included with system
* software version 6.0.5), you could ask NewGWorld to allocate the pixel image
* in temporary memory (formerly called “MultiFinder Temporary Memory”) by
* passing useTempMem in the GWorld flags.  If you pass true in ‘temporary”,
* NewGWorld will be called with the useTempMem flag set.  Otherwise, NewGWorld
* will be called normally.  As usual, pixel images allocated in temporary memory
* should only be used for transient GWorlds.
*
*     I set the clip region of the new GWorld to be coincident with its portRect
* as a matter of habit as soon as I create a GWorld.  The reason for this is
* because UpdateGWorld, at least as of this writing, resizes the clip region of
* a GWorld if you specify the stretchBits flag.  NewGWorld initializes the clip
* region to the entire QuickDraw coordinate plane.  When UpdateGWorld resizes
* the GWorld larger with the stretchBits flag on, it also resizes the GWorld’s
* clip region proportionately larger.  If the clip region already covers the
* entire coordinate plane, the coordinates will get wrapped around and the clip
* region will be set to an empty region, and so nothing can be drawn into that
* GWorld.  By setting the clip region to the portRect right from the start, this
* problem never happens.
\******************************************************************************/

static GWorldPtr
CreateOffScreen (basePort, depth, temporary)
	WindowPtr basePort;  /* Pointer to the window to base off-screen on */
	short     depth;     /* Desired depth of pixel map in bits per pixel */
	Boolean   temporary; /* true if allocate GWorld image in temp memory */
	{
	CGrafPtr    currPort;    /* Pointer to the saved port */
	GDHandle    currGDevice; /* Handle to the current GDevice */
	GWorldPtr   offScreen;   /* Pointer to our GWorld */
	Rect        offRect;     /* portRect of basePort in global coordinates */
	GWorldFlags tempFlags;   /* Flag indicating if temp memory is to be used */
	QDErr       result;      /* Error return from NewGWorld */

	GetGWorld (/*<*/&currPort, /*<*/&currGDevice);
	SetPort (basePort);

	/* Globalize portRect of basePort */
	offRect = basePort->portRect;
	if (depth == 0)
		{
		LocalToGlobal (/*◊*/&topLeft(offRect));
		LocalToGlobal (/*◊*/&botRight(offRect));
		}

	/* Set tempFlags to reflect desire for temporary memory */
	if (temporary)
		tempFlags = useTempMem;
	else
		tempFlags = 0;

	/* Create the new off-screen port and set it as the current port */
	result = NewGWorld (/*<*/&offScreen, depth, &offRect, nil, nil, tempFlags);
	if (result == noErr)
		{
		SetGWorld (offScreen, (GDHandle) nil);

		/* Good idea to set the clip region to the portRect of the new GWorld */
		ClipRect (&offScreen->portRect);

		/* Clear the new GWorld to white */
		if (LockPixels (GetGWorldPixMap (offScreen)))
			{
			EraseRect (&offScreen->portRect);
			UnlockPixels (GetGWorldPixMap (offScreen));
			}
		}
	else
		offScreen = nil;

	/* Reset current GrafPort and GDevice to what it was when we were called */
	SetGWorld (currPort, currGDevice);
	return offScreen;
	}


/******************************************************************************\
* PaintImage - Paint color ramps into the current GrafPort
*
*     PaintImage paints a series of color ramps into the current GrafPort.  To
* showcase the error diffusion dithering that CopyBits can perform on Color
* QuickDraw machines, the image isn’t drawn directly to the current GrafPort if
* Color QuickDraw is available.  Instead, a temporary 32-bit deep GWorld is
* created and the ramps are drawn into it.  When that’s done, the 32-bit deep
* GWorld is copied to the current GrafPort with the ditherMode modifier on.  If
* there isn’t enough memory for the temporary GWorld or if Color QuickDraw isn’t
* available, then the ramps are drawn directly to the current GrafPort.
\******************************************************************************/

static void
PaintImage ()
	{
	GWorldPtr rampWorld;    /* 32-bit deep GWorld to draw ramps into */
	CGrafPtr  savedPort;    /* Pointer to saved CGrafPort for later restoring */
	GDHandle  savedGDevice; /* Handle to saved GDevice for later restoring */
	RGBColor  rampColor;    /* Color to use when drawing a step of the ramp */
	short     rampFactor;   /* Value to set a color component to in a ramp */
	short     pinFactor;    /* Value to pin a color component to in a ramp */
	short     x;            /* Horizontal coordinate to draw ramp step */
	short     y;            /* Vertical coordinate to draw ramp step */

	GetGWorld (/*<*/&savedPort, /*<*/&savedGDevice);

	/* Create the offscreen to draw ramps if Color QuickDraw available */
	if (gHasCQD)
		{
		/* Try to make the temporary GWorld in application heap */
		rampWorld = CreateOffScreen ((GrafPtr) savedPort, 32, !kUseTempMem);

		/* If that didn’t work, try allocating the GWorld in temporary memory */
		if (rampWorld == nil)
			rampWorld = CreateOffScreen ((GrafPtr) savedPort, 32, kUseTempMem);

		/* If got GWorld, set it, otherwise trash it */
		if (rampWorld != nil)
			if (LockPixels (GetGWorldPixMap (rampWorld)))
				SetGWorld (rampWorld, nil);
			else
				{
				DisposeGWorld (rampWorld);
				rampWorld = nil;
				}
		}
	else
		rampWorld = nil;

	/* Pin component values at $0000 first, then at $FFFF */
	for (pinFactor = 0; pinFactor >= -1; pinFactor--)
		{
		/* Loop on y coordinate over entire GrafPort */
		for (y = 0; y < kMaxRampCount; y++)
			{
			/* Calculate value of components that are dynamic in a ramp */
			rampFactor = y * 65535 / kMaxRampCount;
			if (pinFactor == -1)
				{
				/* If pinning at $FFFF, then use one’s comp of rampFactor */
				rampFactor ^= 0xFFFF;
				x = kRampWidth;
				}
			else
				x = 0;

			rampColor.red = rampFactor;
			rampColor.green = rampFactor;
			rampColor.blue = rampFactor;
			RGBForeColor (&rampColor);
			MoveTo (x, y);
			Line (kRampWidth, 0);

			x += kRampWidth * 2;
			rampColor.blue = pinFactor;
			RGBForeColor (&rampColor);
			MoveTo (x, y);
			Line (kRampWidth, 0);

			x += kRampWidth * 2;
			rampColor.green = pinFactor;
			RGBForeColor (&rampColor);
			MoveTo (x, y);
			Line (kRampWidth, 0);

			x += kRampWidth * 2;
			rampColor.blue = rampFactor;
			RGBForeColor (&rampColor);
			MoveTo (x, y);
			Line (kRampWidth, 0);

			x += kRampWidth * 2;
			rampColor.red = pinFactor;
			RGBForeColor (&rampColor);
			MoveTo (x, y);
			Line (kRampWidth, 0);

			x += kRampWidth * 2;
			rampColor.green = rampFactor;
			RGBForeColor (&rampColor);
			MoveTo (x, y);
			Line (kRampWidth, 0);

			x += kRampWidth * 2;
			rampColor.blue = pinFactor;
			RGBForeColor (&rampColor);
			MoveTo (x, y);
			Line (kRampWidth, 0);

			x += kRampWidth * 2;
			rampColor.green = pinFactor;
			RGBForeColor (&rampColor);
			MoveTo (x, y);
			Line (kRampWidth, 0);
			}
		}

	/* If drawing into rampWorld, then CopyBits it to the current GrafPort */
	if (rampWorld != nil)
		{
		SetGWorld (savedPort, savedGDevice);
		CopyBits (&((GrafPtr) rampWorld)->portBits, &((GrafPtr) savedPort)->
				portBits, &rampWorld->portRect, &savedPort->portRect, srcCopy |
				ditherCopy, nil);
		UnlockPixels (GetGWorldPixMap (rampWorld));
		DisposeGWorld (rampWorld);
		}
	}


/******************************************************************************\
* TrackMarquee - Track the dragging of the marquee
*
*     As long as the user holds down the mouse button, a selection marquee is
* drawn with one corner at the location that the mouse was pressed and the
* opposite corner at the location that the mouse currently is.  The off-screen
* image that the dragger is being drawn over (you could think of this as the
* document’s data) must be specified by srcImage.  The window that the user will
* see the image and the selection marquee must be specified by dstImage.  Every
* time the mouse is moved, srcImage is copied into a temporary GWorld.  This is
* where the actual marquee animation takes place.  The marquee is then drawn
* into the temporary GWorld, then that is copied to the window specified by
* dstImage.  The next time the mouse is moved, srcImage is again copied into the
* temporary GWorld which erases the old marquee and the process repeats.
*
*     To optimize the speed of the tracking, only the portion of the GWorlds and
* screen that are affected by the marquee are copied.  So if the selection
* marquee is small, only a small portion of the GWorlds are copied.  The maxRect
* local variable holds the union of the marquee in the previous iteration and
* the current iteration, and it’s this rectangle that’s used as the bounds of
* the CopyBits.  That assures that the marquee in the previous iteration is
* erased.
\******************************************************************************/

static void
TrackMarquee (srcImage, dstImage, startPt, finalRect)
	GWorldPtr srcImage;   /* Pointer to GWorld with source image */
	WindowPtr dstImage;   /* Pointer to window that animation is visible in */
	Point     startPt;    /* Starting location of mouse in window coordinates */
	Rect      *finalRect; /* Marquee rectangle when user released the mouse */
	{
	Point     newPt;          /* Current location of mouse in local coords */
	Point     oldPt;          /* Mouse loc in last iteration; local coords */
	CGrafPtr  currPort;       /* Current GrafPort for restoring */
	GDHandle  currGDevice;    /* Handle to current GDevice for restoring */
	GWorldPtr animationWorld; /* Temporary GWorld used for smooth animation */
	long      scrollTime;     /* Time of last scroll */
	Rect      marqueeRect;    /* Rectangle of marquee */
	Rect      maxRect;        /* Union of current marqueeRect and previous */

	SetPort (dstImage);
	GetGWorld (/*<*/&currPort, /*<*/&currGDevice);

	/* Create the animation offscreen */
	animationWorld = CreateOffScreen (dstImage, 0, !kUseTempMem);

	/* If that didn’t work, try allocating the GWorld in temporary memory */
	if (animationWorld == nil)
		animationWorld = CreateOffScreen (dstImage, 0, kUseTempMem);

	/* If THAT didn’t work, give up */
	if (animationWorld != nil)
		{
		if (LockPixels (GetGWorldPixMap (animationWorld)))
			{
			/* Prime the animation world */
			SetGWorld (animationWorld, nil);
			if (LockPixels (GetGWorldPixMap (srcImage)))
				{
				CopyBits (&((GrafPtr) srcImage)->portBits, &((GrafPtr)
						animationWorld)->portBits, &srcImage->portRect,
						&animationWorld->portRect, srcCopy, nil);
				UnlockPixels (GetGWorldPixMap (srcImage));
				}

			/* Draw first frame to the window so that old marquee is erased */
			SetGWorld ((CGrafPtr) dstImage, currGDevice);
			CopyBits (&((GrafPtr) animationWorld)->portBits, &dstImage->
					portBits, &animationWorld->portRect, &dstImage->portRect,
					srcCopy | ditherCopy, nil);

			/* Prime the loop variables */
			oldPt = newPt = startPt;
			scrollTime = 0;
			Pt2Rect (startPt, startPt, /*<*/&marqueeRect);
			maxRect = marqueeRect;

			/* Keep tracking until the mouse button is released */
			while (StillDown ())
				{
				GetMouse (/*◊*/&newPt);

				/* Only redraw the marquee if the mouse moved */
				if (!EqualPt (oldPt, newPt))
					{
					/* Erase the old temporary image */
					SetGWorld (animationWorld, (GDHandle) nil);
					if (LockPixels (GetGWorldPixMap (srcImage)))
						{
						CopyBits (&((GrafPtr) srcImage)->portBits,
								&((GrafPtr) animationWorld)->portBits,
								&marqueeRect, &marqueeRect, srcCopy,
								(RgnHandle) nil);
						UnlockPixels (GetGWorldPixMap (srcImage));
						}

					/* Draw the new dragger position */
					SetGWorld (animationWorld, nil);
					PenPat (gMarqueePatt);
					Pt2Rect (startPt, newPt, /*<*/&marqueeRect);
					FrameRect (&marqueeRect);
					UnionRect (&marqueeRect, &maxRect, /*<*/&maxRect);

					/* Draw the new frame to the window */
					SetGWorld ((CGrafPtr) dstImage, currGDevice);
					CopyBits (&((GrafPtr) animationWorld)->portBits, &dstImage->
							portBits, &maxRect, &maxRect, srcCopy | ditherCopy,
							nil);

					oldPt = newPt;
					maxRect = marqueeRect;
					}

				/* If enough time since last ant march, then march ants */
				if (TickCount () - scrollTime > kMinScrollTime)
					{
					ScrollPattern (gMarqueePatt);
					PenPat (gMarqueePatt);
					FrameRect (&marqueeRect);
					scrollTime = TickCount ();
					}
				}
			UnlockPixels (GetGWorldPixMap (animationWorld));
			}
		DisposeGWorld (animationWorld);
		}

	/* Reset current GWorld to what it was at routine entry */
	SetGWorld (currPort, currGDevice);

	/* Return dragged rect in finalRect, or set empty if dragged rect empty */
	if (EqualPt (startPt, newPt))
		SetRect (/*<*/finalRect, 0, 0, 0, 0);
	else
		Pt2Rect (startPt, newPt, /*<*/finalRect);
	}


/******************************************************************************\
* ScrollPattern - Scroll a pattern for the marchine ants effect
*
*     This routine rotates an old eight-byte pattern down one pixel, moving the
* bottom row of pixels to the top.  This is done to with the marquee pattern so
* that the ants march.
\******************************************************************************/

static void
ScrollPattern (aPattern)
	Pattern aPattern; /* Address of pattern to scroll */
	{
	Byte tempByte; /* Holds one byte of pattern for rotation */

	tempByte = aPattern [7];
	BlockMove ((Ptr) &aPattern [0], (Ptr) &aPattern [1], sizeof (Pattern) - 1);
	aPattern [0] = tempByte;
	}


/******************************************************************************\
* UpdateOffScreen - Update GWorld to screen characteristics of a window
*
*     UpdateOffScreen updates the off-screen GWorld specified by offScreen so
* that it has the same depth and color table of the deepest screen that
* intersects the window specified by baseWindow.  Also, the pixels of offScreen
* are properly aligned so that a CopyBits call from the off-screen to the screen
* is as fast as possible.  If enough of a change is made to offScreen, then a
* completely new GWorld will be made by UpdateGWorld, and a pointer to it is
* returned back in offScreen.
*
*     This routine is really a showcase for the UpdateGWorld routine.  It’s this
* routine, introduced with 32-Bit QuickDraw 1.0, that handles updating the
* characteristics of the GWorld to the screen characteristics and pixel
* alignment of a rectangle on the screen, normally a window’s portRect.
*
*     You can see that I’m passing clipPix in the GWorld flags.  I never resize
* the GWorld, so it would seem that asking UpdateGWorld to clip the existing
* image to the new size of the GWorld is a fine waste of time.  But what I’m
* asking UpdateGWorld to do is to retain the existing image.  If I had passed an
* empty set for the GWorld flags, UpdateGWorld could throw out the existing
* image if it sees fit to do so.  The result of UpdateGWorld is returned in
* returnFlags.
*
*     If UpdateGWorld had to change the depth of the GWorld, it returns the
* newDepth flag.  If it had to change the color table of the GWorld, it returns
* the mapPix flag. I decided that, for this sample code, I would completely
* redraw the picture into the GWorld if either of these things had to be done.
* UpdateGWorld is completely capable of dithering the image to the new depth and
* color table, but that only helps you if UpdateGWorld makes the GWorld
* shallower than it was--it doesn’t do much for you if it made the GWorld
* deeper.  That’s why I decided to redraw the picture in either case.
\******************************************************************************/

static QDErr
UpdateOffScreen (offScreen, returnFlags, baseWindow)
	GWorldPtr   *offScreen;   /* Ptr to GWorld to update; updated GWorld ptr */
	GWorldFlags *returnFlags; /* Returns flags that UpdateGWorld returns */
	WindowPtr   baseWindow;   /* Pointer to window to base GWorld on */
	{
	Rect     windRect;    /* portRect of baseWindow in global coordinates */
	CGrafPtr currPort;    /* Pointer to the current GrafPort */
	GDHandle currGDevice; /* Handle to the current GDevice */
	QDErr    error;       /* Result of QuickDraw operations */

	/* Convert baseWindow’s portRect to global coordinates */
	windRect = baseWindow->portRect;
	LocalToGlobal (/*◊*/&topLeft(windRect));
	LocalToGlobal (/*◊*/&botRight(windRect));

	/* Update GWorld so that it has the same characteristics as the screen */
	*returnFlags = UpdateGWorld (/*◊*/offScreen, 0, &windRect, (CTabHandle) nil,
			(GDHandle) nil, clipPix);

	/* If an error occurs, get the error code */
	if (gwFlagErr & *returnFlags)
		error = *returnFlags;
	else
		{
		error = noErr;

		/* Redraw our picture if updated GWorld has new depth or color table */
		if ((newDepth & *returnFlags) || (mapPix & *returnFlags))
			{
			GetGWorld (/*<*/&currPort, /*<*/&currGDevice);
			SetGWorld (*offScreen, nil);
			if (LockPixels (GetGWorldPixMap (*offScreen)))
				{
				PaintImage ();
				UnlockPixels (GetGWorldPixMap (*offScreen));
				}
			SetGWorld (currPort, currGDevice);
			}
		}

	return error;
	}
