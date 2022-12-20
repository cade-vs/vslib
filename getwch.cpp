/****************************************************************************
 *
 *  Copyright (c) 2022 Vladi Belperchinov-Shabanski "Cade" 
 *  http://cade.noxrun.com/  <cade@noxrun.com> <cade@bis.bg> <cade@cpan.org>
 *
 *  SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 ****************************************************************************/

#include <wchar.h>
#include <locale.h>
#include <ncursesw/curses.h>
#include "unicon.h"

int main( int argc, char** argv )
{
  setlocale( LC_ALL, "" );
    initscr();
    cbreak();
    noecho();
    raw();
    start_color();
    qiflush();
    nonl(); // `return' translation off
    WINDOW *conio_scr=newwin(0,0,0,0);
    keypad(conio_scr,TRUE); // allow function keys (keypad)
    meta(conio_scr,TRUE); // switch to 8 bit terminal return values
    // intrflush(conio_scr,FALSE); //
    idlok(conio_scr,TRUE);      // hardware line ins/del (required?)
    idcok(conio_scr,TRUE);      // hardware char ins/del (required?)
    // nodelay(conio_scr,FALSE);   // blocking getch()
    scrollok(conio_scr,TRUE);   // scroll screen if required (cursor at bottom)
  printf( "press CTRL+C to exit\n" );
  while(4)
    {
    wint_t wch;
    int i = wget_wch( conio_scr, &wch );
    if( wch == 'q' ) break;
    printf( "%x %lx\r\n", i, wch );
    }
  con_done();
}
