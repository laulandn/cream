/* $Id: LGDebug.h,v 1.19 1997/04/08 17:24:41 hempling Exp $ */

/*
 * LGDebug.h:
 * (c) Copyright 1992-1997 Metrowerks Corporation  All Rights Reserved.
 * 
 * 
 */

/*
 * The following functions are available in the debuggable Latitude library.
 */

#ifndef __AP_DEBUG_H
#define __AP_DEBUG_H

/*
 * The following debug calls are all suitable for calling from the
 * the debugger.
 */

/*
 * The following enum matches the the CLASS_MAP listed in 
 * toolbox/base/debug.c
 */
/*
 * Here are the debug classes:
 */
typedef enum {
    LG_DEBUG_ALL,
    LG_DEBUG_PS,
    LG_DEBUG_GUI,
    LG_DEBUG_FOREIGN,
    LG_DEBUG_HFS,
    LG_DEBUG_BASE,
    LG_DEBUG_AEOBJECTS,
    LG_DEBUG_AEPACKOBJECT,
    LG_DEBUG_ASDEBUGGING,
    LG_DEBUG_ALIASES,
    LG_DEBUG_APPLEEVENTS,
    LG_DEBUG_APPLEGUIDE,
    LG_DEBUG_APPLESCRIPT,
    LG_DEBUG_APPLETALK,
    LG_DEBUG_BALLOONS,
    LG_DEBUG_CMAPPLICATION,
    LG_DEBUG_CTBUTILITIES,
    LG_DEBUG_CODEFRAGMENTS,
    LG_DEBUG_COLORPICKER,
    LG_DEBUG_COLORPICKERCOMPONENTS,
    LG_DEBUG_COMMRESOURCES,
    LG_DEBUG_COMPONENTS,
    LG_DEBUG_CONNECTIONS,
    LG_DEBUG_CONTROLSTRIP,
    LG_DEBUG_CONTROLS,
    LG_DEBUG_CURSORDEVICES,
    LG_DEBUG_DATABASEACCESS,
    LG_DEBUG_DESKBUS,
    LG_DEBUG_DEVICES,
    LG_DEBUG_DIALOGS,
    LG_DEBUG_DICTIONARY,
    LG_DEBUG_DISKINIT,
    LG_DEBUG_DISKS,
    LG_DEBUG_DISPLAYS,
    LG_DEBUG_DRAG,
    LG_DEBUG_ENET,
    LG_DEBUG_EPPC,
    LG_DEBUG_EDITIONS,
    LG_DEBUG_ERRORS,
    LG_DEBUG_EVENTS,
    LG_DEBUG_FCNTL,
    LG_DEBUG_FSM,
    LG_DEBUG_FILETRANSFERS,
    LG_DEBUG_FILES,
    LG_DEBUG_FIXMATH,
    LG_DEBUG_FOLDERS,
    LG_DEBUG_FONTS,
    LG_DEBUG_GESTALT,
    LG_DEBUG_ICONS,
    LG_DEBUG_IMAGECODEC,
    LG_DEBUG_IMAGECOMPRESSION,
    LG_DEBUG_LGSANE,
    LG_DEBUG_LISTS,
    LG_DEBUG_LOWMEM,
    LG_DEBUG_MIDI,
    LG_DEBUG_MACHINEEXCEPTIONS,
    LG_DEBUG_MEDIAHANDLERS,
    LG_DEBUG_MEMORY,
    LG_DEBUG_MENUS,
    LG_DEBUG_MOVIES,
    LG_DEBUG_NOTIFICATION,
    LG_DEBUG_OSA,
    LG_DEBUG_OSACOMP,
    LG_DEBUG_OSAGENERIC,
    LG_DEBUG_OSUTILS,
    LG_DEBUG_PCI,
    LG_DEBUG_PLSTRINGFUNCS,
    LG_DEBUG_PPCTOOLBOX,
    LG_DEBUG_PACKAGES,
    LG_DEBUG_PALETTES,
    LG_DEBUG_PICTUTILS,
    LG_DEBUG_POWER,
    LG_DEBUG_PRINTING,
    LG_DEBUG_PROCESSES,
    LG_DEBUG_QDOFFSCREEN,
    LG_DEBUG_QUICKDRAW,
    LG_DEBUG_QUICKDRAWTEXT,
    LG_DEBUG_RESOURCES,
    LG_DEBUG_RETRACE,
    LG_DEBUG_SCSI,
    LG_DEBUG_SCRAP,
    LG_DEBUG_SCRIPT,
    LG_DEBUG_SEGLOAD,
    LG_DEBUG_SERIAL,
    LG_DEBUG_SHAPE,
    LG_DEBUG_SHUTDOWN,
    LG_DEBUG_SLOTS,
    LG_DEBUG_SOUND,
    LG_DEBUG_SOUNDCOMPONENTS,
    LG_DEBUG_SOUNDINPUT,
    LG_DEBUG_SPEECH,
    LG_DEBUG_STANDARDFILE,
    LG_DEBUG_STANDARDSOUND,
    LG_DEBUG_START,
    LG_DEBUG_TERMINALS,
    LG_DEBUG_TEXTEDIT,
    LG_DEBUG_TEXTUTILS,
    LG_DEBUG_THREADS,
    LG_DEBUG_TIMER,
    LG_DEBUG_TOOLUTILS,
    LG_DEBUG_TYPES,
    LG_DEBUG_WINDOWS,
    LG_DEBUG_WORLDSCRIPT
} LG_DEBUG_CLASS;

