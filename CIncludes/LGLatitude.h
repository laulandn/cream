/* $Id: LGLatitude.h,v 1.35 1997/04/08 17:24:43 hempling Exp $ */

/*
 * LGLatitude.h:
 * (c) Copyright 1992-1997 Metrowerks Corporation  All Rights Reserved.
 * 
 */

#ifndef __LGLATITUDE_H
#define __LGLATITUDE_H

/*
 * This file defines essential Latitude macros and
 * functions.  It is included by Latitude's version of Types.h
 * so that user programs do not have to include it explicitly
 * when compiling with Latitude.  LATITUDE specific material is
 * conditionalized on _LATITUDE_ so that developers who wish
 * to use the macros defined here may unconditionally include
 * this file in their code.
 */

#ifdef _LATITUDE_

/*
 * Delete the pascal keyword from all function declarations.
 * With Latitude, all toolbox entry points and user callback
 * procedures use C calling conventions.
 */
#define pascal

#ifndef __TYPES__
#include <Types.h>
#endif

/*
 * The following are private definitions not to be used by
 * Latitude clients.
 */
#define __LATITUDE_SLOW_PSTRINGS__
#define __LATITUDE_WRITABLE_PSTRINGS__
#ifdef __cplusplus
extern "C" {
#endif
extern StringPtr lg_latitude_passtr(const char *str);
#ifdef __cplusplus
}
#endif

#ifdef __LATITUDE_SLOW_PSTRINGS__
#define _LATITUDE_PASSTR(x)	lg_latitude_passtr(x)
#else	/* __LATITUDE_SLOW_PSTRINGS__ */
#define _LATITUDE_PASSTR(x)       ("\377" # x)
#endif	/* __LATITUDE_SLOW_PSTRINGS__ */

/*
 * PUBLIC DECLARATIONS SPECIFIC TO LATITUDE
 */

/*
 * This macro controls alignment of elements in structures.
 * 680x0 compilers align structure elements just like
 * Macintosh compilers, but all other compilers require the
 * insertion of filler bytes to make certain elements in
 * separate structures have the same offset.
*/
#ifndef __mc68000__
#define __LATITUDE_LONG_ALIGNED__
#define _LATITUDE_LONG_ALIGNED_
#endif

/*
 * The comp type is a signed 64-bit integer type supported by
 * MPW compilers.  GNUC supports this as "long long".  Other
 * compilers will see a struct.  This declaration will allow
 * comp types to be passed to Toolbox routines, but won't
 * allow computation on values of type comp.
 */
#ifdef _LATITUDE_
typedef struct wide comp;
#else
typedef struct { short man[4]; } comp;
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*
 * The following functions are local extensions to the Macintosh
 * interface, for use with Latitude.
 */

struct EventRecord;
typedef void (*LATITUDE_EVENT_HOOKPTR)(struct EventRecord *theEvent);
typedef void (*LATITUDE_FILL_DEFAULT_MENUBAR_PROCPTR)(void);

/* 
 * You may compile in data normally found in files during 
 * the start-up sequence performed by lg_latitude_init(). This data 
 * includes the System file, the application's resource fork, and 
 * any bitmap fonts normally installed in the System:Fonts folder.
 * Compiling in resource data allows the application 
 * to be completely self contained, i.e. no dependance upon the 
 * locations of these files.
 *
 * To enable this feature, allocate an array of LG_MEMFILE_RECORDS
 * and set each record's field like this:
 *
 *	name = pascal string containing the file name under which
 *	       this data is usually known.
 *
 *	type = constant describing the kind of data. See below.
 *
 *	buffer = pointer to the buffer containing the resource data.
 *
 *	length = length of the resource data.
 *
 * The array should be terminated with a record who's type is 0.
 */

/*
 * Set these values in the type field to designate the
 * record's data as containing this type of data.
 */

/* System resource file */
#define LG_MEMFILE_SYSTEM_TYPE	QUADCONST('S','Y','S','M')
/* Application resource file */
#define LG_MEMFILE_APPL_TYPE	QUADCONST('A','P','P','L')
/* Font resource file */
#define LG_MEMFILE_FONT_TYPE	QUADCONST('F','O','N','T')

typedef struct {
    Str63 name;
    OSType type;
    const unsigned char *buffer;
    unsigned long length;
} LG_MEMFILE_RECORD;

enum {
    LG_APP_INFO_SIGNATURE		= 0x1,
    LG_APP_INFO_CLASSNAME		= 0x2,
    LG_APP_INFO_FALLBACKS 		= 0x4,
    LG_APP_INFO_WINDOW_GUI_COLOR	= 0x8,
    LG_APP_INFO_MODAL_EVENT_HOOK	= 0x10,
    LG_APP_INFO_MEMFILE_LIST		= 0x20,
    LG_APP_INFO_NO_FLOATING_DIALOGS	= 0x40,
    LG_APP_INFO_ASSUME_DIALOGS_MODAL	= 0x80,
    LG_APP_INFO_FILL_DEFAULT_MENUBAR_PROC	= 0x100,
    LG_APP_INFO_WINDOWS_USE_APP_ICON	= 0x200,
    LG_APP_INFO_COPYBITS_USE_GUI_COLOR	= 0x400
};

