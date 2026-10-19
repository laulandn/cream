
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
 	File:		CRMSerialDevices.h
 
 	Contains:	Communications Resource Manager Serial Device interfaces.
 
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

#ifndef __CRMSERIALDEVICES__
#define __CRMSERIALDEVICES__


#ifndef __TYPES__
#include <Types.h>
#endif
/*#	include <ConditionalMacros.h>								*/

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
/* 	for the crmDeviceType field of the CRMRec data structure	*/
	crmSerialDevice				= 1,
/*	version of the CRMSerialRecord below	*/
	curCRMSerRecVers			= 1
};

/* Maintains compatibility w/ apps & tools that expect an old style icon	*/
struct CRMIconRecord {
	long							oldIcon[32];				/* ICN#	*/
	long							oldMask[32];
	Handle							theSuite;					/* Handle to an IconSuite	*/
	long							reserved;
};
typedef struct CRMIconRecord CRMIconRecord;

typedef CRMIconRecord *CRMIconPtr, **CRMIconHandle;

struct CRMSerialRecord {
	short							version;
	StringHandle					inputDriverName;
	StringHandle					outputDriverName;
	StringHandle					name;
	CRMIconHandle					deviceIcon;
	long							ratedSpeed;
	long							maxSpeed;
	long							reserved;
};
typedef struct CRMSerialRecord CRMSerialRecord;

typedef CRMSerialRecord *CRMSerialPtr;


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

#endif /* __CRMSERIALDEVICES__ */



