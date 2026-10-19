/*
 * $Id: LGExtras.h,v 1.80 1997/08/14 00:53:53 hempling Exp $
 * (c) Copyright 1992-1997 Metrowerks Corporation  All Rights Reserved.
 * 
 * 
 */

#ifndef __LGEXTRAS_H__
#define __LGEXTRAS_H__

/*
 * Extensions provided by Latitude to the Macintosh Toolbox API.
 */

#ifndef __TYPES__
#include <Types.h>
#endif
#ifndef __FILES__
#include <Files.h>
#endif
#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif
#ifndef __WINDOWS__
#include <Windows.h>
#endif
#ifndef __MENUS__
#include <Menus.h>
#endif
#ifndef __PRINTING__
#include <Printing.h>
#endif
#ifndef __EVENTS__
#include <Events.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef USE_MIT_SHM
#define USE_MIT_SHM
#endif
#if defined(USE_MIT_SHM)

#define LG_LATITUDE_ANY_EVENT	-1

	/* For shared memory, we introduce some more calls.  One is a general
        mechanism to register an extension event filter.  Second, of course,
        is the access to the shared memory allocations routines itself.	   */

	/* Takes an event, and returns false if the event was successfully
        processed and the event should not be turned into a Mac event.	   */
typedef	Boolean	(*LG_EVENT_FILTER_PROC)(int *event);

  /* Register an event filter.  Every filter in the list is called for every
     event whose type matches the type registered, or if the type registered
     is LG_LATITUDE_ANY_EVENT, then the filter is called for every event.  If
     _any_ of the filters called for that type of event returns false, then
     the event is not converted into a Mac event, but is instead immediately
     given to XtDispatchEvent.											   */
extern Boolean lg_latitude_register_event_filter (int type,
												LG_EVENT_FILTER_PROC callBack);

  /* Remove an event filter from the filter list.						   */
extern Boolean lg_latitude_remove_event_filter (int type,
											  LG_EVENT_FILTER_PROC callBack);


  /* Shared memory interface.  What we're going to do is allocate a hunk of
     shared memory at startup time.  We will try and be nice about how much
     shared memory we allocate - we'll try and allocated half of the maximum
     allowed by the system.  For the amount of memory that we _can_
     allocated, we will treat it as a memory pool, allocating chunks out of
     it, so that work can be done directly into the shared memory.  If the
     application then does a blit with that memory, then that memory is used
     directly, and no copying need be done.  Note that blitting with a
     previously allocated hunk of the shared memory pool does an implicit
     free back into the pool of that memory.  Also note that this means that
     if the byte ordering of the blit needs to be fixed, that it is done
     _in_place_ for this case.											   */

  /* What was the size of the shared memory hunk that we allocated?		   */
extern unsigned long lg_latitude_shm_size (void);

  /* Allocate a hunk out of the shared memory pool.  If size is greater than
      lg_latitude_shm_size(), then this call will return NULL.  If enough of
      the pool is occupied such that there is no large enough free segment,
      but there _could_ be if all the in-flight requests complete, then this
      routine may synchronize with the X server in order to clear out those
      in_flight sections of the pool.  If there is no way that the requested
      size can currently be squeezed out of the pool, this routine will
      return NULL.														   */
extern void	*lg_latitude_shm_alloc (unsigned long size);

  /* This should normally not have to be called.						   */
extern void	lg_latitude_shm_free (void *addr);

  /* Is the address from the shared memory pool?						   */
extern Boolean lg_latitude_shm_valid(void *addr);

#else

#define	lg_latitude_shm_size()		0
#define lg_latitude_shm_alloc(size)	NULL
#define lg_latitude_shm_free(addr)
#define lg_latitude_shm_valid(addr)	false

#endif

/*
 * Direct pixel byte ordering on the mac is fixed. X has other ideas
 * here we list the different byte orders that we support.
 */

#define LG_QD_PIXEL_INDEXED 		0
#define LG_QD_PIXEL_URGB		1
#define LG_QD_PIXEL_UBGR		2
#define LG_QD_PIXEL_RGBU		3
#define LG_QD_PIXEL_BGRU		4

/* Find out what the pixel byte ordering scheme is */
extern short lg_latitude_pixel_byte_order(void);
/* 
 * Set the default byte order for GWorld image data.
 * returns the last default. 
 */
extern short lg_latitude_gworld_byte_order(short new_byte_order);
extern OSErr lg_latitude_change_byte_order(PixMapPtr pm, short desired_byte_order);

/*
 * Use lg_latitude_code_resource() to register your own function pointer
 * to be called by the toolbox as if the given resource type and id
 * contained a compiled code routine.
 * 
 * Latitude creates a Handle to the given function pointer and installs
 * it into the ROMMapHndl resource map. 
 *
 * To later retreive your code handle directly from the ROMMapHndl,
 * you can do something like this:
 *
 * #define ROMMAPHNDL_REFNUM	1
 *
 *	short cur_refnum;
 *	Handle defproc_handle;
 *	
 *	cur_refnum = CurResFile();
 *	UseResFile(ROMMAPHNDL_REFNUM);		
 *	defproc_handle = Get1Resource(resType, resID);
 *	UseResFile(cur_refnum);
 *
 * See Inside Macintosh More Macintosh Toolbox, page 1-135, for
 * details.
 */
extern Handle lg_latitude_code_resource(ResType theType, short theID,
                               ConstStr255Param name, ProcPtr proc);

/*
 * Given a handle created from lg_latitude_code_resource(), 
 * lg_latitude_code_address() returns the function pointer.
 * Dereferencing a lg_latitude_code_resource() handle won't get you
 * your function pointer. Latitude inserts some special instructions
 * before your function pointer. Using lg_latitude_code_address() is
 * the best way to get back your function pointer.
 */
extern ProcPtr lg_latitude_code_address(Handle h);