#ifdef __cplusplus
extern "C" {
#endif
/*
 * Enable all messages of the given with level at or below the given
 * level.  level is 1 to 9.  Use level 0 to disable messages.
 *
 * In general:
 * 0   nothing: messages of this class disabled.
 * 1-3 error conditions, bad parameters, failure recovery, etc.
 * 4-7 Interesting internal goings on.
 * 8   Trap and Function entry and exit.
 * 9   Particularly voluminous or uninteresting output.
 */
extern void lg_debug_enable(LG_DEBUG_CLASS c_class, int level);

/*
 * You can also enable flags by parsing a string of the form:
 *   "module:level,module:level,module:level"
 * The module names are mapped to upper case and prefixed with
 * LG_DEBUG_ to derive the above LG_DEBUG_CLASS names.
 */
extern void lg_debug_parse(const char *string);


#ifndef __LATITUDE_KERNEL__

#include <Menus.h>
#include <Windows.h>
#include <Dialogs.h>
#include <Controls.h>
#include <TextEdit.h>
#include <Fonts.h>
#include <Quickdraw.h>

/*
 * The following routines all print interesting structures to stderr.
 */

/* 
 * Display the contents of the given ControlHandle. 
 */
extern void lg_debug_control(ControlHandle theControl);

/* 
 * Call lg_debug_control() for each controlHandle in window contrlList. 
 */
extern void lg_debug_controls(WindowPtr wPtr);

/* 
 * Display contents of the given dialog and associated item list. 
 */
extern void lg_debug_dialog(DialogPtr theDialog);

/*
 * Display contents of the four DAString globals. 
 */
extern void lg_debug_param_text(void);

/* 
 * Display info about specified dialog item list member 
 */
extern void lg_debug_dialog_item(DialogPtr theDialog, short itemNo);

/* 
 * Display contents of the event queue. 
 */
extern void lg_debug_events(void);

/* 
 * Display contents of the given FontRec. 
 */
extern void lg_debug_dump_font(FontRec **fontRec);

/* 
 * Display contents of the given WidthTable handle. 
 */
extern void lg_debug_dump_width_table(WidthTable **widthTable);

/* 
 * Display width Table in the WidthListHandle global. 
 */
extern void lg_debug_dump_width_list(void);

/* 
 * Display contents of the given MenuHandle. 
 */
extern void lg_debug_menu(MenuHandle theMenu);

/* 
 * Display menus in the given menu list handle. 
 * If menuList == 0, the MenuList global will be used. 
 */
extern void lg_debug_menu_list(Handle menuList);

/* 
 * Display contents of the given TEHandle. 
 */
extern void lg_debug_tehandle(TEHandle hTE);

/* 
 * Display contents of the given WindowRecord. 
 * If wPeek == 0, the current port will be used.
 */
extern void lg_debug_window(WindowPeek wPeek);

/* 
 * Call lg_debug_window for the given WindowRecord and all 
 * windows in wPeek->nextWindow list.
 * If wPeek == 0, the WindowList global is used. 
 */
extern void lg_debug_windowlist(WindowPeek wPeek);

/* 
 * Display port info. The port value can be type GrafPtr or CGrafPtr.
 * If port == 0, the current port will be displayed.
 */
extern void lg_debug_port(GrafPtr port);

/* 
 * Display contents of the given resource map handle. 
 */
extern void lg_debug_resource_map(Handle resourceMap);

/* 
 * Display contents of the given CTabHandle. 
 */
extern void lg_debug_ctab(CTabHandle cTab);

/* 
 * Display entries in the given CTabHandle in which 
 * red == green == blue. 
 */
extern void lg_debug_print_grays(CTabHandle cTab);

/* 
 * Display the PICT opcodes in the given PicHandle. 
 */
extern void lg_debug_pict(PicHandle thePic);

/* 
 * Display info about a volume given its volume reference number. 
 * If vRefNum == 0, info about all volumes from global VCBQHdr queue
 * will be displayed.
 */
extern void lg_debug_vol(short vRefNum);

/* 
 * Display info about a drive given its drive reference number. 
 * If dQDrive == 0, info about all drives from global DrvQHdr queue
 * will be displayed.
 */
extern void lg_debug_drive(short dQDrive);

/* 
 * Display info from the FCB of the given open file's refnum. 
 * If fileRefNum == 0, info about all open files will be displayed.
 */
extern void lg_debug_fcb(short fileRefNum);

/* 
 * Display info about the working directory given a wd refnum. 
 * If wdRefNum == 0, info about all open working directories will
 * be displayed.
 */
extern void lg_debug_wd(short wdRefNum);

/* 
 * Display all entries in a volume the given volume refnum. 
 * Directory and file catalog number info is displayed as will as
 * file system type.
 *
 * Only available for non-HFS volumes. 
 */
extern void lg_debug_dumpvol(short vRefNum);

/*
 * Display the items and flavors for the given dragref.
 */
extern void lg_debug_drag(unsigned long dragref);

/* 
 * Display all of the installed tracking and receive callbacks
 * registered with the Drag Manager.
 */
extern void lg_debug_drag_callbacks();



#endif

#ifdef __cplusplus
}
#endif

#endif /* __AP_DEBUG_H */

