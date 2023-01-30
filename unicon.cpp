/****************************************************************************
 *
 *  Copyright (c) 1996-2022 Vladi Belperchinov-Shabanski "Cade" 
 *  http://cade.noxrun.com/  <cade@noxrun.com> <cade@bis.bg> <cade@cpan.org>
 *
 * SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 ****************************************************************************/

#include "unicon.h"

/****************************************************************************
**
** UNIX Part
**
****************************************************************************/

#ifdef _TARGET_HAVE_CURSES

/****************************************************************************
** This part is loosely based on `linconio':
** ---------------------------------------------------------------------
** File: conio.h     Date: 03/09/1997       Version: 1.02
** CONIO.H an implementation of the conio.h for Linux based on ncurses.
** This is copyright (c) 1996,97 by Fractor / Mental EXPlosion.
** If you want to copy it you must do this following the terms of the
** GNU Library Public License
** Please read the file "README" before using this library.
****************************************************************************/

  int __fg;
  int __bg;
  int __ta;
  WINDOW *conio_scr;

  #define CON_PAIR(f,b) (((b)*8)+(f)+1)

  /* Some internals... */
  int colortab(int a) /* convert UNIX/Curses Color code to DOS-standard */
  {
     switch(a) {
        case cBLACK   : return COLOR_BLACK;
        case cBLUE    : return COLOR_BLUE;
        case cGREEN   : return COLOR_GREEN;
        case cCYAN    : return COLOR_CYAN;
        case cRED     : return COLOR_RED;
        case cMAGENTA : return COLOR_MAGENTA;
        case cYELLOW  : return COLOR_YELLOW;
        case cWHITE   : return COLOR_WHITE;
     }
     return 0;
  }

  int con_init()
  {
    initscr();
    start_color();
    qiflush();
    cbreak();
    if ( !getenv("UNICON_NO_RAW") )
      {
      /* To allow curses app to handle properly ctrl+z, ctrl+c, etc.
         I should not call raw() here, anyway it is known that this
         will cause misinterpretation of some keys (arrows) after
         resuming (SUSP -- ctrl+z)...
         So, unless UNICON_NO_RAW exported and set to any value raw()
         is called as usual.
      */
      raw();
      }
    noecho();
    nonl(); // `return' translation off
    if (!has_colors())
       fprintf(stderr,"Attention: A color terminal may be required to run this application !\n");
    conio_scr=newwin(0,0,0,0);
    keypad(conio_scr,TRUE); // allow function keys (keypad)
    meta(conio_scr,TRUE); // switch to 8 bit terminal return values
    // intrflush(conio_scr,FALSE); //
    idlok(conio_scr,TRUE);      // hardware line ins/del (required?)
    idcok(conio_scr,TRUE);      // hardware char ins/del (required?)
    // nodelay(conio_scr,FALSE);   // blocking getch()
    scrollok(conio_scr,TRUE);   // scroll screen if required (cursor at bottom)
    /* Color initialization */
    for ( __bg=0; __bg<8; __bg++ )
       for ( __fg=0; __fg<8; __fg++ )
          init_pair( CON_PAIR(__fg,__bg), colortab(__fg), colortab(__bg));
    con_ta(7);
    return 0;
  }

  void con_done()
  {
    delwin(conio_scr);
    endwin();
  }

  void con_suspend()
  {
    con_done();
  }

  void con_restore()
  {
    con_init();
  }

  void con_reset_screen_size()
  {
    //ungetch( KEY_RESIZE );
    //nodelay(conio_scr,TRUE);   // non-blocking getch()
  }

  void con_ta( int attr )
  {
    __ta = attr;
    wattrset(conio_scr,0); /* (???) My curses-version needs this ... */
    __fg = COLORFG(attr);
    __bg = COLORBG(attr);
    wattrset(conio_scr,COLOR_PAIR(CON_PAIR( __fg%8, __bg%8 )) | ( __bg > 7 )*(A_BLINK) | ( __fg > 7 )*(A_BOLD) );
    wbkgdset( conio_scr, COLOR_PAIR(CON_PAIR( __fg%8, __bg%8 )) );
  }

  void con_ce( int attr )
  {
    if (attr != -1)
      {
      int ta = __ta;
      con_ta( attr );
      wclrtoeol(conio_scr);
      wrefresh(conio_scr);
      con_ta( ta );
      }
    else
      {
      wclrtoeol(conio_scr);
      wrefresh(conio_scr);
      }
  }

  void con_cs( int attr )
  {
    if (attr != -1)
      {
      int ta = __ta;
      con_ta( attr );
      wclear(conio_scr);
      wmove(conio_scr,0,0);
      wrefresh(conio_scr);
      con_ta( ta );
      }
    else
      {
      wclear(conio_scr);
      wmove(conio_scr,0,0);
      wrefresh(conio_scr);
      }
  }

  void con_puts( const char *s )
  {
    waddstr(conio_scr,s);
    wrefresh(conio_scr);
  }

  int con_max_x()
  {
    return getmaxx(conio_scr);
  }

  int con_max_y()
  {
    return getmaxy(conio_scr);
  }

  int con_x()
  {
    return getcurx(conio_scr)+1;
  }

  int con_y()
  {
    return getcury(conio_scr)+1;
  }

  void con_fg( int color )
  {
    __fg=color;
    con_ta( CONCOLOR( __fg, __bg ) );
  }

  void con_bg( int color )
  {
    __bg=color;
    con_ta( CONCOLOR( __fg, __bg ) );
  }


  void con_xy( int x, int y )
  {
    wmove(conio_scr,y-1,x-1);
    wrefresh(conio_scr);
  }

  void con_chide()
  {
    con_xy( 1, 1 );
    leaveok(conio_scr,TRUE);
    curs_set( 0 );
  }

  void con_cshow()
  {
    leaveok(conio_scr,FALSE);
    curs_set( 1 );
  }

  int con_kbhit()
  {
    int i;
    nodelay(conio_scr,TRUE);
    i=wgetch(conio_scr);
    nodelay(conio_scr,FALSE);
    if (i==-1)
      return 0;
    else
      ungetch(i);
    return(i);
  }

  int con_getch()
  {
    int i = wgetch(conio_scr);
    if( i == -1 ) i = 0;
    if( i == 27 )
      if( con_kbhit() )
        i = UKEY_ALT_PREFIX + wgetch(conio_scr);
    return( i > 0xFF ? UKEY_WIDE(i) : i );
  }

  void con_beep()
  {
    printf( "\007" );
    fflush( stdout );
  }

  wchar_t con_getwch()
  {
    wchar_t wch = 0;
    char s[6];
    int  z = 0;
    s[z] = 0;
    int ch = con_getch();
    if( UKEY_IS_WIDE_CTRL( ch ) ) return ch; // control key

    int x = -1;
         if( (ch & 0x80) == 0x00 ) x = 0;
    else if( (ch & 0xE0) == 0xC0 ) x = 1;
    else if( (ch & 0xF0) == 0xE0 ) x = 2;
    else if( (ch & 0xF8) == 0xF0 ) x = 3;

    s[z++] = ch;
    s[z] = 0;
    while( x > 0 )
      {
      ch = con_getch();
      if( (ch & 0xC0) != 0x80 ) return -1;
      s[z++] = ch;
      s[z] = 0;
      x--;
      }
    if( mbtowc( &wch, s, MB_CUR_MAX ) )
    {
    }
    return wch;
  }

