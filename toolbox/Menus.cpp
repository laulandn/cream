
#include <iostream>


extern "C" {


#include <Menus.h>
#include <Errors.h>


#include "../crossroads.h"


////////////////////////////////////////////////////////////////////////////////
// Menus
////////////////////////////////////////////////////////////////////////////////


void InitMenus() {
  fprintf(stderr,"(InitMenus)\n");
  open_libs(0,NULL);
  // NOTE: Ok to skip...
}


void ClearMenuBar() {
  fprintf(stderr,"ClearMenuBar() not implemented!\n");
}


short GetMBarHeight() {
  // Not true, but good enough
  return 12;
}


void GetMenuItemText(MenuRef theMenu, short item, Str255 itemString) {
  if(!theMenu) { fprintf(stderr,"GetMenuItemText() theMenu is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"GetMenuItemText() not implemented!\n");
}


void HiliteMenu(short menuID) {
  //fprintf(stderr,"HiliteMenu() not implemented!\n");
}


long MenuKey(short ch) {
  fprintf(stderr,"MenuKey() not implemented!\n");
  return 0;
}


long MenuSelect(Point startPt) {
  fprintf(stderr,"MenuSelect() not implemented!\n");
  return 0;
}


MenuRef NewMenu(short menuID, ConstStr255Param menuTitle) {
  fprintf(stderr,"NewMenu() not implemented!\n");
  //return dsHMenuFindErr;
  return 0;
}


void InsertMenu(MenuRef theMenu, short beforeID) {
  if(!theMenu) { fprintf(stderr,"InsertMenu() theMenu is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"InsertMenu() not implemented!\n");
}


void DrawMenuBar() {
  // NOTE: Ok to skip...
  //fprintf(stderr,"DrawMenuBar() not implemented!\n");
}


void AppendMenu(MenuRef menu, ConstStr255Param data) {
  if(!menu) { fprintf(stderr,"AppendMenu() menu is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"AppendMenu() not implemented!\n");
}


void AppendResMenu(MenuRef theMenu, ResType theType) {
  if(!theMenu) { fprintf(stderr,"AppendResMenu() theMenu is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"AppendResMenu() not implemented!\n");
}


//void AddResMenu(MenuRef theMenu, ResType theTyp) {
  //fprintf(stderr,"AddResMenu() not implemented!\n");
//}


Boolean SystemEdit(short editCmd) {
  fprintf(stderr,"SystemEdit() not implemented!\n");
  return false;
}


void EnableItem(MenuRef theMenu, short item)
{
  if(!theMenu) { fprintf(stderr,"EnableItem() theMenu is NULL!\n"); exit(EXIT_FAILURE); }
  //fprintf(stderr,"EnableItem() not implemented!\n");
}


void CheckItem(MenuRef theMenu, short item, Boolean checked)
{
  if(!theMenu) { fprintf(stderr,"CheckItem() theMenu is NULL!\n"); exit(EXIT_FAILURE); }
  //fprintf(stderr,"CheckItem() not implemented!\n");
}


MenuRef GetMenu(short resourceID)
{
  fprintf(stderr,"GetMenu() not implemented!\n");
  return NULL;
}


MenuRef GetMenuHandle(short menuID)
{
  fprintf(stderr,"GetMenuHandle() not implemented!\n");
  return NULL;
}


Handle GetNewMBar(short menuBarID) { 
  fprintf(stderr,"GetNewMBar() not implemented!\n"); 
  return NULL; 
}


void DisableItem(MenuRef theMenu, short item)
{
  if(!theMenu) { fprintf(stderr,"DisableItem() theMenu is NULL!\n"); exit(EXIT_FAILURE); }
  //fprintf(stderr,"DisableItem() not implemented!\n");
}


void SetMenuBar(Handle menuList)
{
  if(!menuList) { fprintf(stderr,"SetMenuBar() menuList is NULL!\n"); exit(EXIT_FAILURE); }
  fprintf(stderr,"SetMenuBar() not implemented!\n");
}


}