/* Scale windows on different platforms.
 * Pass the native display's dpi for a window
 * size identical to the window rendered on a 72 dpi mac.
 * Joel Kehle Sept 95
 * Note: Current implementation only handle ONSCREEN Windows!
 */
extern Boolean lg_latitude_scale_for_dpi(GrafPtr port, short dpi);

/* Put native info in the message field of key events */
extern void lg_latitude_native_key_messages(Boolean native);
extern void lg_latitude_pixmap_changed(const void *changed_pixmap);
extern void lg_latitude_bitmap_to_memory(const void *the_bitmap, Rect *rect, Handle memory);
/*
 * The following call is used to change the default value of
 * copybits_use_gui_color set in the LG_APP_INFO_BLOCK structure passed to
 * lg_latitude_init().  This override is sometimes necessary when the
 * default value is not generating the desired result from CopyBits()
 * operations. Set the parameter use_gui_color to tell CopyBits() whether to
 * use the GUI color of a window to colorize a src image during a copy.
 * To set a global default value at startup, see the copybits_use_gui_color
 * field in the LG_APP_INFO_BLOCK structure.
 */
extern void lg_latitude_copybits_use_gui_color(Boolean use_gui_color);

extern OSErr lg_native_to_apple_region(void *native, RgnHandle rgn);
extern void *lg_apple_to_native_region(RgnHandle rgn);
extern OSErr lg_flush_region(RgnHandle rgn);

/*
 * Returns the name of the current printer.  Pass the address of a 
 * character buffer big enough to hold the printer name. If no printer 
 * is available, NULL is returned.
 */
extern void lg_latitude_current_printer(char **name);
/*
 * Prompts user to choose a printer.  Return True if one was selected, 
 * False otherwise.
 */
extern Boolean lg_latitude_printer_select(void);

/*
 * Support for alternate Printer Manger High Level routines.
 *
 * If you wish to supply your own Print Dialogs or send custom
 * information directly to the Latitude Unix Printer driver, you
 * can use lg_latitude_tprint_set_characteristics() and
 * lg_latitude_tprint_get_characterstics() to set and retrieve
 * instructions.
 *
 * The data associated with a TPrint handle that you can get/send is:
 *
 *    destinationFlags - Specify the type and destination
 *	of the generated postscript.
 *    modificationFlags - Specify transformations to be performed
 *	on the generated postscript.
 *    outputFile - Where to put the postscript if the destination
 * 	is a file.
 *    printerName - A null terminated string containing the name
 * 	of the printer to send the postscript.
 *    auxPtr - a four byte value that can be used as private data.
 *
 * Null values for any of the pointers are acceptable.
 */

/*
 * Bit flag values for desintationFlags.
 */
#define LG_LW_COLOR                0x0008
#define LG_LW_USEFILE              0x0004

/* 
 * Bit flag values for modificationFlags.
 */
#define LG_LW_INVERT               0x0001
#define LG_LW_FLIPH                0x0002
#define LG_LW_FLIPV                0x0004
#define LG_LW_PRECISIONBITS        0x0010
#define LG_LW_LARGEPRINTAREA       0x0020
#define LG_LW_FASTBITMAP           0x0040
#define LG_LW_UNLIMITEDFONTS       0x0080

typedef enum {LGPrintLevel1 = 1, LGPrintLevel2 = 2,
	      LGPrintLevel1Color} LGPrintLevel;
typedef enum {LGPrintASCII, LGPrintBinary} LGPrintFormat;
typedef enum {LGPrintNoFonts, LGPrintAllFonts, LGPrintNonPrinterFonts,
      LGPrintNon13Fonts, LGPrintNon35Fonts} LGPrintDownloadFonts;

typedef struct _LGPrintExtras {
    LGPrintLevel language_level;
    LGPrintFormat format;
    LGPrintDownloadFonts download_fonts;
    char color_device;
    int num_doc_setup_strings;
    char **doc_setup_strings;
    int num_page_setup_strings;
    char **page_setup_strings;
    int num_fonts;
    char *resident_fonts;
    char *dispose_command;
    char *fax_string;
    void (*free_proc)(struct _LGPrintExtras *);
} LGPrintExtras;

extern void lg_latitude_tprint_set_characteristics(THPrint hPrint,
    unsigned short destinationFlags, unsigned short modificationFlags,
	FSSpec *outputFile, char *printer_name, LGPrintExtras *auxPtr);

extern void lg_latitude_tprint_get_characteristics(THPrint hPrint,
    unsigned short *destinationFlags, unsigned short *modificationFlags,
	FSSpec *outputFile, char *printer_name, LGPrintExtras **auxPtr);

/*
 * Use lg_latitude_set_page_sizes() to supply a set of page sizes for
 * the Page Setup dialog. If page sizes are supplied in this way,
 * the radio buttons and popup menu normally displayed in the
 * "Paper" area will be replaced with a popup menu containing
 * the page sizes supplied.
 * 
 * The arguments are:
 * hPrint - a THPrint handle. Currently used to keep the
 *    currently selected page size in sync with changes made to
 *    the new set of page sizes.
 * n - the number of page sizes being described
 * name - a list of n standard page size names
 *    These are the names that appear in the popup menu that replaces
 *    the radio buttons and popup menu normally displayed in the 
 *    "Paper" area.
 * name_tran - a list of n "translated" page size names.
 *    If not null, name_tran[x] will be displayed instead of name[x].
 * imageable_area - a list of n float arrays, each 4 elements each:
 *	llx, lly, urx, ury. These elements give the bounding area
 * 	to be imaged. The elements are in units of 1/72 inch.
 * dimension - a list of n float arrays, each 2 elements each:
 *	width, height. These elements are in units of 1/72 inch.
 * page_size_ps_string - a list of n PostScript invocation strings 
 *	to invoke this page size.
 * page_region_ps_string - a list of n PostScript invocation strings
 *	to invoke this page size for imaging but not to affect
 *	paper tray selection.
 *
 * page_size_ps_string is used with manual feed is NOT selected.
 * page_region_ps_string is used when manual feed is selected.
 *
 * It is best to call lg_latitude_set_page_sizes before PrStlInit and
 * any time that the destination printer may have changed, i.e. before
 * starting a print job.
 *
 * The first page size entry will be used as the default if the
 * user does not choose a page size from the Page Setup dialog.
 */

