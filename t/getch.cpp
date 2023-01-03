/****************************************************************************
 *
 *  Copyright (c) 2022 Vladi Belperchinov-Shabanski "Cade" 
 *  http://cade.noxrun.com/  <cade@noxrun.com> <cade@bis.bg> <cade@cpan.org>
 *
 *  SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 ****************************************************************************/

#include  <locale.h>
#include "unicon.h"

int main( int argc, char** argv )
{
  setlocale( LC_ALL, "" );
  con_init();
  printf( "press CTRL+C to exit\n" );
  while(4)
    {
    int ch = con_getch();
    if( ch == UKEY_CTRL_C )break;
    printf( "%d %x\r\n", ch, ch );
    }
  con_done();
}
