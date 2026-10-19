
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
 	File:		CMPRComponent.h
 
 	Contains:	ColorSync ProfileResponder Components Interface 
 
 	Version:	Technology:	ColorSync 2.0
 				Package:	Universal Interfaces 2.1 in \322MPW Latest\323 on ETO# 18
 
 	Copyright:	\251 1984-1995 by Apple Computer, Inc.
 				All rights reserved.
 
 	Bugs?:		If you find a problem with this file, use the Apple Bug Reporter
 				stack.  Include the file and version information (from above)
 				in the problem description and send to:
 					Internet:	apple.bugs@applelink.apple.com
 					AppleLink:	APPLE.BUGS
 
*/

#ifndef __CMPRCOMPONENT__
#define __CMPRCOMPONENT__


#ifndef __TYPES__
#include <Types.h>
#endif
/*#	include <ConditionalMacros.h>								*/

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
#endif
/*#	include <MixedMode.h>										*/
/*#	include <QuickdrawText.h>									*/

#ifndef __COMPONENTS__
#include <Components.h>
#endif

#ifndef __CMAPPLICATION__
#include <CMApplication.h>
#endif
/*#	include <Files.h>											*/
/*#		include <OSUtils.h>									*/
/*#			include <Memory.h>									*/
/*#		include <Finder.h>										*/
/*#	include <Printing.h>										*/
/*#		include <Errors.h>										*/
/*#		include <Dialogs.h>									*/
/*#			include <Menus.h>									*/
/*#			include <Controls.h>								*/
/*#			include <Windows.h>								*/
/*#				include <Events.h>								*/
/*#			include <TextEdit.h>								*/
/*#	include <CMICCProfile.h>									*/

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
	CMPRInterfaceVersion		= 0
};

/* Component function selectors */
enum {
	kCMPRGetProfile				= 0,
	kCMPRSetProfile				= 1,
	kCMPRSetProfileDescription	= 2,
	kCMPRGetIndexedProfile		= 3,
	kCMPRDeleteDeviceProfile	= 4
};

extern pascal CMError CMGetProfile(ComponentInstance pr, CMProfileHandle aProfile, CMProfileHandle *returnedProfile)
 FIVEWORDINLINE(0x2F3C, 8, 0, 0x7000, 0xA82A);
extern pascal CMError CMSetProfile(ComponentInstance pr, CMProfileHandle newProfile)
 FIVEWORDINLINE(0x2F3C, 4, 1, 0x7000, 0xA82A);
extern pascal CMError CMSetProfileDescription(ComponentInstance pr, long DeviceData, CMProfileHandle hProfile)
 FIVEWORDINLINE(0x2F3C, 8, 2, 0x7000, 0xA82A);
extern pascal CMError CMGetIndexedProfile(ComponentInstance pr, CMProfileSearchRecordHandle search, CMProfileHandle *returnProfile, long *index)
 FIVEWORDINLINE(0x2F3C, 12, 3, 0x7000, 0xA82A);
extern pascal CMError CMDeleteDeviceProfile(ComponentInstance pr, CMProfileHandle deleteMe)
 FIVEWORDINLINE(0x2F3C, 4, 4, 0x7000, 0xA82A);

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

#endif /* __CMPRCOMPONENT__ */