extern void lg_latitude_set_page_sizes(THPrint hPrint, short n, 
    char **name, char **name_tran, 
    float **imageable_area, float **dimension,
    char **page_size_ps_string, char **page_region_ps_string);

/*
 * By default, the printer driver filters each character
 * written to a postscript file to ensure that only
 * 7 bit ascii characters are written. It also escapes ('\\')
 * special characters automatically.
 *
 * Use lg_latitude_print_pass_through(true) to
 * force the printer driver to skip the filtering.
 * lg_latitude_print_pass_through(false)
 * restores the default printer driver filter.
 */

extern void lg_latitude_print_pass_through(Boolean enable);

/* Set the resource ID to use for the print dialog */
extern void lg_latitude_set_print_job_dialog_resource(int resId);

/* translation to/from native platform file names */
extern OSErr lg_latitude_native_to_fsspec(const void *native, FSSpec *fsspec);
extern OSErr lg_latitude_native_to_root_fsspec(const void *native, FSSpec *fsspec);
extern OSErr lg_latitude_fsspec_to_native(const FSSpec *fsspec, void *native);

/*
 * If a file in a Mac volume is manipulated directly with Unix
 * system calls, there is a chance that the Mac catalog information
 * will not correctly reflect the modified file. To ensure that
 * the Mac catalog information is updated, use 
 * lg_latitude_force_fstat() to force a catalog update of the
 * file described by the given FSSpec.
 */
extern OSErr lg_latitude_force_fstat(const FSSpec *fsspec);


/* What about the window has the window manager changed? */
#define LG_WMGR_POSITION	0x01
#define LG_WMGR_SIZE		0x02
#define LG_WMGR_HILITE		0x04
#define LG_WMGR_BEHIND		0x08
#define LG_WMGR_GOAWAY		0x10
/*
 * An application can be informed about changes to windows caused
 * by the desktop window manager: position, size,
 * hilite (i.e. keyboard focus) and stacking order.
 *
 * Setting the proc to NULL causes the default action:
 *	position (post mouseDown inDrag, if window has one).
 *	size (post mouseDown inGrow, if window has one).
 *	focus (post mouseDown inDrag, if window has one).
 *	behind (post mouseDown inDrag of front window if front
 *	    window changes).
 *	goaway (post mouseDown inGoAway)
 *
 * Calling lg_latitude_wmgr_proc returns the old proc.  Clients will
 * generally save this and call it within their new proc.
 */
typedef void (*LG_WMGR_PROC)(unsigned long changed, WindowPtr theWindow);

extern LG_WMGR_PROC lg_latitude_wmgr_proc(LG_WMGR_PROC proc);

/*
 * Use the following routine to override or give hints about attributes for
 * a specific window instance.  The GUI needs to know information when its
 * window is created, so the call needs to be made by the application before
 * the Mac window is created.  Once the window is created using the values
 * passed in by the application, the values are discarded and reset so that
 * they will not affect any other windows.  To manually force a reset, simply
 * call this routine with a NULL parameter.
 *
 * Here is a description for each field in the structure:
 *
 * flags - a combination of bits which tells Latitude which fields in the
 *         structure are being used.  Use the values in the enum below.
 * parts - a combination of bits which tells Latitude which window parts are
 *         desired for the window.  The value of parts is the same as the
 *         one used in lg_latitude_register_window() above.
 * modal - A boolean value which tells Latitude that the window
 *         will be modal.  This gives the Latitude GUI a chance to use the
 *         right kind of windows to obtain the best interaction with the
 *         GUI's window manager.  If unspecified, the default is false.
 * use_gui_color - 
 *         In general, Latitude WCTabHandles for Mac windows contain three
 *         extra entries for the GUI's foreground, edged-in, and edged-out
 *         colors.  The latter two are used to give a 3D look.  However,
 *         the window background may be set to either the standard Mac white,
 *         or the GUI's window background color.  This depends on the values
 *         of the window_gui_color field in the LG_APP_INFO_BLOCK structure
 *         (See lg_latitude_init()), or on the value of use_gui_color to the
 *         call below.
 *
 *         The following call is used to override the value of window_gui_color
 *         in LG_APP_INFO_BLOCK for a specific window instance.  If
 *         window_gui_color is false and use_gui_color is true, then this will
 *         call SetWinColor on the next window to set a WCTabHandle whose
 *         contentColor is the GUI's window background color.  If
 *         window_gui_color is set to true, then Latitude assumes all windows
 *         want to use the GUI's window background color.  In this case, the
 *         following call can be used with use_gui_color set to false to
 *         inform Latitude that an individual window instance does not want
 *         to use the GUI's window background color.
 *
 * floats - 
 *	   Signal the GUI as to the floating state of a window. One of the
 *         following values is expected:
 *
 *         - LG_WINDOW_HAS_FLOATERS: This is the default for non-dialog
 *           windows, usually referred to as document windows. It means that
 *           the window is not a floater, and that floaters will be maintained
 *           in front of this window in the window stack.
 *
 *         - LG_WINDOW_IS_FLOATER: This is the default for dialog windows,
 *           unless the no_floating_dialogs field in the LG_APP_INFO_BLOCK has
 *           been set to true.  This flag guarantees that the window will
 *           be maintained in front of windows that can have floaters in the
 *           window stack.  Note that window stacking among floaters is not
 *           guaranteed, and that this window will not be stacked in front of
 *           windows that can have no floaters. See below.
 *
 *         - LG_WINDOW_HAS_NO_FLOATERS: This flag means that the window can be
 *           anywhere on the windows stack: in front of floaters, or behind
 *           windows that have floaters.
 *
 * gui_name -
 *	   A string specifying the name of the GUI widget.  If unspecified,
 *         the GUI will use either the window title or the default string
 *         "windowShell".  This string must be static, or must not be freed
 *	   until after a window creation call is made.
 *
 * use_app_icon -
 *         A boolean value which tells Latitude to set a window's icon to
 *         be the same as the application icon.  The default is true.  If
 *         false, the window's icon will be unspecified and picked up from
 *         the environment when available.
 *
 * gui_state_iconic -
 *	   A boolean value which tells Latitude to set a window's GUI state
 *	   to be initially iconic.  If unspecified, the default is to leave
 *	   this GUI resource unset.
 */

