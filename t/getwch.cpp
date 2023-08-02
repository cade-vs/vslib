/****************************************************************************
 *
 *  Copyright (c) 2022 Vladi Belperchinov-Shabanski "Cade" 
 *  http://cade.noxrun.com/  <cade@noxrun.com> <cade@bis.bg> <cade@cpan.org>
 *
 *  SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 ****************************************************************************/

#include <curses.h>
#include <wchar.h>
#include <locale.h>
#include <ncursesw/curses.h>

int main( int argc, char** argv )
{
  setlocale( LC_ALL, "" );
    initscr();
    start_color();
    qiflush();
    cbreak();
    noecho();
    nonl(); // `return' translation off
    WINDOW *conio_scr=newwin(0,0,0,0);
    keypad(conio_scr,TRUE); // allow function keys (keypad)
    meta(conio_scr,TRUE); // switch to 8 bit terminal return values
    // intrflush(conio_scr,FALSE); //
    idlok(conio_scr,TRUE);      // hardware line ins/del (required?)
    idcok(conio_scr,TRUE);      // hardware char ins/del (required?)
    // nodelay(conio_scr,FALSE);   // blocking getch()
    scrollok(conio_scr,TRUE);   // scroll screen if required (cursor at bottom)
    /* Color initialization */
  printf( "press CTRL+C to exit\n" );
  int z;
  while(4)
    {
    wint_t wch;
    int i = wget_wch( conio_scr, &wch );
    z++;
    if( wch == 3 || i == 3 ) break;
    printf( "# %d -- %x  %lx -- dec: %d  %ld\r\n", z, i, wch, i, wch );
    }
    delwin(conio_scr);
    endwin();
}
