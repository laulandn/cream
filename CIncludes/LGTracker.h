/*
 * $Id: LGTracker.h,v 1.6 1997/04/08 17:24:45 hempling Exp $
 * (c) Copyright 1992-1997 Metrowerks Corporation  All Rights Reserved.
 * 
 * 
 */


#ifndef __LGTRACKER_H__
#define __LGTRACKER_H__

/*
 * These are the entrypoints into the mouse tracking system.
 */

typedef enum { LG_TRACKER_DOWN, LG_TRACKER_MOVED, LG_TRACKER_UP } LG_TRACKER;

/* This is the format of the motion information returned by Latitude. */
typedef struct {
    LG_TRACKER type;		/* MouseDown, MouseMoved, or MouseUp */
    int x;			/* Global (Mac) x-position */
    int y;			/* Global (Mac) y-position */
    int pressure;		/* Tablet pressure at the time of this event */
    int sequence;		/* Uniquely identifies a mouse stroke */
    unsigned long time;		/* X-server time of event */
} LG_MOTION_REC;


#ifdef __cplusplus
extern "C" {
#endif

/*
 * Initialize the motion tracker for a particular mouse stroke.
 * Pass the mouseDown.message field as the first argument, and the
 * current GrafPtr as the second.  This must be called for each mouse stroke
 * before attempting to retrieve corresponding motion information.
 */
extern void lg_latitude_tracker_initialize(unsigned long message);

/*
 * Retrieve motion events for the mouse stroke initiated by the mouseDown
 * event whose message field was passed to lg_latitude_tracker_initialize().
 * The number of available events will be returned in count.  The return
 * value will be TRUE if the last position reported corresponds to the
 * mouseUp, and FALSE if the mouseUp has not yet occurred.
 */
extern int lg_latitude_tracker_get_motion(LG_MOTION_REC **buffer, int *count);

/*
 * Flush the tracker queue for a certain message.  If message is zero, the
 * whole tracker queue will be emptied.
 */
extern void lg_latitude_tracker_flush(unsigned long message);

#ifdef __cplusplus
}
#endif

#endif /* __LGTRACKER_H__ */

