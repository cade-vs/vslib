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
  wprintf( L"press CTRL+C to exit\r\n" );
  wchar_t wch;
  while(4)
    {
    wchar_t wch = con_getwch();
    if( wch == 3 ) break;
    wprintf( L"%lx\r\n", wch );
    }
  con_done();
}


