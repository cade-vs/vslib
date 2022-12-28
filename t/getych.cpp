/****************************************************************************
 *
 *  Copyright (c) 2022 Vladi Belperchinov-Shabanski "Cade" 
 *  http://cade.noxrun.com/  <cade@noxrun.com> <cade@bis.bg> <cade@cpan.org>
 *
 *  SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 ****************************************************************************/

#include <stdio.h>
#include <wchar.h>
#include  <locale.h>
#include "unicon.h"

int main( int argc, char** argv )
{
  setlocale( LC_ALL, "" );
  con_init();
  wprintf( L"press CTRL+C to exit\n" );
  char s[6];
  wchar_t wc[6];
  while(4)
    {
    wc[0] = 0;
    int  z = 0;
    s[z] = 0;
    int ch = con_getch();
    if( ch < 255 )
      {
      int x = -1;
           if( (ch & 0x80) == 0x00 ) x = 0;
      else if( (ch & 0xE0) == 0xC0 ) x = 1;
      else if( (ch & 0xF0) == 0xE0 ) x = 2;
      else if( (ch & 0xF8) == 0xF0 ) x = 3;

      wprintf( L">> ch = %d %x  [%x][%x][%x][%x] x = %d\r\n", ch, ch, ch & 0x80, ch & 0xE0, ch & 0xF0, ch & 0xF8, x );

      s[z++] = ch;
      s[z] = 0;
      while( x > 0 )
        {
        ch = con_getch();
        wprintf( L" --- ch = %x, x = %d, z = %d\r\n", ch, x, z );
        if( (ch & 0xC0) != 0x80 )
          {
          wprintf( L" --- bad utf8 seq %x\r\n", ch );
          break;
          }
        s[z++] = ch;
        s[z] = 0;
        x--;
        }
      int r = mbtowc( wc, s, MB_CUR_MAX );
      wprintf( L" x = %d, r = %d, z = %d, wc = %lx\r\n", x, r, z, wc[0] );
      ch = 0;
      }
    
    if(wc[0]==3)break;
    wprintf( L" got control key %x and wide char %lx [%ls]\r\n\r\n\r\n\r\n", ch, wc[0], wc );
    }
  con_done();
}


