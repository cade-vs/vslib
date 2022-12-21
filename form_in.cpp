/****************************************************************************
 *
 *  Copyright (c) 1996-2022 Vladi Belperchinov-Shabanski "Cade" 
 *  http://cade.noxrun.com/  <cade@noxrun.com> <cade@bis.bg> <cade@cpan.org>
 *
 *  SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 ****************************************************************************/

#include <vstring.h>
#include <wstring.h>
#include "form_in.h"
#include "scroll.h"

int EditStrBF = CONCOLOR( chWHITE, cBLUE );
int EditStrFH = CONCOLOR( cBLACK, cWHITE );

int TextInput( int x, int y, const char *prompt, int maxlen, int fieldlen, WString &str_io, void (*handlekey)( int key, WString &str_io, int &pos ) )
{
  int res = 0;
  int insert = 1;

  WString work = str_io;

  ScrollPos scroll;
  scroll.set_min_max( 0, str_len( work ) );
  scroll.set_pagesize( fieldlen );
  scroll.set_pagestep( 0.3 );
  scroll.go( str_len( work ) );

  int show = 1;
  int firsthit = 1;
  int opage = -1;
  con_cshow();
  while(1)
    {
    if (opage != scroll.page()) show = 1;
    if (show)
      {
      WString work_out;
      str_copy( work_out, work, scroll.page(), scroll.pagesize() );
      str_pad( work_out, - scroll.pagesize() );
      work_out = L"[" + work_out + L"]"; // used to be spaces
      if ( scroll.page() > 0 ) str_set_ch( work_out, 0, L'<' );
      if ( scroll.page() + scroll.pagesize() < str_len( work_out ) ) str_set_ch( work_out, str_len(work_out)-1, L'>' );
      VString str_out = work_out;
      con_out( x, y, str_out, firsthit ? EditStrFH : EditStrBF );
      show = 0;
      opage = scroll.page();
      }
    con_xy( x + scroll.pos() - scroll.page() + 1 , y );
    
    wchar_t wch;
    wch = con_getwch();
    if( ( ! KEY_IS_WIDE_CTRL( wch ) ) && wch >= 32 && wch != KEY_BACKSPACE && str_len( work ) < maxlen - 1 )
      {
      if (firsthit)
        {
        work.undef();
        scroll.go(0);
        firsthit = 0;
        }
        if (!insert) str_del( work, scroll.pos(), 1 );
        str_ins_ch( work, scroll.pos(), wch );
        scroll.set_min_max( 0, str_len( work ) );
        scroll.go( scroll.pos() );
        scroll.down();
      show = 1;
      };
    if (firsthit)
      {
      show = 1;
      firsthit = 0;
      }

    if( wch == 27 )
      {
      res = 0;
      break;
      } else
    if( wch == 13 )
      {
      str_io = work;
      res = 1;
      break;
      } else
    if( wch == KEY_CTRL_U )
      {
      scroll.go(0);
      work.undef();
      show = 1;
      } else
    if( ( wch == KEY_BACKSPACE || wch == 8 ) && (scroll.pos() > 0) )
      {
      scroll.up();
      str_del( work, scroll.pos(), 1 );
      show = 1;
      } else
    if ( wch == KEY_WIDE(KEY_IC)    ) insert = !insert; else
    if ( wch == KEY_WIDE(KEY_LEFT)  ) scroll.up(); else
    if ( wch == KEY_WIDE(KEY_RIGHT) ) scroll.down(); else
    /*
    if ( wch == KEY_PPAGE ) scroll.ppage(); else
    if ( wch == KEY_NPAGE ) scroll.npage(); else
    */
    if ( wch == KEY_WIDE(KEY_HOME) || wch == KEY_CTRL_A ) scroll.go(0); else
    if ( wch == KEY_WIDE(KEY_END)  || wch == KEY_CTRL_E ) scroll.go(str_len(work)); else
    if ( ( wch == KEY_WIDE(KEY_DC) || wch == KEY_CTRL_D ) && scroll.pos() < str_len(work) )
      {
      str_del( work, scroll.pos(), 1 );
      show = 1;
      } else
    if ( handlekey )
      {
      int npos = scroll.pos();
      handlekey( wch, work, npos );
      scroll.set_min_max( 0, str_len( work ) );
      scroll.go( scroll.pos() );
      if (scroll.pos() != npos) scroll.go( npos );
      show = 1;
      }

    scroll.set_min_max( 0, str_len( work ) );
    scroll.go( scroll.pos() );
    }
  con_chide();
  return res;
}

// eof form_in.cpp
