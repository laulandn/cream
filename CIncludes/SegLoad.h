
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
 	File:		SegLoad.h
 
 	Contains:	Segment Loader Interfaces.
 
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

#ifndef __SEGLOAD__
#define __SEGLOAD__


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
	appOpen						= 0,							/*Open the Document (s)*/
	appPrint					= 1								/*Print the Document (s)*/
};

struct AppFile {
	short							vRefNum;
	OSType							fType;
	short							versNum;					/*versNum in high byte*/
	Str255							fName;
};
typedef struct AppFile AppFile;

/*
	Because PowerPC applications don\325t have segments,
	UnloadSeg is unsupported for PowerPC.
*/
#if GENERATING68K
extern pascal void UnloadSeg(void *routineAddr)
 ONEWORDINLINE(0xA9F1);
#else
#define UnloadSeg(x)
#endif
#if !GENERATINGCFM
extern pascal void CountAppFiles(short *message, short *count);
extern pascal void GetAppFiles(short index, AppFile *theFile);
extern pascal void ClrAppFiles(short index);
extern pascal void GetAppParms(Str255 apName, short *apRefNum, Handle *apParam)
 ONEWORDINLINE(0xA9F5);
#if CGLUESUPPORTED
extern void getappparms(char *apName, short *apRefNum, Handle *apParam);
#endif
#endif
#if OLDROUTINELOCATIONS
extern pascal void ExitToShell(void)
 ONEWORDINLINE(0xA9F4);
#endif

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

#endif /* __SEGLOAD__ */