#ifndef BIT
#define BIT(x) (1L << (x))
#endif

enum {
    LG_WINDOW_HAS_FLOATERS,
    LG_WINDOW_IS_FLOATER,
    LG_WINDOW_HAS_NO_FLOATERS
};

enum {
    LG_WMGR_WINDOW_PARTS		= 0x1,
    LG_WMGR_WINDOW_MODAL		= 0x2,
    LG_WMGR_WINDOW_USE_GUI_COLOR       	= 0x4,
    LG_WMGR_WINDOW_FLOAT       		= 0x8,
    LG_WMGR_WINDOW_GUI_NAME		= 0x10,
    LG_WMGR_WINDOW_USE_APP_ICON		= 0x20,
    LG_WMGR_WINDOW_GUI_STATE_ICONIC    	= 0x40
};

typedef struct {
    long flags;
    long parts;
    char modal;
    char use_gui_color;
    char floats;
    char *gui_name;
    char use_app_icon;
    char gui_state_iconic;
} LG_WMGR_WINDOW_BLOCK;

extern void lg_latitude_next_window(LG_WMGR_WINDOW_BLOCK *info);

/*
 * To draw in the native GUI foreground color using Quickdraw, access
 * the rgb value in the window color table using the index below, and
 * set it in the port.  Then, use standard Quickdraw calls to perform
 * the drawing.
 *
 * To draw in native looking colors to offscreen ports, determine the window
 * that the offscreen port will eventually be drawn to, then access the
 * foreground, edged-in, or edged-out rgb values for the window out of
 * its ctTable using the indexes below.  Use these values as the offscreen
 * port's foreground and background values as desired, then use standard
 * Quickdraw calls to do the drawing.
 */
enum {
    LG_WMGR_GUI_FOREGROUND	= 5,
    LG_WMGR_GUI_EDGED_IN	= 6,
    LG_WMGR_GUI_EDGED_OUT	= 7
};

/*
 * Native looking frame and line edges can be drawn directly to windows by
 * calling lg_latitude_frame_gui_color() and lg_latitude_line_gui_color().
 * These are convenience routines which access LG_WMGR_GUI_EDGED_IN and
 * LG_WMGR_GUI_EDGED_OUT from the window's WCTabHandle to create the correct
 * GUI native effect (3D).
 */
extern void lg_latitude_frame_gui_color(const Rect *rect, Boolean edged_in);
extern void lg_latitude_line_gui_color(const Point *from, const Point *to,
				     Boolean edged_in);

/*
 * The following allow dynamic modification of a window's floating state,
 * access to the first non-floating window, and also enables/disables
 * native windows to be treated as floaters.
 */
extern void lg_latitude_floating_window(const WindowPtr theWindow, short flag);
extern WindowPtr lg_latitude_front_non_floating_window(void);
extern Boolean lg_latitude_native_floater(const void *floater);
extern Boolean lg_latitude_withdraw_native_floater(const void *floater);

/*
 * This allows applications to inform the toolbox of special cases
 * of window proc ids for decorations and behavior. The default for proc
 * ids is to use the proc id passed in at NewWindow() time. An application
 * may want to override this and use the internal WDEF 0 or 1. To do
 * this, baseproc should be set to the value the toolbox must subtract
 * from the real proc id.
 * Since we have added 3 non-mac decorations - Windows Maximize,
 * Menus in Docs and the Iconify decoration - and 2 non-mac window
 * behaviors - Window Floats and Window is Modal -, our rules for adding
 * these items may not be sufficient to do what applications want.
 * By default, we add all of these decorations if
 * 1] The GUI resource file says it's ok, and
 * 2] The appropriate values in parts were passed in -
 *    BIT(LG_WINDOW_PARTS_MENUBAR), BIT(LG_WINDOW_PARTS_MAXIMIZE),
 *    and/or BIT(LG_WINDOW_PARTS_ICONIFY).
 *
 * The non-mac window floating behavior turned on by LG_WINDOW_PARTS_FLOAT
 * guarantees that the window "float" with respect to other windows.  See
 * lg_latitude_floating_window() for details.  Note that by default dialog
 * windows are floaters.  This default may be turned off via the
 * LG_APP_INFO_BLOCK field no_floating_dialogs, leaving the
 * application in total control of which windows should float through proc ids
 * (for a group of windows), and/or through the use of the
 * lg_latitude_floating_window() call (for single window instances).

 * The non-mac window floating behavior turned on by LG_WINDOW_PARTS_MODAL
 * guarantees that the GUI will use the best GUI window type that works best
 * for modal windows and GUI window managers.  We recommend setting this bit
 * whenever applicable.
 */

#define LG_WINDOW_PARTS_MODAL    11
#define LG_WINDOW_PARTS_FLOAT    12
#define LG_WINDOW_PARTS_MAXIMIZE 13
#define LG_WINDOW_PARTS_MENUBAR  14
#define LG_WINDOW_PARTS_ICONIFY  15

