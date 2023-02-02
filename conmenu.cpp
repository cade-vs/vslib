/****************************************************************************
 *
 *  Copyright (c) 1996-2022 Vladi Belperchinov-Shabanski "Cade" 
 *  http://cade.noxrun.com/  <cade@noxrun.com> <cade@bis.bg> <cade@cpan.org>
 *
 *  SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 ****************************************************************************/

#include <unicon.h>
#include <scroll.h>
#include "conmenu.h"

ConMenuInfo con_default_menu_info;

int con_toggle_box( int x, int y, const wchar_t *title, ToggleEntry* toggles, ConMenuInfo *menu_info  )
{
  ScrollPos scroll;
  int z;

  if( x < 0 ) x = con_max_x() + x;
  if( y < 0 ) y = con_max_y() + y;

  int w = -1;
  int h = -1;

  int maxlen = 0;
  int count  = 0;
  while( toggles[count].key != 0 )
    {
    int sl = str_len( toggles[count].name );
    if (sl > maxlen) maxlen = sl;
    count++;
    }
  maxlen += 6+3; // 6 for state string and 3 for " key " string in the front

  if (w == -1) w = maxlen;
  if (h == -1) h = count;

  z = str_len( title );
  if (w < z) w = z;
  if (h > count) h = count;
  if (h == 0) h = 1;

  // FIXME: those should be fixed!!!
  if (x + w > con_max_x()  ) w = con_max_x() - x - 1;
  if (y + h > con_max_y()-4) h = con_max_y() - y - 5;

  WString str;
  WString str1;
  WString hots = L"";
  for(z = 0; z < count; z++)
    if ( wcsncmp( L"--", toggles[z].name, 2 ) )
      str_add_ch( hots, toggles[z].key );
    else
      str_add_ch( hots, L' ' );
  con_xy(x,y);

  str = L"";
  str = title;
  str_pad( str,-w, menu_info->bo ? L'-' : L' ' );
  if (str_len(str) > w) str_sleft(str,w);
  if (menu_info->bo)
    str = L".-" + str + L"-.";
  else
    str = L"  " + str + L"  ";
  con_out(x,y,VString(str),menu_info->ti);
  y++;

  scroll.wrap = 1;
  scroll.set_min_max( 0, count-1 );
  scroll.set_pagesize( h );
  scroll.go( 0 );

  while(1)
    {
    for( z = 0; z < scroll.pagesize(); z++ )
      {
      if (scroll.page() + z >= count)
        {
        str = L" ~";
        str_pad( str, -(w+2), L' ');
        }
      else
        {
        int sep = (wcsncmp(L"--", toggles[scroll.page()+z].name, 2) == 0);
        if (sep)
          {
          str = L"";
          str += toggles[scroll.page()+z].name;
          str_pad( str, -w, L'-');
          str += L"--";
          }
        else
          {
          str = L" "; str_add_ch( str, toggles[scroll.page()+z].key ); str += L" ";
          str += toggles[scroll.page()+z].name;
          str_pad( str, -(w-6), L' ');
          str1 = toggles[scroll.page()+z].states[*(toggles[scroll.page()+z].data)];
          str_pad( str1, 6, L' '); str1 += L" ";
          str += L" " + str1;
          }
        }
      if (menu_info->bo)
        str = L"|" + str + L"|";
      else
        str = L" " + str + L" ";
//      if (str.len() > w) StrSLeft(str,w);
//      str = " " + str + " ";
      con_out( x, y+z, VString(str), ( scroll.page()+z != scroll.pos() ) ? menu_info->cn : menu_info->ch );
      }
    if (menu_info->bo)
      {
      str = L"";
      str_pad( str, w+2, L'-' );
      str = L"`" + str + L"'";
      con_out( x, y+scroll.pagesize(), VString(str), menu_info->cn );
      }
    wchar_t wch = con_getwch();
    menu_info->ec = wch;

    if ( wch == UKEY_UP   ) scroll.up();
    if ( wch == UKEY_DOWN ) scroll.down();
    if ( wch == UKEY_PGUP ) scroll.ppage();
    if ( wch == UKEY_PGDN ) scroll.npage();
    if ( wch == UKEY_HOME ) scroll.home();
    if ( wch == UKEY_END  ) scroll.end();

    if ( wch == 27 || wch == 8 ||  wch == UKEY_BACKSPACE ) return 0; // exit on ESC or BS
//    if ( ch < 0 || ch > 255 ) continue; //FIXME: unicode?
    if ( wch == 13 /* && wcsncmp(L"--", toggles[scroll.pos].name, 2) */ ) return 1;
    z = ( wch == L' ' ) ? scroll.pos() : str_find( hots, wch );
    if ( z > 0 && wcsncmp(L"--", toggles[z].name, 2) )
      {
      int state = *(toggles[z].data) + 1;
      if (toggles[z].states[state] == NULL) state = 0;
      *(toggles[z].data) = state;
      }
    }
  return -1;
}

