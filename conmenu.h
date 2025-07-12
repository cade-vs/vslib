/****************************************************************************
 *
 *  Copyright (c) 1996-2022 Vladi Belperchinov-Shabanski "Cade" 
 *  http://cade.noxrun.com/  <cade@noxrun.com> <cade@bis.bg> <cade@cpan.org>
 *
 *  SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 ****************************************************************************/

#ifndef _CONMENU_H_
#define _CONMENU_H_

#include <vstring.h>
#include <wstring.h>
#include <vstrlib.h>

struct  ToggleEntry
{
  int     key;
  wchar_t name[64];
  int     *data;
  const wchar_t **states;
};

struct ConMenuInfo
{
  ConMenuInfo() { defaults(); }
  void defaults() { cn = 112; ch = 47; ti = 95; td = bo = ec = ac = 0; }

  int cn; // normal color
  int ch; // highlight color
  int ti; // title color

  int bo; // should view borders?
  int st; // scroll type -- 1 dynamic and 0 normal/static
  int td; // TAB key is assumed as KEY_DOWN (tab down)

  wchar_t ec; // exit char (used by con_menu_box)
  wchar_t ac; // alternative confirm (used by menu box)

/*
  char hide_magic[32];
*/  
};

extern ConMenuInfo con_default_menu_info;

int con_toggle_box( int x, int y, const wchar_t *title, ToggleEntry* toggles, ConMenuInfo *menu_info );
int con_menu_box( int x, int y, const wchar_t *title, WArray *wa, int hotkeys, ConMenuInfo *menu_info );

/* show full screen varray list (w/o last two lines of the screen) */
int con_full_box( int x, int y, const wchar_t *title, WArray *wa, ConMenuInfo *menu_info );

#endif //_CONMENU_H_
