
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
 ** 	Module:		QD3DAcceleration.h										 **
 ** 																		 **
 ** 																		 **
 ** 	Purpose: 	Header file for low-level 3D driver API					 **
 ** 				Vendor IDs, and Apple's engine IDs						 **
 ** 																		 **
 ** 																		 **
 ** 	Copyright (C) 1994-95 Apple Computer, Inc.  All rights reserved.	 **
 ** 																		 **
 ** 																		 **
 *****************************************************************************/
#ifndef QD3DAcceleration_h
#define QD3DAcceleration_h

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
#endif

/******************************************************************************
 **																			 **
 ** 						Vendor ID definitions							 **
 **																			 **
 *****************************************************************************/

/*
 * If kQAVendor_BestChoice is used, the system chooses the "best" drawing engine
 * available for the target device. This should be used for the default.
 */
 
#define kQAVendor_BestChoice		(-1)

/*
 * The other definitions (kQAVendor_Apple, etc.) identify specific vendors
 * of drawing engines. When a vendor ID is used in conjunction with a
 * vendor-defined engine ID, a specific drawing engine can be selected.
 */

#define kQAVendor_Apple			0
#define kQAVendor_ATI			1
#define kQAVendor_Radius		2
#define kQAVendor_Mentor		3		/* Mentor Software, Inc. */
#define kQAVendor_Matrox		4
#define kQAVendor_Yarc			5

/******************************************************************************
 **																			 **
 **						 Apple's engine ID definitions						 **
 **																			 **
 *****************************************************************************/

#define kQAEngine_AppleSW		0		/* Default software rasterizer */
#define kQAEngine_AppleHW		(-1)	/* Apple accelerator */
#define kQAEngine_AppleHW2		1		/* Another Apple accelerator */

#ifdef __cplusplus
}
#endif

#if defined(__MWERKS__)
#ifndef _LATITUDE_
#	pragma options align=reset
#endif
#ifndef _LATITUDE_
#	pragma enumsalwaysint reset
#endif
#endif

#endif /* QD3DAcceleration_h */