typedef struct {
    long flags;
    OSType signature;
    char *classname;
    char **fallbacks;
    char window_gui_color;
    LATITUDE_EVENT_HOOKPTR modal_event_hook;
    LG_MEMFILE_RECORD *memfile_list;
    char no_floating_dialogs;
    char assume_dialogs_modal;
    LATITUDE_FILL_DEFAULT_MENUBAR_PROCPTR fill_default_menubar_proc;
    Boolean windows_use_app_icon;
    Boolean copybits_use_gui_color;
} LG_APP_INFO_BLOCK;

/*
 * signature:   the four character constant that is your app's finder
 *	        signature.
 * classname:   The name of your application. Used to define an X class
 *	        for your app.
 * fallbacks:   GUI resource fallbacks.
 * window_gui_color: When non-zero, automatically set window background
 *		color to the gui background color.
 * modal_event_hook: Used by Latitude modal dialogs in which the app
 *		doesn't have the chance to supply a modal filter,
 *		such as the Copy Files dialog. This hook is currently
 *		called when update events for windows other than
 *		the dialog window are encountered.
 * memfile_list: See the comment describing LG_MEMFILE_RECORD above.
 * no_floating_dialogs: By default, windows created by the dialog manager
 *		float.  If an application uses dialog windows as regular
 *		windows, it may not want dialogs to float.  For these
 *		cases, set this field to true.
 * assume_dialogs_modal: By default, windows created by the dialog manager
 *		are not assumed to be modal.  If an application can safely
 *              ascertain that all its dialogs are modal, then it should
 *              set this value to true.  This will yield best GUI window
 *              manager behavior.  Otherwise, an application can set
 *              individual instances of dialogs to be modal via the
 *              lg_latitude_next_window() call.
 * fill_default_menubar_proc: When using the lg_latitude_affect_menubar_in_window
 *		function to have different menubars in different windows,
 *		set this field to a function which will set the menus
 *		you wish to be seen when all windows are closed and
 *		the floating menubar is displayed. This function should
 *		call DrawMenuBar() before returning.
 * windows_use_app_icon: By default, GUI window icons are explicitly set to
 *		the application icon.  However, certain GUIs allow icons to
 *		be picked up from the environment as long as the icons are not
 *		explicitly set.  If you would like to get the icons from the
 *		environment, set this field to false.  The default is true.
 * copybits_use_gui_color: By default, CopyBits() will use the background
 *              color of the port to colorize a source image during a copy.
 *		However when the destination is a window, applications
 *		may want CopyBits() to use the window's GUI background color
 *		instead of the actual background set in the port.  Set the
 *		parameter copybits_use_gui_color to tell copybits whether to
 *		use the GUI color of a window to colorize a source image
 *		during a copy.
 */


/*
 * lg_latitude_init:
 * Initialize the Latitude.  This corresponds
 * to booting a Macintosh and starting your application.
 */
extern OSErr lg_latitude_init(long argc, char **argv, LG_APP_INFO_BLOCK *info);
/*
 * Initialize without launching.  This corresponds to turning on
 * the mac and starting a finder.
 */
extern OSErr lg_latitude_start(long argc, char **argv, LG_APP_INFO_BLOCK *info);

/*
 * lg_latitude_pre_init()
 * Initialize the trap table. Allows apps to call managers that are
 * assumed to be initialized before the app starts.
 */
extern void lg_latitude_pre_init(void);

/*
 * lg_latitude_code_resource:
 * This creates a handle to the given ProcPtr, and inserts it into
 * the ROM resource map.  Every code resource should be so registered,
 * since the application resource fork contains only 68000 code.
 */
extern Handle lg_latitude_code_resource(ResType theType, short theID,
                               ConstStr255Param name, ProcPtr proc);

#ifdef __cplusplus
}
#endif

#endif  /* _LATITUDE_ */

/*
 * PUBLICALLY AVAILABLE MACROS AND FUNCTIONS
 */

/*
 * An ANSI C replacement for four-byte character constants.
 * For example, one can replace instances of 'CODE' with
 * QUADCONST('C','O','D','E').
 */
#define QUADCONST(a, b, c, d)                   \
            (((long) ((a) & 0xff) << 24)        \
             | ((long) ((b) & 0xff) << 16)      \
             | ((long) ((c) & 0xff) << 8)       \
             | ((long) ((d) & 0xff)))

/*
 * This is an ANSI C replacement for MPW and THINK C's Pascal string
 * syntax.  One can replace most instances of "\pstring" with
 * PSTRINGCONST("string").  Note that PasStr("hello") may have
 * type "char *" when compiled with the THINK compiler, but
 * always has type "unsigned char *" when compiled by Unix
 * compilers, so a little bit of care is necessary.
 */
#ifdef _LATITUDE_
#ifdef __LATITUDE_WRITABLE_PSTRINGS__
#define PSTRINGCONST(x)         ((StringPtr) _LATITUDE_PASSTR(x))
#else
#define PSTRINGCONST(x)         ((ConstStr255Param) _LATITUDE_PASSTR(x))
#endif /* __LATITUDE_WRITABLE_PSTRINGS__ */
#else
#define PSTRINGCONST(x)         "\p" ## x        /* no parens here! */
#endif /* _LATITUDE_ */

/*
 * gestaltMachineType returns this value under Latitude.
 */
enum { gestaltLatitude = 45 };

#endif  /* __LGLATITUDE_H */

