/*
 *
 * (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-1999
 *
 * SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 * $Id: form_in.cpp,v 1.2 2001/10/28 13:53:02 cade Exp $
 *
 */

#include "form_in.h"
#include "scroll.h"

BSet FI_LETTERS  = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
BSet FI_DIGITS   = "0123456789";
BSet FI_ALPHANUM = FI_LETTERS & FI_DIGITS;
BSet FI_REALS    = FI_DIGITS & "Ee+-.";
BSet FI_USERS    = "";
BSet FI_MASKS    = "A#$U"; // A-any, #-int, $-real, U-users

int EditStrBF = CONCOLOR( chWHITE, cBLUE );
int EditStrFH = CONCOLOR( cBLACK, cWHITE );

int TextInput( int x, int y, const char *prompt, int maxlen, int fieldlen, String *strres, void (*handlekey)( int key, String &s, int &pos ) )
{
  int res = 0;
  int insert = 1;
  String str = *strres;
  String tmp;
  int ch;

  TScrollPos scroll;
  scroll.type = 1;
  scroll.min = 0;
  scroll.max = maxlen;
  scroll.pagesize = fieldlen;
  scroll.gotopos( str_len(str) );
  
  int show = 1;
  int firsthit = 1;
  int opage = -1;
  con_cshow();
  while(1)
    {
    if (opage != scroll.page) show = 1;
    if (show)
      {
      str_copy( tmp, str, scroll.page, scroll.pagesize );
      str_pad( tmp, -scroll.pagesize );
      tmp = " " + tmp + " ";
      if ( scroll.page > 0 ) str_set_ch( tmp, 0, '<' );
      if ( scroll.page+scroll.pagesize < str_len(str) ) str_set_ch( tmp, str_len(tmp)-1, '>' );
      con_out(x, y, tmp, firsthit ? EditStrFH : EditStrBF );
      show = 0;
      opage = scroll.page;
      }
    con_xy( x + scroll.pos - scroll.page + 1 , y );
    ch = con_getch();
    if(ch > 31 && ch < 129 && str_len(str) < 70)
      {
      if (firsthit)
        {
        str = "";
        scroll.gotopos(0);
        firsthit = 0;
        }
        if (!insert) str_del( str, scroll.pos, 1 );
        str_ins_ch( str, scroll.pos, ch );
        scroll.down();
      show = 1;
      };
    if (firsthit)
      {
      show = 1;
      firsthit = 0;
      }
    if( ch == 27 )
      {
      res = 0;
      break;
      } else
    if( ch == 13 )
      {
      *strres = str;
      res = 1;
      break;
      } else
    if( (ch == KEY_BACKSPACE || ch == 8) && (scroll.pos > 0) )
      {
      scroll.up();
      str_del( str, scroll.pos, 1 );
      show = 1;
      } else
    if ( ch == KEY_IC  ) insert = !insert; else
    if ( ch == KEY_LEFT  ) scroll.up(); else
    if ( ch == KEY_RIGHT && scroll.pos < str_len(str) ) scroll.down(); else
    if ( ch == KEY_HOME ) scroll.gotopos(0); else
    if ( ch == KEY_END  ) scroll.gotopos(str_len(str)); else
    if ( ch == KEY_DC  && scroll.pos < str_len(str) )
      {
      str_del( str, scroll.pos, 1 );
      show = 1;
      } else
    if ( handlekey )
      {
      int npos = scroll.pos;
      handlekey( ch, str, npos );
      if (scroll.pos != npos) scroll.gotopos( npos );
      show = 1;
      }
    }
  con_chide();
  return res;
}

int TextInput( int x, int y, const char *prompt, int maxlen, int fieldlen, char *strres, void (*handlekey)( int key, String &s, int &pos ) )
{
  String str = strres;
  int res = TextInput( x, y, prompt, maxlen, fieldlen, &str, handlekey );
  strcpy( strres, str.data() );
  return res;
}

/*
int InputStr( int x, int y, const char *prompt, int maxlen, int fieldlen, String *strres )
{
int res = 0;
String str;
String tmp;

ConOut( x, y, prompt, CONCOLOR(EditStrFG, EditStrBG) );
str = *strres;
int pos = str.GetLength();
int ch;

int sx = x + strlen( prompt );
int sy = y;

int show = 2;

ConCShow();
// ConOut(sx, sy, str.Right(fieldlen), EditStrFG, EditStrBG);
int firsthit = 1;
while(1)
  {
  if (show)
    {
    int page = pos / fieldlen;
//    if (page == 0) page = fieldlen / 2;
    if (show == 2)
      {
      tmp = " ";
      int z = fieldlen - (str.len() - page);
      tmp *= z;
      tmp = " " + str.Copy( page, fieldlen ) + tmp + " ";
      if ( page > 0 ) tmp.SetCh( 0, '<' );
      if ( str.len() - page > fieldlen ) tmp.SetCh( fieldlen+1, '>' );
      ConOut( sx, sy, tmp, CONCOLOR(EditStrFG, EditStrBG) );
      }
    ConXY( sx+pos-page+1, sy );
    show = 0;
    }

  ch = ConGetch();
  if(ch > 31 && ch < 129 && str.GetLength() < 70)
    {
    if (firsthit)
      {
      str = "";
      pos = 0;
      firsthit = 0;
      }
      str.InsertCh( pos, ch );
      pos++;
    show = 2;
    };
  firsthit = 0;
  if( ch == 27 )
    {
    res = 0;
    break;
    } else
  if( ch == 13 )
    {
    *strres = str;
    res = 1;
    break;
    } else
  if( (ch == KEY_BACKSPACE || ch == 8) && (pos > 0) )    
    {
    pos--;
    str.Delete( pos, 1 );
    show = 2;
    } else
  if( ch == KEY_LEFT  ) // <- 
    {
    if (pos > 0)
      {
      pos--;
      show = 1;
      }
    } else
  if( ch == KEY_RIGHT ) // -> 
    {
    if ( pos < str.GetLength() )
      {
      pos++;
      show = 1;
      }
    } else
  if ( ch == KEY_HOME ) { pos = 0; show = 1; } else
  if ( ch == KEY_END  ) { pos = str.GetLength(); show = 1; } else
  if ( ch == KEY_DC  && pos < str.GetLength() )
    {
    str.Delete( pos, 1 );
    show = 2;
    }
  }
ConCHide();
return res;
}
*/

// eof form_in.cpp