extern void lg_latitude_register_window(short procid, short use_lg_wdef0, long parts);
extern void *lg_latitude_window_widget(WindowPtr window);

/*
 * Retrieve the shell widget of the topmost document window if
 * menubarPlacement is "document", or the floating menubar otherwise.
 * This should be used as the parent for dialog shells.
 */
extern void *lg_latitude_get_menubar_widget(void);

/*
 * Retrieve the top most shell widget.
 */
extern void *lg_latitude_get_main_widget(void);

/*
 * Dispatch an event.  This should be called instead of XtDispatchEvent()
 * since some internal processing of the event may be necessary.
 */
extern Boolean lg_latitude_dispatch_event(void *event);

/*
 * These are the entry points into our tablet support.
 * lg_latitude_get_pressure_info() returns whether the tablet is available or
 * not.  If it is, it returns the minimum and maximum pressure values
 * supported by the tablet device.
 * lg_latitude_get_pressure() returns the most recent pressure.
 */

extern Boolean lg_latitude_get_pressure_info(int *min_pressure, int *max_pressure);
extern long lg_latitude_get_pressure(void);

/*
 * A rect for the parameter window is calculated depending on the values
 * of full_size, hide_parts and show_menu_bar.
 *
 * If full_size is true, a rect for the window is returned such that if
 * the window would be set to it, it would occupy the whole device display.
 * If full_size is false, the window's current rect is returned.
 * 
 * If menubarPlacement is set to "document", show_menu_bar allows the menu
 * bar to be shown (true) or hidden (false).
 * 
 * The hide_parts parameter permits the application to request the minimal
 * amount of decorations supported by the GUI, giving maximum display area
 * to the window.  Some GUIs do not support this parameter.
 *
 * For either value of full_size, the rect returned accounts for space
 * requirements which depend on the value of show_menu_bar and hide_parts.
 */
extern void
lg_latitude_full_screen_size_window(WindowPtr theWindow, Boolean full_size,
				  Boolean hide_parts, Boolean show_menu_bar,
				  Rect *new_win_rect);

/*
 * The standard Mac MDEF does not allow for 
 * command key sequences that involve modifiers other than the
 * command key, such as control, alt, and shift. To expand this
 * functionality, and communicate the key sequences to the user,
 * some applications use their own custom MDEF to draw 
 * standard looking menus with items that have these expanded key
 * sequences. 
 *
 * Latitude supplies a custom MDEF which is capable of displaying
 * these multi-key command sequences. The application calls
 * lg_latitude_custom_mdef() from within its own custom MDEF,
 * defering the mDrawMsg, mChooseMsg, mSizeMsg, and mPopUpMsg
 * tasks when appropriate. 
 *
 * The arguments are:
 *
 *    short message	   - the MDEF message.
 *    MenuHandle theMenu   - menu handle.
 *    Rect *menuRect	   - display rectangle for the menu.
 *    Point hitPt	   - mouse location.
 *    void *auxilliary	   - a.k.a "whichItem". item last chosen.
 *
 * These arguments are identical to those described in the Macintosh
 * programming literature and the application's MDEF should simply pass
 * through its arguments to lg_latitude_custom_mdef() for all messages.
 * 
 * For the mDrawMsg, the application's MDEF should skip all of
 * the drawing operations needed to actually draw the menu. Instead,
 * it should call lg_latitude_custom_mdef() once for each item
 * in the menu, sending a description of the menu item in the
 * form of a pointer to a LG_CUSTOM_ITEM record as the
 * whichItem argument. For example:
 *
 *	LG_CUSTOM_ITEM custom_item;
 *	...
 *	case mDrawMsg:
 *	    for (custom_item.itemNumber = 1; 
 *	    custom_item.itemNumber <= menu_item_count; 
 *	    custom_item.itemNumber++)
 *	    {
 *		(fill in fields of custom_item)
 *
 *		lg_latitude_custom_mdef(mDrawMsg, theMenu, menuRect,
 *		    hitPt, (void *) &custom_item);
 *	    }
 *	    break;
 *		    
 * Use the Mac defined cmdKey, shiftKey, optionKey, and controlKey
 * values to set the acceleratorModifier field for the custom_item.
 * Use the standard character code to set the acceleratorChar
 * field for the custom_item. A set of non-alpha keycodes is defined
 * below.
 *
 * For example, a command-control-shift-F1 sequence would be 
 * described as:
 *
 *  custom_item.acceleratorModifier = cmdKey | controlKey | shiftKey;
 *  custom_item.acceleratorChar = LG_CUSTOM_ITEM_F1;
 *
 * The rest of the fields of the LG_CUSTOM_ITEM record are identical
 * to the item attributes for the standard Mac MDEF.
 */

typedef struct {
    short itemNumber;
    Boolean enabled;
    ResType iconType;		/* 0, 'ICON', or 'SICN' */
    short iconID;		/* resource id for iconType icon */
    short markChar;
    StringPtr itemString;
    unsigned short acceleratorModifier;
    short acceleratorChar;
    Style itemStyle;
    short subMenu;		/* menu id for a submenu. */
    BitMap *itemImage;		/* currently not used. */
} LG_CUSTOM_ITEM;

/* 
 * Use these defines for the acceleratorChar to display 
 * key words/symbols in menu item command-key strings.
 */
