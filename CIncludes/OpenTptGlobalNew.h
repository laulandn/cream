
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
	File:		OpenTptGlobalNew.h

	Contains:	Definition of "new" operator that uses Open Transport's
				OTAllocMem and OTFreeMem functions.

	Copyright:	\251 1994-1996 by Apple Computer, Inc., all rights reserved.


*/

#ifndef __OPENTPTGLOBALNEW__
#define __OPENTPTGLOBALNEW__

#ifndef SystemSevenOrLater
#define SystemSevenOrLater	1
#endif

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif
#ifndef __STDDEF__
#include <StdDef.h>
#endif

#if GENERATING68K && defined(__MWERKS__)
#ifndef _LATITUDE_
#pragma pointers_in_D0
#endif
#endif
#if PRAGMA_IMPORT_SUPPORTED
#ifndef _LATITUDE_
#pragma import on
#endif
#endif

extern "C" 
{
	void* 	OTAllocMem(size_t);
	void	OTFreeMem(void*);
}

#if PRAGMA_IMPORT_SUPPORTED
#ifndef _LATITUDE_
#pragma import off
#endif
#endif
#if GENERATING68K && defined(__MWERKS__)
#ifndef _LATITUDE_
#pragma pointers_in_A0
#endif
#endif

inline void* operator new(size_t size)
{
	return OTAllocMem(size);
}

inline void* operator new(size_t size, size_t extra)
{
	return OTAllocMem(size + extra);
}

inline void operator delete(void* theMem)
{
	OTFreeMem(theMem);
}
	
#endif	/* __OPENTPTGLOBALNEW__ */



