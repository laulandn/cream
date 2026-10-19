
#include <iostream>


extern "C" {


#include <Dialogs.h>


#include "../crossroads.h"


////////////////////////////////////////////////////////////////////////////////
// Dialogs
////////////////////////////////////////////////////////////////////////////////


void InitDialogs(void *ignored) {
  fprintf(stderr,"(InitDialogs)\n");
  open_libs(0,NULL);
  // NOTE: Ok to skip...
}


void DialogCopy(DialogRef theDialog) {
  if(!theDialog) { fprintf(stderr,"DialogCopy() theDialog is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"DialogCopy() not implemented!\n");
}


void DialogCut(DialogRef theDialog) {
  if(!theDialog) { fprintf(stderr,"DialogCut() theDialog is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"DialogCut() not implemented!\n");
}


void DialogDelete(DialogRef theDialog) {
  if(!theDialog) { fprintf(stderr,"DialogDelete() theDialog is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"DialogDelete() not implemented!\n");
}


void DialogPaste(DialogRef theDialog) {
  if(!theDialog) { fprintf(stderr,"DialogPaste() theDialog is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"DialogPaste() not implemented!\n");
}


void SelectDialogItemText(DialogRef theDialog, short itemNo, short strtSel, short endSel) {
  if(!theDialog) { fprintf(stderr,"SelectDialogItemText() theDialog is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"SelectDialogItemText() not implemented!\n");
}


void DisposeDialog(DialogRef theDialog)
{
  if(!theDialog) { fprintf(stderr,"DisposeDialog() theDialog is NULL!\n"); exit(EXIT_FAILURE); }
  //fprintf(stderr,"DisposeDialog() not implemented!\n");
}


DialogRef GetNewDialog(short dialogID, void *dStorage, WindowRef behind)
{
  fprintf(stderr,"GetNewDialog() not implemented!\n");
  return 0;
}


short Alert(short alertID, ModalFilterUPP modalFilter)
{
  fprintf(stderr,"Alert() not implemented!\n");
  return 0;
}


void ModalDialog(ModalFilterUPP modalFilter, short *itemHit)
{
  fprintf(stderr,"ModalDialog() not implemented!\n");
}


Boolean DialogSelect(const EventRecord *theEvent, DialogRef *theDialog, short *itemHit)
{
  fprintf(stderr,"DialogSelect() not implemented!\n");
  return FALSE;
}


void GetDialogItem(DialogRef theDialog, short itemNo, short *itemType, Handle *item, Rect *box)
{
  if(!theDialog) { fprintf(stderr,"GetDialogItem() theDialog is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"GetDialogItem() not implemented!\n");
}


Boolean IsDialogEvent(const EventRecord *e)
{
  if(!e) { fprintf(stderr,"IsDialogEvent() e is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"IsDialogEvent() not implemented!\n");
  return FALSE;
}


}