#define LG_CUSTOM_ITEM_COMMANDMARK    (17)
#define LG_CUSTOM_ITEM_CHECKMARK      (18)
#define LG_CUSTOM_ITEM_DIAMONDMARK    (19)
#define LG_CUSTOM_ITEM_CONTROLMARK   (239)
#define LG_CUSTOM_ITEM_SPACEMARK     (240)
#define LG_CUSTOM_ITEM_ENTER1MARK    (242)
#define LG_CUSTOM_ITEM_KEYPADMARK    (245)
#define LG_CUSTOM_ITEM_DELETEMARK    (246)
#define LG_CUSTOM_ITEM_BACKSPACEMARK (247)
#define LG_CUSTOM_ITEM_TABMARK       (248)
#define LG_CUSTOM_ITEM_ENTER2MARK    (249)
#define LG_CUSTOM_ITEM_DOWNMARK      (250)
#define LG_CUSTOM_ITEM_LEFTMARK      (251)
#define LG_CUSTOM_ITEM_RIGHTMARK     (252)
#define LG_CUSTOM_ITEM_UPMARK        (253)
#define LG_CUSTOM_ITEM_OPTIONMARK    (254)
#define LG_CUSTOM_ITEM_SHIFTMARK     (255)

#define LG_CUSTOM_ITEM_F1	(0x107A)
#define LG_CUSTOM_ITEM_F2	(0x1078)
#define LG_CUSTOM_ITEM_F3	(0x1063)
#define LG_CUSTOM_ITEM_F4	(0x1076)
#define LG_CUSTOM_ITEM_F5	(0x1060)
#define LG_CUSTOM_ITEM_F6	(0x1061)
#define LG_CUSTOM_ITEM_F7	(0x1062)
#define LG_CUSTOM_ITEM_F8	(0x1064)
#define LG_CUSTOM_ITEM_F9	(0x1065)
#define LG_CUSTOM_ITEM_F10	(0x106D)
#define LG_CUSTOM_ITEM_F11	(0x1067)
#define LG_CUSTOM_ITEM_F12	(0x106F)
#define LG_CUSTOM_ITEM_F13	(0x1069)
#define LG_CUSTOM_ITEM_F14	(0x106B)
#define LG_CUSTOM_ITEM_F15	(0x1071)


extern void
lg_latitude_custom_mdef(short message, MenuHandle theMenu, Rect *menuRect,
    Point hitPt, void *auxilliary);

/*
 * Latitude maintains the MenuList handle in Macintosh alignment
 * space. These two functions are useful for getting info out of the
 * MenuList without having to traverse a Macintosh aligned structure.
 *
 * lg_latitude_menu_list_count() returns the number of menus in the
 * first have of the given menu list (menu bar menus) and the number of 
 * menus in the second half of the menu list (hierarchical menus).
 *
 * lg_latitude_get_ind_menu_from_menu_list() returns the nth
 * menu handle from the menu list. The index extends into
 * the second half of the menu list.
 */
extern void lg_latitude_menu_list_count(Handle menu_list_handle,
    short *first_half_count, short *second_half_count);
extern MenuHandle lg_latitude_get_ind_menu_from_menu_list(short index,
    Handle menu_list_handle);

/*
 * To facilitate the appearance of different menubars in
 * different windows when the application is run with
 * menubar placement set to "document", use
 * lg_latitude_affect_menubar_in_window() to direct all
 * subsequent DrawMenubar calls to only affect the menubar in
 * the given window. 
 *
 * Mac applications typically change the menus in the 
 * menubar upon receiving an activateEvt event. By 
 * inserting a call to this function before performing
 * the menubar modifying operations, the Latitude enabled
 * application will appear to have different menubars
 * in different windows. Internally, however, there is
 * still only one MenuList! This function merely permits
 * DrawMenuBar to only update the appearence of the
 * menubar in the window specified. 
 *
 * To cause menubar modifying operations to affect all
 * visible menubars, call this function with a windowPtr
 * equal to NULL.
 *
 * To cause menubar modifying operations to affect only
 * the floating menubar, call this function with a windowPtr
 * equal to LG_MENU_FLOATING_BAR.
 *
 * This function has no affect if only one menubar is visible
 * or the application is running with menubar placement set
 * to floating.
 */

#define LG_MENU_FLOATING_BAR -1

extern void lg_latitude_affect_menubar_in_window(const WindowPtr theWindow);

/*
 * When using lg_latitude_affect_menubar_in_window(), applications
 * may have a difficult time when it is desired that all 
 * menubars become temporarily disabled - when a modal window
 * is displayed, for example. To make this operation easier, 
 * use lg_latitude_set_menubar_state(). 
 *
 * lg_latitude_set_menubar_state(false) will disable all visible
 * menubars. The state of individual menus is preserved at
 * this time.
 * 
 * lg_latitude_set_menubar_state(true) will enable all visible
 * menubars. Individual menus that were disabled before
 * the menubar was disabled will be returned to their
 * former, disabled, state.
 *
 * Be sure to always pair up enable and disable menu bar
 * calls.
 */
extern void lg_latitude_set_menubar_state(Boolean enabled);

/*
 * lg_latitude_menu_suppress_item() provides a convenient way to suppress
 * items from being shown when a menu is displayed. Suppressing
 * menu items does not cause any other items in the menu to change their
 * item IDs. Only menus using the standard MDEF or Latitude supplied
 * custom MDEF are affected.
 *
 * lg_latitude_menu_suppress_item() takes three arguments:
 *
 *   MenuHandle theMenu - menu handle
 *   short item_number  - item number affected. zero for all items.
 *   Boolean suppress	- non-zero = suppress, zero = show item.
 *
 * If item_number is 0, then all items in the menu will be affected
 * by the "suppress" argument value. 
 *
 * If items are inserted or removed from menu, 
 * lg_latitude_menu_suppress_item(theMenu, 0, false) should first be
 * perfomed to unset all suppressed items.
 *
 * MenuKey() treats suppressed items as disabled items: it
 * returns 0 and sets the MenuDisable global appropriately when
 * given a key to a disabled or suppressed item.
 */
extern void lg_latitude_menu_suppress_item(MenuHandle theMenu,
    short item_number, Boolean suppress);
  

