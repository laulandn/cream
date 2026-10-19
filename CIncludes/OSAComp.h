
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
 	File:		OSAComp.h
 
 	Contains:	AppleScript Component Implementor's Interfaces.
 
 	Version:	Technology:	AppleScript 1.1
 				Package:	Universal Interfaces 2.1 in \322MPW Latest\323 on ETO# 18
 
 	Copyright:	\251 1984-1995 by Apple Computer, Inc.
 				All rights reserved.
 
 	Bugs?:		If you find a problem with this file, use the Apple Bug Reporter
 				stack.  Include the file and version information (from above)
 				in the problem description and send to:
 					Internet:	apple.bugs@applelink.apple.com
 					AppleLink:	APPLE.BUGS
 
*/

#ifndef __OSACOMP__
#define __OSACOMP__


#ifndef __APPLEEVENTS__
#include <AppleEvents.h>
#endif
/*#	include <Errors.h>											*/
/*#		include <ConditionalMacros.h>							*/
/*#	include <Types.h>											*/
/*#	include <Memory.h>											*/
/*#		include <MixedMode.h>									*/
/*#	include <OSUtils.h>										*/
/*#	include <Events.h>											*/
/*#		include <Quickdraw.h>									*/
/*#			include <QuickdrawText.h>							*/
/*#	include <EPPC.h>											*/
/*#		include <AppleTalk.h>									*/
/*#		include <Files.h>										*/
/*#			include <Finder.h>									*/
/*#		include <PPCToolbox.h>									*/
/*#		include <Processes.h>									*/
/*#	include <Notification.h>									*/

#ifndef __OSA__
#include <OSA.h>
#endif
/*#	include <AEObjects.h>										*/
/*#	include <Components.h>										*/

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

extern pascal OSErr OSAGetStorageType(Handle scriptData, DescType *dscType);
extern pascal OSErr OSAAddStorageType(Handle scriptData, DescType dscType);
extern pascal OSErr OSARemoveStorageType(Handle scriptData);

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

#endif /* __OSACOMP__ */



