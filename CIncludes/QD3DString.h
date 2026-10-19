
/**********************************************************************
 *
 * 	THIS FILE WAS AUTOMATICALLY GENERATED FROM APPLE INCLUDE FILES 
 * 	BY THE INSTALLATION OF LATITUDE 
 *
 * 	(c) Copyright 1992-1997   Metrowerks Corporation
 *  	All Rights Reserved.
 *
 *********************************************************************/

/******************************************************************************
 **																			 **
 ** 	Module:		QD3DString.h 											 **
 ** 																		 **
 ** 																		 **
 ** 	Purpose:														   	 **
 ** 																		 **
 ** 																		 **
 ** 																		 **
 ** 	Copyright (C) 1994-1995 Apple Computer, Inc. All rights reserved.	 **
 ** 																		 **
 ** 																		 **
 *****************************************************************************/
#ifndef QD3DString_h
#define QD3DString_h

#ifndef QD3D_h
#include <QD3D.h>
#endif  /*  QD3D_h  */

#if PRAGMA_ONCE
#ifndef _LATITUDE_
#	pragma once
#endif
#endif

#if defined(__MWERKS__)
#ifndef _LATITUDE_
#	pragma enumsalwaysint on
#endif
#ifndef _LATITUDE_
#	pragma align_array_members off
#endif
#ifndef _LATITUDE_
#	pragma options align=native
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif /*  __cplusplus  */


/******************************************************************************
 **																			 **
 **								String Routines								 **
 **																			 **
 *****************************************************************************/
 
TQ3ObjectType Q3String_GetType(
	TQ3StringObject		stringObj);


/******************************************************************************
 **																			 **
 **						C String Routines									 **
 **																			 **
 *****************************************************************************/

TQ3StringObject Q3CString_New(
	const char				*string);

TQ3Status Q3CString_GetLength(
	TQ3StringObject			stringObj,
	unsigned long			*length);

TQ3Status Q3CString_SetString(
	TQ3StringObject			stringObj,
	const char				*string);

TQ3Status Q3CString_GetString(
	TQ3StringObject			stringObj,
	char					**string);

TQ3Status Q3CString_EmptyData(
	char					**string);

#ifdef __cplusplus
}
#endif /*  __cplusplus  */

#if defined(__MWERKS__)
#ifndef _LATITUDE_
#pragma options align=reset
#endif
#ifndef _LATITUDE_
#pragma enumsalwaysint reset
#endif
#endif

#endif  /*  QD3DString_h  */



