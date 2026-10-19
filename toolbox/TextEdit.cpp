
#include <iostream>


extern "C" {


#include <TextEdit.h>


#include "../crossroads.h"


////////////////////////////////////////////////////////////////////////////////
// TextEdit
////////////////////////////////////////////////////////////////////////////////


void TEInit() {
  fprintf(stderr,"(TEInit)\n");
  // NOTE: Ok to skip...
}


void TEActivate(TEHandle hTE) { fprintf(stderr,"TEActivate() not implemented!\n"); }


void TEClick(Point pt, Boolean fExtend, TEHandle h) { fprintf(stderr,"TEClick() not implemented!\n"); }


void TECopy(TEHandle hTE) { fprintf(stderr,"TECopy() not implemented!\n"); }


void TECut(TEHandle hTE) { fprintf(stderr,"TECut() not implemented!\n"); }


void TEDeactivate(TEHandle hTE) { fprintf(stderr,"TEDeactivate() not implemented!\n"); }


void TEDelete(TEHandle hTE) { fprintf(stderr,"TEDelete() not implemented!\n"); }


void TEIdle(TEHandle hTE) { fprintf(stderr,"TEIdle() not implemented!\n"); }


void TEKey(short key, TEHandle hTE) { fprintf(stderr,"TEKey() not implemented!\n"); }


TEHandle TENew(const Rect *destRect, const Rect *viewRect) { fprintf(stderr,"TENew() not implemented!\n"); return NULL; }


void TEPaste(TEHandle hTE) { fprintf(stderr,"TEPaste() not implemented!\n"); }


void TEUpdate(const Rect *rUpdate, TEHandle hTE) { fprintf(stderr,"TEUpdate() not implemented!\n"); }


void TEAutoView(Boolean fAuto, TEHandle hTE) { fprintf(stderr,"TEAutoView() not implemented!\n"); }


void TEDispose(TEHandle hTE) { fprintf(stderr,"TEDispose() not implemented!\n"); }


OSErr TEFromScrap(void) { fprintf(stderr,"TEFromScrap() not implemented!\n"); return noErr; }


OSErr TEToScrap(void) { fprintf(stderr,"TEToScrap() not implemented!\n"); return noErr; }


long TEGetScrapLength(void) { fprintf(stderr,"TEGetScrapLength() not implemented!\n"); return 0; }


CharsHandle TEGetText(TEHandle hTE) { fprintf(stderr,"TEGetText() not implemented!\n"); return NULL; }


void TEScroll(short dh, short dv, TEHandle hTE) { fprintf(stderr,"TEScroll() not implemented!\n"); }


}
