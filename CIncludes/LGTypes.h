/*
 * $Id: LGTypes.h,v 1.8 1997/04/08 17:24:46 hempling Exp $
 * (c) Copyright 1992-1997 Metrowerks Corporation  All Rights Reserved.
 * 
 * 
 */

#ifndef __LGTYPES_H
#define __LGTYPES_H

#ifdef _LATITUDE_

#define pascal

#endif  /* _LATITUDE_ */

#if defined(__LATITUDE_KERNEL__) && !defined(OLDROUTINENAMES)
#define OLDROUTINENAMES 0
#endif

#define USES68KINLINES 0

#define USESROUTINEDESCRIPTORS 0

#define QUADCONST(a, b, c, d)                   \
            (((long) ((a) & 0xff) << 24)        \
             | ((long) ((b) & 0xff) << 16)      \
             | ((long) ((c) & 0xff) << 8)       \
             | ((long) ((d) & 0xff)))

#endif  /* __LGTYPES_H */