/*
 * Allow clients to enable/disable focus lock on the fly.  This is
 * necessary for Mac applications which want to run native
 * applications and would not like to suspend because of the child
 * application, e.g. Photoshop plug-ins.
 */
extern void lg_latitude_focus_lock(Boolean lock);

/*
 * Get the native key name for the given mac virtual key code.
 * For example, sending the mac key code for the command key (0x37) will
 * return the name of the key assigned for the command key.
 *
 * See Inside Mac: Macintosh Toolbox Essentials, 2-43 for key code maps.
 */
#define LG_KEY_CONTROL 0x3B
#define LG_KEY_COMMAND 0x37
#define LG_KEY_SHIFT   0x38
#define LG_KEY_LOCK    0x39
#define LG_KEY_OPTION  0x3A

extern void lg_latitude_native_key_name(short virt_code, 
    StringPtr key_name);

/*
 * Designate the given control to be drawn with or without the native 
 * default marking. Use this function rather than FrameRoundRect if 
 * you have a button in a window who's windowKind != dialogKind. 
 * For dialogs, use the SetDialogDefaultItem function instead.
 */
extern void lg_latitude_control_default_ring(ControlHandle theControl,
    Boolean set_it);

/*
 * The following calls signal the GUI to use the parameter parent as the
 * parent to the next GUI window created.  This allows Mac ports to be
 * embedded in native windows.
 */
extern void lg_latitude_set_next_window_parent(const void *parent);
extern void lg_latitude_set_next_popup_parent(const void *parent);
 
/*
 * Use the icon resource specified by theID and theType to set the
 * window's icon in the GUI.  If fsspec is non-NULL, the resources in
 * that file are used, otherwise it is assumed that the resource file is
 * already open and accessible by Get1Resource(theType, theID).  If the
 * resource is not found, lg_latitude_set_finder_icon() does nothing.
 */
extern void lg_latitude_set_finder_icon(const WindowPtr theWindow,
				      const FSSpec *fsspec,
				      ResType theType, short theID);

/*
 * Returns the position of the floating menu bar in global coordinates
 */
extern void lg_latitude_floating_bar_origin(short *hGlobal, short *vGlobal);

/*
 * Iconify or deiconify windows. A value of true causes the window to be
 * iconified, otherwise the window is deiconified.
 */
extern void lg_latitude_iconify_window(const WindowPtr theWindow,
				       Boolean iconify);
/*
 * Returns the iconification state of the GUI window.  Returns true if the
 * window is iconified, false otherwise.
 */
extern Boolean lg_latitude_window_iconified(const WindowPtr theWindow);

/*
 * Allow applications the option of showing native style alerts
 * when they call Alert, StopAlert, CautionAlert, or NoteAlert.
 * Latitude will place buttons and static text items 
 * in a native windowing system alert. Static text items are
 * concatinated in the order that they are found in the alert's
 * item list.  All other types of items are ignored.  While the 
 * native alert is presented, the application's filter function will 
 * be called if one is supplied. 
 *
 * If the alert does not contain a default button, the conversion 
 * is aborted and a "Mac" style alert is presented. 
 *
 * To present native alerts, call lg_latitude_use_native_alerts() with
 * a non-zero value. To present "Mac" style alerts, call this 
 * function with a zero value. Latitude defaults to "Mac" style alerts.
 *
 * To present the first character of each alert button hilited as a GUI
 * mnemonic, pass true as the value of the use_mnemonics parameter.
 *
 * lg_latitude_using_native_alerts() returns the current status of
 * these features.
 */
extern void lg_latitude_use_native_alerts(Boolean use_native,
					Boolean use_mnemonics);
extern Boolean lg_latitude_using_native_alerts(Boolean *using_mnemonics);

/*
 * Allow applications to check the status returned by QA partner.
 */
extern int lg_latitude_qap_running(void);

/* 
 * Use the MOUSE_BUTTON_NUMBER() macro to determin which mouse button 
 * is currently down. Given the modifiers field from the EventRecord, 
 * this macro returns the number of the mouse button currently down.
 * Latitude is using bits 5 and 6 (currently reserved by Apple)
 * to store the use of mouse buttons 2 and 3.
 */
#define MOD_MOUSE_BUTTON_3	0x40
#define MOD_MOUSE_BUTTON_2	0x20

#define MOUSE_BUTTON_NUMBER(mod) \
	(((mod) & btnState) ? 0  \
	: (((mod) & MOD_MOUSE_BUTTON_3) ? 3      \
	: (((mod) & MOD_MOUSE_BUTTON_2) ? 2      \
	: 1)))

/*
 * Allow applications to warp the mouse pointer
 */
extern void lg_latitude_warp_pointer(short hGlobal, short vGlobal);

/*
 * An app can provide a buffer which contains file contents and
 * ask the toolbox to use the buffer as an opened file.
 * This function returns a refnum value which can be used
 * with the File Manager calls to read data from the "file".
 * In memory buffer files are considered locked and read-only.
 *
 * filename - the name to assign to this in memory file buffer.
 * *buffer  - the address of the beginning of the file contents.
 * length   - the size of the file contents.
 * is_resource - opens this data as a resource or data fork.
 * *refnum  - the file's refnum is returned in this pointer.
 *
 * The FCB created (and associated with the returned *refnum)
 * will show that the file "exists" on the BootDrive,
 * in the root directory. 
 */
extern OSErr lg_latitude_open_in_memory_file(StringPtr filename,
    const unsigned char *buffer, unsigned long length,
    Boolean is_resource, short *refnum);

/*
 * Under the Mac, the Standard File Package automatically filters out
 * "invisible" files before the app's file filter proc has a chance to 
 * see them. Latitude marks UNIX files who's name begins with '.' 
 * as "invisible". By calling lg_latitude_sf_show_invisible_files(TRUE),
 * the Standard File Package will skip filtering invisible files and
 * pass them on to the app's file filter proc. Upon application startup,
 * the Mac filtering behavior is the default. Once the behavior is changed
 * with this call, it remains until changed again by this call.
 */