int con_menu_box( int x, int y, const wchar_t *title, WArray *wa, int hotkeys, ConMenuInfo *menu_info  )
{
  ScrollPos scroll;
  int z;

  if( x < 0 ) x = con_max_x() + x;
  if( y < 0 ) y = con_max_y() + y;

  int w = -1;
  int h = -1;

  if (w == -1) w = wa->max_len();
  if (h == -1) h = wa->count();

  z = str_len(title);
  if (w < z) w = z;
  if (h > wa->count()) h = wa->count();
  if (h == 0) h = 1;

  // FIXME: those should be fixed!!!
  if (x + w > con_max_x()  ) w = con_max_x() - x - 4;
  if (y + h > con_max_y()-4) h = con_max_y() - y - 4;

  WString str;
  WString hots = L"";
  if ( hotkeys > -1 )
    {
    for(z = 0; z < wa->count(); z++)
      if (wcsncmp(L"--", wa->get(z), 2))
        str_add_ch( hots, wchar_t(((const wchar_t*)(wa->get(z)))[hotkeys]) );
      else
        str_add_ch( hots,L' ' );
    str_up(hots);
    }
  con_xy(x,y);

  str = L" ";
  str += title;
  str += L" ";
  str_pad( str,-(w), menu_info->bo ? L'-' : L' ' );
  if (str_len(str) > w) str_sleft(str,w);
  if (menu_info->bo)
    str = L".-" + str + L"-.";
  else
    str = L"  " + str + L"  ";
  con_out(x,y,VString(str),menu_info->ti);
  y++;

  scroll.wrap = 1;
  scroll.set_min_max( 0, wa->count()-1 );
  scroll.set_pagesize( h );
  scroll.go( 0 );

  while(1)
    {
    for( z = 0; z < scroll.pagesize(); z++ )
      {
      str = (scroll.page()+z >= wa->count())? L"~" : wa->get(scroll.page()+z);

/*
      if ( menu_info->hide_magic[0] )
        {
        int i = str_rfind( str, menu_info->hide_magic );
        if ( i != -1)
          str_sleft( str, i );
        }
*/
      str_pad( str,-w , (wcsncmp(L"--", str, 2) == 0)?L'-':L' ');
      if (str_len(str) > w)
        str = str_dot_reduce( str, w );
      if (menu_info->bo)
        str = L"| " + str + L" |";
      else
        str = L"  " + str + L"  ";
      con_out( x, y+z, VString(str), ( scroll.page()+z != scroll.pos() ) ? menu_info->cn : menu_info->ch );
      }
    if (menu_info->bo)
      {
      str = L"";
      str_pad( str, w+2, L'-' );
      str = L"`" + str + L"'";
      con_out( x, y+scroll.pagesize(), VString(str), menu_info->cn );
      }
    wchar_t wch = con_getwch();
    menu_info->ec = wch;

    if ( wch == UKEY_UP   ) scroll.up();
    if ( wch == UKEY_DOWN ) scroll.down();
    if ( wch == UKEY_PGUP ) scroll.ppage();
    if ( wch == UKEY_PGDN ) scroll.npage();
    if ( wch == UKEY_HOME ) scroll.home();
    if ( wch == UKEY_END  ) scroll.end();

    if ( wch == 27 || wch == 8 ||  wch == UKEY_BACKSPACE )
      {
      // exit on ESC or BS
      menu_info->ac = 0;
      return -1;
      }
//    if ( ch < 0 || ch > 255 ) continue; //FIXME: unicode?
    if ( wch == 13 )
      {
      if (wcsncmp(L"--", wa->get(scroll.pos()), 2) != 0) // ako e "--" e separator
        {
        menu_info->ec = hots[scroll.pos()];
        menu_info->ac = 0;
        return scroll.pos();
        }
      }
    if ( menu_info->ac > 0 && wch == menu_info->ac )
      {
      if (wcsncmp( L"--", wa->get(scroll.pos()), 2) != 0) // ako e "--" e separator
        {
        menu_info->ec = menu_info->ac;
//        menu_info->ec = hots[scroll.pos()];
        menu_info->ac = 0;
        return scroll.pos();
        }
      }
    z = str_find( hots, towupper(wch) );
    if ( z > -1 )
      {
      menu_info->ec = hots[z];
      menu_info->ac = 0;
      return z;
      }
    }
  menu_info->ac = 0;
  return -1;
}

int con_full_box( int x __attribute__((unused)), int y __attribute__((unused)), const wchar_t *title, WArray *wa, ConMenuInfo *menu_info )
{
  ScrollPos scroll;
  scroll.wrap = 0;
  scroll.set_min_max( 0, wa->count()-1 );
  scroll.set_pagesize( con_max_y() - 3 ); /* one title and two status */
  scroll.go( 0 );

  wchar_t pos_str[32];

  con_xy( 1, 1 );
  con_puts( VString( title ), menu_info->ti );
  con_ce( menu_info->ti );
  while(4)
    {
    WString str;
    int z;
    for( z = 0; z < scroll.pagesize(); z++ )
      {
      if ( scroll.page() + z < wa->count() )
        str = wa->get( scroll.page() + z );
      else
        str = L"~";
      str = str_dot_reduce( str, con_max_x()-1 );
      con_xy( 1, z + 2 );
      int c = ( scroll.page() + z == scroll.pos() ) ? menu_info->ch : menu_info->cn;
      con_puts( VString(str), c );
      con_ce( c );
      }
    swprintf( pos_str, (sizeof(pos_str)/sizeof(wchar_t))-1, L" %5d of %5d", scroll.pos()+1, scroll.max()+1 );
    con_out( con_max_x() - 15, 1, VString(pos_str), menu_info->ti );
    int wch;
    switch( (wch = con_getwch()) )
      {
      case 27 : menu_info->ec = 27; return -1; break;
      case  8 : menu_info->ec =  8; return -1; break;
      case UKEY_BACKSPACE : menu_info->ec =  UKEY_BACKSPACE; return -1; break;
      case 13 : menu_info->ec = 13; return scroll.pos(); break;
      case UKEY_UP   : scroll.up(); break;
      case UKEY_DOWN : scroll.down(); break;
      case UKEY_PGUP : scroll.ppage(); break;
      case UKEY_PGDN : scroll.npage(); break;
      case UKEY_HOME : scroll.home(); break;
      case UKEY_END  : scroll.end(); break;
      default:
        if ( towlower( wch ) == towlower( menu_info->ac ) )
          {
          menu_info->ec = menu_info->ac;
          return -2;
          }
          break;
      }
    }
}

