/*
 * $Id: LGSane.h,v 1.6 1997/04/08 17:24:44 hempling Exp $
 * (c) Copyright 1992-1997 Metrowerks Corporation  All Rights Reserved.
 * 
 * 
 */

#include "fp.h"

/*
 * Pack7 DecStr68K calls
 */
#ifndef _LATITUDE_
#pragma parameter __A0 NumToStringTrap(__D0, __A0)
#endif
pascal StringPtr NumToStringTrap(long theNum, StringPtr theString)
#ifndef _LATITUDE_
    = { 0x3f3c, 0x0000, 0xA9EE}
#endif
;
#ifndef _LATITUDE_
#pragma parameter __D0 StringToNumTrap(__A0)
#endif
pascal long StringToNumTrap(ConstStr255Param theString)
#ifndef _LATITUDE_
    = {0x3f3c, 0x0001, 0xA9EE}
#endif
;
pascal void PStr2Dec(ConstStr255Param str, short *index, decimal *result,
		    Boolean *prefix)
#ifndef _LATITUDE_
    = {0x3f3c, 0x0002, 0xA9EE}
#endif
;
pascal void Dec2Str(const decform *f,const decimal *d,char *s)
#ifndef _LATITUDE_
    = {0x3f3c, 0x0003, 0xA9EE}
#endif
;
pascal void CStr2Dec(const char *str, short *index, decimal *result,
		    Boolean *prefix)
#ifndef _LATITUDE_
    = {0x3f3c, 0x0004, 0xA9EE}
#endif
;