#endif /* _TARGET_HAVE_CURSES */

// target have yascreen curses replacement
#ifdef _TARGET_HAVE_YASCREEN
#include <stdio.h>
#include <signal.h>

  int __fg;
  int __bg;
  int __ta;
  int __x=0;
  int __y=0;
  uint32_t __attr=0;
  yascreen *ya_s=NULL;

  /* Some internals... */
  int colortab(int a) /* convert UNIX/Curses Color code to DOS-standard */
  {
     switch(a) {
        case cBLACK   : return YAS_BLACK;
        case cBLUE    : return YAS_BLUE;
        case cGREEN   : return YAS_GREEN;
        case cCYAN    : return YAS_CYAN;
        case cRED     : return YAS_RED;
        case cMAGENTA : return YAS_MAGENTA;
        case cYELLOW  : return YAS_YELLOW;
        case cWHITE   : return YAS_WHITE;
     }
     return 0;
  }

  void con_yas_sigwinch( int sig __attribute__((unused)) )
  {
    signal( SIGWINCH, con_yas_sigwinch ); // (re)setup signal handler
    con_reset_screen_size();
  }

  int con_init()
  {
    signal( SIGWINCH, con_yas_sigwinch ); // (re)setup signal handler
    if (ya_s)
    {
      yascreen_term_set(ya_s,YAS_NOBUFF|YAS_NOSIGN|YAS_NOECHO);
      if (-1==yascreen_resize(ya_s,0,0))
        yascreen_resize(ya_s,80,25);
      yascreen_altbuf(ya_s,1);
      yascreen_cursor(ya_s,0);
      con_ta(7);
      yascreen_redraw(ya_s);
      return 0;
    }
    ya_s=yascreen_init(0,0);
    if (!ya_s)
      ya_s=yascreen_init(80,25);
    if (!ya_s)
    return 1;
    yascreen_term_set(ya_s,YAS_NOBUFF|YAS_NOSIGN|YAS_NOECHO);
    yascreen_altbuf(ya_s,1);
    yascreen_cursor(ya_s,0);
    con_ta(7);
    return 0;
  }

  void con_done()
  {
    yascreen_clear(ya_s);
    yascreen_altbuf(ya_s,0);
    yascreen_cursor(ya_s,1);
    yascreen_term_restore(ya_s);
    yascreen_free(ya_s);
    ya_s=NULL;
  }

  void con_suspend()
  {
    con_done();
  }

  void con_restore()
  {
    con_init();
    yascreen_update(ya_s);
  }

  void con_reset_screen_size()
  {
    yascreen_pushch(ya_s,YAS_SCREEN_SIZE);
  }

  void con_ta( int attr )
  {
    __ta = attr;
    __fg = COLORFG(attr);
    __bg = COLORBG(attr);
    __attr = YAS_FGCOLOR(colortab(__fg%8))|YAS_BGCOLOR(colortab(__bg%8))|((__bg>7)*YAS_ITALIC)|((__fg>7)*YAS_BOLD);
  }

  void con_ce( int attr )
  {
    if (attr != -1)
      {
      int ta = __ta;
      con_ta( attr );
      yascreen_printxyu(ya_s,__x,__y,__attr,"%*s",yascreen_sx(ya_s),"");
      con_ta( ta );
      }
    else
      {
      yascreen_printxyu(ya_s,__x,__y,__attr,"%*s",yascreen_sx(ya_s),"");
      }
  }

  void con_cs( int attr )
  {
    if (attr != -1)
      {
      int ta = __ta;
      con_ta( attr );
      yascreen_clear_mem(ya_s,__attr);
      __x=__y=0;
      yascreen_update(ya_s);
      con_ta( ta );
      }
    else
      {
      yascreen_clear_mem(ya_s,__attr);
      __x=__y=0;
      yascreen_update(ya_s);
      }
  }

  void con_puts( const char *s )
  {
    yascreen_putsxyu(ya_s,__x,__y,__attr,s);
    __x=yascreen_x(ya_s);
    __y=yascreen_y(ya_s);
  }

  int con_max_x()
  {
    return yascreen_sx(ya_s);
  }

  int con_max_y()
  {
    return yascreen_sy(ya_s);
  }

  int con_x()
  {
    return(__x+1);
  }

  int con_y()
  {
    return(__y+1);
  }

  void con_fg( int color )
  {
    __fg=color;
    con_ta( CONCOLOR( __fg, __bg ) );
  }

  void con_bg( int color )
  {
    __bg=color;
    con_ta( CONCOLOR( __fg, __bg ) );
  }


  void con_xy( int x, int y )
  {
    __x=x-1;
    __y=y-1;
    yascreen_cursor_xy(ya_s,__x,__y);
  }

  void con_chide()
  {
    con_xy( 1, 1 );
    yascreen_cursor(ya_s,0);
  }

  void con_cshow()
  {
    yascreen_cursor(ya_s,1);
  }

  int con_kbhit()
  {
    int i = yascreen_peekch(ya_s);
    if( i == YAS_K_NONE ) i = 0;
    return(i);
  }

  int con_getch()
  {
    int i = yascreen_getch( ya_s );
    if( i == YAS_K_NONE ) i = 0;
    return( i );
  }

  void con_beep()
  {
    printf( "\007" );
    fflush( stdout );
  }

  wchar_t con_getwch()
  {
    wchar_t w = yascreen_getwch( ya_s );
    if( w == YAS_K_NONE ) w = 0;
    return( w );
  }

#endif

/****************************************************************************
**
** COMMON Part
**
****************************************************************************/


  void con_out( int x, int y, const char *s )
  {
    con_out( x, y, s, __ta );
  }

  void con_out( int x, int y, const char *s, int attr )
  {
    int ta = __ta;
    con_ta( attr );
    con_xy( x, y );
    con_puts( s );
    con_ta( ta );
  }

  void con_puts( const char *s, int attr )
  {
    int ta = __ta;
    con_ta( attr );
    con_puts( s );
    con_ta( ta );
  }

/****************************************************************************
** EOF
****************************************************************************/

