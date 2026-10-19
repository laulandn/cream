
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
 ** 	Module:		QD3DRenderer.h											 **
 ** 																		 **
 ** 																		 **
 ** 	Purpose: 	Renderer types and routines	 						 	 **
 ** 																		 **
 ** 																		 **
 ** 																		 **
 ** 	Copyright (C) 1992-1995 Apple Computer, Inc.  All rights reserved.	 **
 ** 																		 **
 ** 																		 **
 *****************************************************************************/
#ifndef QD3DRenderer_h
#define QD3DRenderer_h

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
#endif	/* __cplusplus */

/******************************************************************************
 **																			 **
 **							Renderer Functions								 **
 **																			 **
 *****************************************************************************/

QD3D_EXPORT TQ3RendererObject Q3Renderer_NewFromType(
	TQ3ObjectType			rendererObjectType);

QD3D_EXPORT TQ3ObjectType Q3Renderer_GetType(
	TQ3RendererObject		renderer);

QD3D_EXPORT TQ3Status Q3Renderer_Flush(
	TQ3RendererObject		renderer,
	TQ3ViewObject			view);
	
QD3D_EXPORT TQ3Status Q3Renderer_Sync(
	TQ3RendererObject		renderer,
	TQ3ViewObject			view);


/******************************************************************************
 **																			 **
 **						Interactive Renderer Specific Functions				 **
 **																			 **
 *****************************************************************************/

/* CSG IDs attribute */
#define kQ3AttributeType_ConstructiveSolidGeometryID		Q3_OBJECT_TYPE('c','s','g','i')

/* Object IDs, to be applied as attributes on geometries */
#define kQ3SolidGeometryObjA 0
#define kQ3SolidGeometryObjB 1
#define kQ3SolidGeometryObjC 2
#define kQ3SolidGeometryObjD 3
#define kQ3SolidGeometryObjE 4

/* Possible CSG equations */

typedef enum TQ3CSGEquation {
	kQ3CSGEquationAandB			= (int) 0x88888888,
	kQ3CSGEquationAandnotB 		= 0x22222222,
	kQ3CSGEquationAanBonCad		= 0x2F222F22,
	kQ3CSGEquationnotAandB		= 0x44444444,
	kQ3CSGEquationnAaBorCanB	= 0x74747474
} TQ3CSGEquation;

QD3D_EXPORT TQ3Status Q3InteractiveRenderer_SetCSGEquation(
	TQ3RendererObject		renderer,
	TQ3CSGEquation			equation);

QD3D_EXPORT TQ3Status Q3InteractiveRenderer_GetCSGEquation(
	TQ3RendererObject		renderer,
	TQ3CSGEquation			*equation);

QD3D_EXPORT TQ3Status Q3InteractiveRenderer_SetPreferences(
	TQ3RendererObject		renderer,
	long					vendorID,
	long					engineID);

QD3D_EXPORT TQ3Status Q3InteractiveRenderer_GetPreferences(
	TQ3RendererObject		renderer,
	long					*vendorID,
	long					*engineID);
	
QD3D_EXPORT TQ3Status Q3InteractiveRenderer_SetDoubleBufferBypass(
	TQ3RendererObject		renderer,
	TQ3Boolean				bypass);

QD3D_EXPORT TQ3Status Q3InteractiveRenderer_GetDoubleBufferBypass(
	TQ3RendererObject		renderer,
	TQ3Boolean				*bypass);
	
#ifdef __cplusplus
}
#endif	/* __cplusplus */

#if defined(__MWERKS__)
#ifndef _LATITUDE_
#pragma options align=reset
#endif
#ifndef _LATITUDE_
#pragma enumsalwaysint reset
#endif
#endif

#endif  /*  QD3DRenderer_h  */



