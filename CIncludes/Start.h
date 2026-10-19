
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
 	File:		Start.h
 
 	Contains:	Start Manager Interfaces.
 
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

#ifndef __START__
#define __START__


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

struct SlotDev {
	SignedByte						sdExtDevID;
	SignedByte						sdPartition;
	SignedByte						sdSlotNum;
	SignedByte						sdSRsrcID;
};
typedef struct SlotDev SlotDev, *SlotDevPtr;

struct SCSIDev {
	SignedByte						sdReserved1;
	SignedByte						sdReserved2;
	short							sdRefNum;
};
typedef struct SCSIDev SCSIDev, *SCSIDevPtr;

union DefStartRec {
	SlotDev							slotDev;
	SCSIDev							scsiDev;
};
typedef union DefStartRec DefStartRec;

typedef DefStartRec *DefStartPtr;

struct DefVideoRec {
	SignedByte						sdSlot;
	SignedByte						sdsResource;
};
typedef struct DefVideoRec DefVideoRec;

typedef DefVideoRec *DefVideoPtr;

struct DefOSRec {
	SignedByte						sdReserved;
	SignedByte						sdOSType;
};
typedef struct DefOSRec DefOSRec;

typedef DefOSRec *DefOSPtr;


#if !GENERATINGCFM
#ifndef _LATITUDE_
#pragma parameter GetDefaultStartup(__A0)
#endif
#endif
extern pascal void GetDefaultStartup(DefStartPtr paramBlock)
 ONEWORDINLINE(0xA07D);

#if !GENERATINGCFM
#ifndef _LATITUDE_
#pragma parameter SetDefaultStartup(__A0)
#endif
#endif
extern pascal void SetDefaultStartup(DefStartPtr paramBlock)
 ONEWORDINLINE(0xA07E);

#if !GENERATINGCFM
#ifndef _LATITUDE_
#pragma parameter GetVideoDefault(__A0)
#endif
#endif
extern pascal void GetVideoDefault(DefVideoPtr paramBlock)
 ONEWORDINLINE(0xA080);

#if !GENERATINGCFM
#ifndef _LATITUDE_
#pragma parameter SetVideoDefault(__A0)
#endif
#endif
extern pascal void SetVideoDefault(DefVideoPtr paramBlock)
 ONEWORDINLINE(0xA081);

#if !GENERATINGCFM
#ifndef _LATITUDE_
#pragma parameter GetOSDefault(__A0)
#endif
#endif
extern pascal void GetOSDefault(DefOSPtr paramBlock)
 ONEWORDINLINE(0xA084);

#if !GENERATINGCFM
#ifndef _LATITUDE_
#pragma parameter SetOSDefault(__A0)
#endif
#endif
extern pascal void SetOSDefault(DefOSPtr paramBlock)
 ONEWORDINLINE(0xA083);
extern pascal void SetTimeout(short count);
extern pascal void GetTimeout(short *count);

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

#endif /* __START__ */



