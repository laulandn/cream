/******************************************************************************\
* Program: GMonde 1.1
* Files:   GMonde.r
*
* by Forrest Tanaka
* of Graphics/Toolbox Developer Technical Support
*
* Copyright © 1991 Apple Computer, Inc.
* All rights reserved.
\******************************************************************************/


#include "BalloonTypes.r"
#include "SysTypes.r"
#include "Types.r"

include "GMonde.¹.rsrc";

resource 'SIZE' (-1)
	{
	dontSaveScreen,
	acceptSuspendResumeEvents,
	enableOptionSwitch,
	canBackground,
	multiFinderAware,
	backgroundAndForeground,
	dontGetFrontClicks,
	ignoreChildDiedEvents,
	is32BitCompatible,
	isHighLevelEventAware,
	onlyLocalHLEvents,
	notStationeryAware,
	reserved,
	reserved,
	reserved,
	reserved,
	786432,
	393216
	};
