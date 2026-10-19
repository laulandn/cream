
/**********************************************************************
 *
 * 	THIS FILE WAS AUTOMATICALLY GENERATED FROM APPLE INCLUDE FILES 
 * 	BY THE INSTALLATION OF LATITUDE 
 *
 * 	(c) Copyright 1992-1997   Metrowerks Corporation
 *  	All Rights Reserved.
 *
 *********************************************************************/

/*
 	File:		FileTransferTools.h
 
 	Contains:	CommToolbox File Transfer Tools Interfaces.
 
 	Version:	Technology:	System 7.5
 				Package:	Universal Interfaces 2.1 in \322MPW Latest\323 on ETO# 18
 
 	Copyright:	\251 1984-1995 by Apple Computer, Inc.
 				All rights reserved.
 
 	Bugs?:		If you find a problem with this file, use the Apple Bug Reporter
 				stack.  Include the file and version information (from above)
 				in the problem description and send to:
 					Internet:	apple.bugs@applelink.apple.com
 					AppleLink:	APPLE.BUGS
 
*/

#ifndef __FILETRANSFERTOOLS__
#define __FILETRANSFERTOOLS__


#ifndef __DIALOGS__
#include <Dialogs.h>
#endif
/*#	include <Errors.h>											*/
/*#		include <ConditionalMacros.h>							*/
/*#	include <Memory.h>											*/
/*#		include <Types.h>										*/
/*#		include <MixedMode.h>									*/
/*#	include <Menus.h>											*/
/*#		include <Quickdraw.h>									*/
/*#			include <QuickdrawText.h>							*/
/*#	include <Controls.h>										*/
/*#	include <Windows.h>										*/
/*#		include <Events.h>										*/
/*#			include <OSUtils.h>								*/
/*#	include <TextEdit.h>										*/

#ifndef __FILETRANSFERS__
#include <FileTransfers.h>
#endif
/*#	include <CTBUtilities.h>									*/
/*#		include <StandardFile.h>								*/
/*#			include <Files.h>									*/
/*#				include <Finder.h>								*/
/*#		include <AppleTalk.h>									*/
/*#	include <Connections.h>									*/
/*#	include <Terminals.h>										*/

#ifdef __cplusplus
extern "C" {
#endif

#if PRAGMA_ALIGN_SUPPORTED
#ifndef _LATITUDE_
#pragma options align=mac68k
#endif
#endif

#if PRAGMA_IMPORT_SUPPORTED
#ifndef _LATITUDE_
#pragma import on
#endif
#endif


enum {
/* DEFs */
	fdefType					= 'fdef',
	fsetType					= 'fset',
	fvalType					= 'fval',
	flocType					= 'floc',
	fscrType					= 'fscr',
	fbndType					= 'fbnd',
	fverType					= 'vers'
};

enum {
/* control */
	ftInitMsg					= 0,
	ftDisposeMsg				= 1,
	ftSuspendMsg				= 2,
	ftResumeMsg					= 3,
	ftMenuMsg					= 4,
	ftEventMsg					= 5,
	ftActivateMsg				= 6,
	ftDeactivateMsg				= 7,
	ftGetErrorStringMsg			= 8,
	ftAbortMsg					= 52,
	ftStartMsg					= 100,
	ftExecMsg					= 102,
	ftSendMsg					= 103,
	ftReceiveMsg				= 104,
/* setup */
	ftSpreflightMsg				= 0,
	ftSsetupMsg					= 1,
	ftSitemMsg					= 2,
	ftSfilterMsg				= 3,
	ftScleanupMsg				= 4,
/* validate */
	ftValidateMsg				= 0
};

enum {
	ftDefaultMsg				= 1,
/* scripting */
	ftMgetMsg					= 0,
	ftMsetMsg					= 1,
/* localization */
	ftL2English					= 0,
	ftL2Intl					= 1
};

struct FTSetupStruct {
	DialogPtr						theDialog;					/* the dialog form the application */
	short							count;						/* first appended item */
	Ptr								theConfig;					/* the config record to setup */
	short							procID;						/* procID of the tool */
};
typedef struct FTSetupStruct FTSetupStruct;

typedef FTSetupStruct *FTSetupPtr;


#if PRAGMA_IMPORT_SUPPORTED
#ifndef _LATITUDE_
#pragma import off
#endif
#endif

#if PRAGMA_ALIGN_SUPPORTED
#ifndef _LATITUDE_
#pragma options align=reset
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* __FILETRANSFERTOOLS__ */



