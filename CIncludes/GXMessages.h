
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
 	File:		GXMessages.h
 
 	Contains:	This file contains all of the public data structures,
 
 	Version:	Technology:	Quickdraw GX 1.1
 				Package:	Universal Interfaces 2.1 in \322MPW Latest\323 on ETO# 18
 
 	Copyright:	\251 1984-1995 by Apple Computer, Inc.
 				All rights reserved.
 
 	Bugs?:		If you find a problem with this file, use the Apple Bug Reporter
 				stack.  Include the file and version information (from above)
 				in the problem description and send to:
 					Internet:	apple.bugs@applelink.apple.com
 					AppleLink:	APPLE.BUGS
 
*/

#ifndef __GXMESSAGES__
#define __GXMESSAGES__


#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __MIXEDMODE__
#include <MixedMode.h>
#endif
/*#	include <Types.h>											*/

#ifndef __TYPES__
#include <Types.h>
#endif

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

#if defined(__MWERKS__) && GENERATING68K
#ifndef _LATITUDE_
#	pragma push
#endif
#ifndef _LATITUDE_
#	pragma pointers_in_D0
#endif
#endif
 
/*

	>>>>>> CONSTANTS <<<<<<

*/
/* Message Manager Trap */

enum {
	messageManagerTrap			= 0xABFB
};

/* Message Manager Gestalt Selector */
enum {
	gestaltMessageMgrVersion	= 'mess'
};

/* Message Manager Error Result Codes */
enum {
	messageStopLoopingErr		= -5775,
	cantDeleteRunningHandlerErr	= -5776,
	noMessageTableErr			= -5777,
	dupSignatureErr				= -5778,
	messageNotReceivedErr		= -5799
};

typedef void (*MessageGlobalsInitProcPtr)(void *messageGlobals);

#if GENERATINGCFM
typedef UniversalProcPtr MessageGlobalsInitUPP;
#else
typedef MessageGlobalsInitProcPtr MessageGlobalsInitUPP;
#endif

enum {
	uppMessageGlobalsInitProcInfo = kCStackBased
		 | STACK_ROUTINE_PARAMETER(1, SIZE_CODE(sizeof(void*)))
};

#if GENERATINGCFM
#define NewMessageGlobalsInitProc(userRoutine)		\
		(MessageGlobalsInitUPP) NewRoutineDescriptor((ProcPtr)(userRoutine), uppMessageGlobalsInitProcInfo, GetCurrentArchitecture())
#else
#define NewMessageGlobalsInitProc(userRoutine)		\
		((MessageGlobalsInitUPP) (userRoutine))
#endif

#if GENERATINGCFM
#define CallMessageGlobalsInitProc(userRoutine, messageGlobals)		\
		CallUniversalProc((UniversalProcPtr)(userRoutine), uppMessageGlobalsInitProcInfo, (messageGlobals))
#else
#define CallMessageGlobalsInitProc(userRoutine, messageGlobals)		\
		(*(userRoutine))((messageGlobals))
#endif

typedef MessageGlobalsInitProcPtr MessageGlobalsInitProc;

/*

	PUBLIC INTERFACES

	Message Handler API Routines
*/
extern long CountMessageHandlerInstances(void)
 TWOWORDINLINE(0x7028, 0xABFB);
extern void *GetMessageHandlerClassContext(void)
 TWOWORDINLINE(0x7029, 0xABFB);
extern void *SetMessageHandlerClassContext(void *anyValue)
 TWOWORDINLINE(0x702A, 0xABFB);
extern void *GetMessageHandlerInstanceContext(void)
 TWOWORDINLINE(0x702B, 0xABFB);
extern void *SetMessageHandlerInstanceContext(void *anyValue)
 TWOWORDINLINE(0x702C, 0xABFB);
extern OSErr NewMessageGlobals(long messageGlobalsSize, MessageGlobalsInitUPP initProc)
 TWOWORDINLINE(0x702D, 0xABFB);
extern void DisposeMessageGlobals(void)
 TWOWORDINLINE(0x702E, 0xABFB);
#if !GENERATINGPOWERPC
extern OSErr SendMessage(long messageSelector, ...)
 FOURWORDINLINE(0x221F, 0x7032, 0xABFB, 0x598F);
extern OSErr ForwardMessage(long messageSelector, ...)
 FOURWORDINLINE(0x221F, 0x7035, 0xABFB, 0x598F);
extern OSErr ForwardThisMessage(void)
 TWOWORDINLINE(0x7036, 0xABFB);
#endif
 
#if defined(__MWERKS__) && GENERATING68K
#ifndef _LATITUDE_
#	pragma pop
#endif
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

#endif /* __GXMESSAGES__ */