extern void lg_latitude_sf_show_invisible_files(Boolean show);

/*
 * Applications that create GUI widgets as children of Latitude GUI widgets
 * usually handle GUI events for these widgets themselves.  However, if the
 * application would like to see events for these widgets translated to Mac
 * events, each widget must be registered via
 * lg_latitude_add_mac_events_for_native_child().  When the Latitude GUI
 * widget that parents the application widgets is destroyed, the
 * application widgets are automatically removed.  However if the
 * application needs to prevent its widgets from getting Mac events before
 * the Latitude widget is destroyed, it can do so by calling
 * lg_latitude_remove_mac_events_for_native_child().
 *
 * Most GUI widgets automatically have GUI event masks that causes them to
 * get "interesting" GUI events. For example, in X-Windows most widgets
 * will have set button and key event masks so that they will receive these
 * events.  However, they usually do not have the event mask for mouse
 * motion.  Latitude uses X motion events in widgets to report the correct
 * mouse location.  It is for these motion events and any other events not
 * reported by the GUI the application is responsible for setting the
 * correct GUI event mask. In X-Windows, this needs to be done using
 * XSelectInput() once the application widget is realized:
 *
 * XSelectInput(XtDisplay(appWidget), XtWindow(appWidget),
 *              XtBuildEventMask(latitudeWidget) | PointerMotionMask);
 */
extern Boolean lg_latitude_add_mac_events_for_native_child(void *gui_widget);
extern Boolean lg_latitude_remove_mac_events_for_native_child(void *gui_widget);

/*
 * Retrieve the value mask of the GUI menubarPlacement resource.
 * Possible values are:
 *
 * - LG_MENUBAR_FLOATING: the resource is explicitly set to "floating" or
 *   "+floating", in possible combination with implied "-document".
 *
 * - LG_MENUBAR_DOCUMENT: the resource is explicitly set to "document"
 *   or "+document".
 *
 * - LG_MENUBAR_NOFLOATING: the resource is explicitly set to "-floating".
 *   This means that the floating menubar will never be mapped by Latitude.

 * Any combination of the above is valid.  For example, a resource value of
 * "+floating +document" makes the following call return bitmask
 * LG_MENUBAR_FLOATING|LG_MENUBAR_DOCUMENT.
 */

#define LG_MENUBAR_FLOATING    0x1
#define LG_MENUBAR_DOCUMENT    0x2
#define LG_MENUBAR_NOFLOATING  0x4

extern short lg_latitude_gui_menubar_type(void);

/* 
 * If your application erases the caret by some Quickdraw operation,
 * use this function to set the caret state so that a subsequent
 * TEDeactivate() call will not inadvertently draw the caret.
 */
struct TERec;
extern void lg_latitude_caret_erased(struct TERec **hTE);


/*
 * The following structures are  for overriding the 
 * Latitude Standard File Package. 
 */

typedef enum LG_APP_FILE_REQUEST {
    LG_APP_FILE_STANDARD_PUT = 1,
    LG_APP_FILE_CUSTOM_PUT,
    LG_APP_FILE_SF_PUT,
    LG_APP_FILE_SFP_PUT,
    LG_APP_FILE_STANDARD_GET,
    LG_APP_FILE_CUSTOM_GET,
    LG_APP_FILE_SF_GET,
    LG_APP_FILE_SFP_GET,
    LG_APP_FILE_STANDARD_PREVIEW,
    LG_APP_FILE_CUSTOM_PREVIEW,
    LG_APP_FILE_SF_PREVIEW,
    LG_APP_FILE_SFP_PREVIEW
} LG_APP_FILE_REQUEST;

typedef enum LG_APP_FILE_REQ_FIELD {
    LG_APP_FILE_PROMPT		= 0x0001,
    LG_APP_FILE_DEFAULT_NAME	= 0x0002,
    LG_APP_FILE_DIALOG_ID	= 0x0004,
    LG_APP_FILE_NUM_TYPES	= 0x0008,
    LG_APP_FILE_TYPE_LIST	= 0x0010,
    LG_APP_FILE_WHERE		= 0x0020,
    LG_APP_FILE_DIALOG_HOOK	= 0x0040,
    LG_APP_FILE_FILTER_PROC	= 0x0080,
    LG_APP_FILE_ACTIVE_LIST	= 0x0100,
    LG_APP_FILE_ACTIVATE_PROC	= 0x0200,
    LG_APP_FILE_YOUR_DATA	= 0x0400,
    LG_APP_FILE_TITLE		= 0x0800,
    LG_APP_FILE_APP_DATA	= 0x1000
} LG_APP_FILE_REQ_FIELD;

typedef struct LG_APP_FILE_REQUEST_INFO {
    LG_APP_FILE_REQUEST		type;
    unsigned long		fieldMask;
    ConstStr255Param		prompt;
    ConstStr255Param		defaultName;
    short			dialogID;
    short			numTypes;
    OSType			*typeList;
    Point			where;
    ProcPtr			dialogHook;
    ProcPtr			filterProc;
    short			*activeList;
    ProcPtr			activateProc;
    void			*yourData;
    ConstStr255Param		title;
    void			*appData;
} LG_APP_FILE_REQUEST_INFO;

typedef const char *(*LGFileRequestPtr)
    (LG_APP_FILE_REQUEST_INFO *request);

extern void lg_latitude_file_dialog_callback 
    (LGFileRequestPtr request);

extern void lg_latitude_next_file_request
    (LG_APP_FILE_REQUEST_INFO *request, Boolean override);

#ifdef __cplusplus
}
#endif

#endif /* __LGEXTRAS_H__ */

