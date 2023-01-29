/****************************************************************************
 *
 *  Copyright (c) 1996-2022 Vladi Belperchinov-Shabanski "Cade" 
 *  http://cade.noxrun.com/  <cade@noxrun.com> <cade@bis.bg> <cade@cpan.org>
 *
 *  SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 ****************************************************************************/

#ifndef _UNICON_H_
#define _UNICON_H_

#ifdef HAVE_CONFIG_H
  #include "config.h"
#endif

#include "target.h"

#ifdef _TARGET_UNIX_
  #ifdef _TARGET_HAVE_YASCREEN
    #define inline
    #include <yascreen.h>
    #undef inline
  #else
    #if HAVE_CONFIG_H
      // This will find the curses header on most systems. If it isn't found,
      // the user will receive an error and compilation will terminate.
      // These are defined when ../configure is run, and suggested use is
      // mentioned in the autoconf documentation
      // <https://www.gnu.org/software/autoconf-archive/ax_with_curses.html>
      #if defined HAVE_NCURSESW_CURSES_H
      #  include <ncursesw/curses.h>
      #elif defined HAVE_NCURSESW_H
      #  include <ncursesw.h>
      #elif defined HAVE_NCURSES_CURSES_H
      #  include <ncurses/curses.h>
      #elif defined HAVE_NCURSES_H
      #  include <ncurses.h>
      #elif defined HAVE_CURSES_H
      #  include <curses.h>
      #else
      #  error "SysV or X/Open-compatible Curses header file required"
      #endif
    #elif defined(_TARGET_LINUX_)
    #include <curses.h>
    #elif defined(_TARGET_NETBSD_)
    #include <ncurses.h>
    #else
    #include <curses.h>
    #endif
  #endif
  #include <stdlib.h>
#endif

/****************************************************************************
**
** COLOR defines
**
****************************************************************************/

  #define CONCOLOR(f,b) (b*16+f)
  #define COLORFG(t)    (t % 16)
  #define COLORBG(t)    (t / 16)

  /***** std-colors/normal ******/

  #define cNORMAL    7
  #define cBOLD      8
  #define cREVERSED  CONCOLOR(cBLACK,cWHITE)

  /***** low-colors/normal ******/

  #define cBLACK     0
  #define cBLUE      1
  #define cGREEN     2
  #define cCYAN      3
  #define cRED       4
  #define cMAGENTA   5
  #define cBROWN     6
  #define cYELLOW    6
  #define cLGRAY     7

  /***** hi-colors/bright *******/

  #define chBLACK    7
  #define cWHITE     7
  #define cDGRAY     8
  #define chBLUE     9
  #define chGREEN   10
  #define chCYAN    11
  #define chRED     12
  #define chMAGENTA 13
  #define chYELLOW  14
  #define chWHITE   15

/****************************************************************************
**
** KEY defines
**
****************************************************************************/

/******* common ************************************************************/
                                    
  #define UKEY_WIDE_CTRL_PREFIX     0xF0000
  #define UKEY_WIDE_CTRL_PREFIX_END 0xFFFFD
  #define UKEY_WIDE(k)              (UKEY_WIDE_CTRL_PREFIX+(k))
  #define UKEY_IS_WIDE_CTRL(k)      (((k)>=UKEY_WIDE_CTRL_PREFIX)&&((k)<=UKEY_WIDE_CTRL_PREFIX_END))

  #define UKEY_CTRL_A    1
  #define UKEY_CTRL_B    2
  #define UKEY_CTRL_C    3
  #define UKEY_CTRL_D    4
  #define UKEY_CTRL_E    5
  #define UKEY_CTRL_F    6
  #define UKEY_CTRL_G    7
  #define UKEY_CTRL_H    8
  #define UKEY_CTRL_I    9
  #define UKEY_CTRL_J   10
  #define UKEY_CTRL_K   11
  #define UKEY_CTRL_L   12
  #define UKEY_CTRL_M   13
  #define UKEY_CTRL_N   14
  #define UKEY_CTRL_O   15
  #define UKEY_CTRL_P   16
  #define UKEY_CTRL_Q   17
  #define UKEY_CTRL_R   18
  #define UKEY_CTRL_S   19
  #define UKEY_CTRL_T   20
  #define UKEY_CTRL_U   21
  #define UKEY_CTRL_V   22
  #define UKEY_CTRL_W   23
  #define UKEY_CTRL_X   24
  #define UKEY_CTRL_Y   25
  #define UKEY_CTRL_Z   26

  #define UKEY_TAB       9
  #define UKEY_ENTER    13
  #define UKEY_ESC      27

/******* DJGPP/DOS dropped on 2023.JAN.03 *******************************/

#ifdef _TARGET_GO32_

  #error DJGPP/GO32/DOS Suppot was dropped on 2023.JAN.03
  #error use older version or contact me at <cade@noxrun.com>

/******* UNIX/NCURSES ******************************************************/

#elif defined(_TARGET_HAVE_CURSES)

  #define UKEY_INS       UKEY_WIDE(KEY_IC)
  #define UKEY_BACKSPACE UKEY_WIDE(KEY_BACKSPACE)
  #define UKEY_LEFT      UKEY_WIDE(KEY_LEFT)
  #define UKEY_RIGHT     UKEY_WIDE(KEY_RIGHT)
  #define UKEY_UP        UKEY_WIDE(KEY_UP)
  #define UKEY_DOWN      UKEY_WIDE(KEY_DOWN)
  #define UKEY_HOME      UKEY_WIDE(KEY_HOME)
  #define UKEY_END       UKEY_WIDE(KEY_END)
  #define UKEY_DEL       UKEY_WIDE(KEY_DC)
  #define UKEY_PGUP      UKEY_WIDE(KEY_PPAGE)
  #define UKEY_PGDN      UKEY_WIDE(KEY_NPAGE)

  #define UKEY_ALT_PREFIX    1000
  #define UKEY_ALT_A     UKEY_WIDE(UKEY_ALT_PREFIX + 'a')
  #define UKEY_ALT_B     UKEY_WIDE(UKEY_ALT_PREFIX + 'b')
  #define UKEY_ALT_C     UKEY_WIDE(UKEY_ALT_PREFIX + 'c')
  #define UKEY_ALT_D     UKEY_WIDE(UKEY_ALT_PREFIX + 'd')
  #define UKEY_ALT_E     UKEY_WIDE(UKEY_ALT_PREFIX + 'e')
  #define UKEY_ALT_F     UKEY_WIDE(UKEY_ALT_PREFIX + 'f')
  #define UKEY_ALT_G     UKEY_WIDE(UKEY_ALT_PREFIX + 'g')
  #define UKEY_ALT_H     UKEY_WIDE(UKEY_ALT_PREFIX + 'h')
  #define UKEY_ALT_I     UKEY_WIDE(UKEY_ALT_PREFIX + 'i')
  #define UKEY_ALT_J     UKEY_WIDE(UKEY_ALT_PREFIX + 'j')
  #define UKEY_ALT_K     UKEY_WIDE(UKEY_ALT_PREFIX + 'k')
  #define UKEY_ALT_L     UKEY_WIDE(UKEY_ALT_PREFIX + 'l')
  #define UKEY_ALT_M     UKEY_WIDE(UKEY_ALT_PREFIX + 'm')
  #define UKEY_ALT_N     UKEY_WIDE(UKEY_ALT_PREFIX + 'n')
  #define UKEY_ALT_O     UKEY_WIDE(UKEY_ALT_PREFIX + 'o')
  #define UKEY_ALT_P     UKEY_WIDE(UKEY_ALT_PREFIX + 'p')
  #define UKEY_ALT_Q     UKEY_WIDE(UKEY_ALT_PREFIX + 'q')
  #define UKEY_ALT_R     UKEY_WIDE(UKEY_ALT_PREFIX + 'r')
  #define UKEY_ALT_S     UKEY_WIDE(UKEY_ALT_PREFIX + 's')
  #define UKEY_ALT_T     UKEY_WIDE(UKEY_ALT_PREFIX + 't')
  #define UKEY_ALT_U     UKEY_WIDE(UKEY_ALT_PREFIX + 'u')
  #define UKEY_ALT_V     UKEY_WIDE(UKEY_ALT_PREFIX + 'v')
  #define UKEY_ALT_W     UKEY_WIDE(UKEY_ALT_PREFIX + 'w')
  #define UKEY_ALT_X     UKEY_WIDE(UKEY_ALT_PREFIX + 'x')
  #define UKEY_ALT_Y     UKEY_WIDE(UKEY_ALT_PREFIX + 'y')
  #define UKEY_ALT_Z     UKEY_WIDE(UKEY_ALT_PREFIX + 'z')
  #define UKEY_ALT_1     UKEY_WIDE(UKEY_ALT_PREFIX + '1')
  #define UKEY_ALT_2     UKEY_WIDE(UKEY_ALT_PREFIX + '2')
  #define UKEY_ALT_3     UKEY_WIDE(UKEY_ALT_PREFIX + '3')
  #define UKEY_ALT_4     UKEY_WIDE(UKEY_ALT_PREFIX + '4')
  #define UKEY_ALT_5     UKEY_WIDE(UKEY_ALT_PREFIX + '5')
  #define UKEY_ALT_6     UKEY_WIDE(UKEY_ALT_PREFIX + '6')
  #define UKEY_ALT_7     UKEY_WIDE(UKEY_ALT_PREFIX + '7')
  #define UKEY_ALT_8     UKEY_WIDE(UKEY_ALT_PREFIX + '8')
  #define UKEY_ALT_9     UKEY_WIDE(UKEY_ALT_PREFIX + '9')
  #define UKEY_ALT_0     UKEY_WIDE(UKEY_ALT_PREFIX + '0')
  #define UKEY_ALT_MINUS UKEY_WIDE(UKEY_ALT_PREFIX + '-')
  #define UKEY_ALT_EQ    UKEY_WIDE(UKEY_ALT_PREFIX + '=')
  #define UKEY_ALT_BACKSPACE UKEY_WIDE(UKEY_ALT_PREFIX + KEY_BACKSPACE )

  #define UKEY_F1        UKEY_WIDE(KEY_F(0) + 1)
  #define UKEY_F2        UKEY_WIDE(KEY_F(0) + 2)
  #define UKEY_F3        UKEY_WIDE(KEY_F(0) + 3)
  #define UKEY_F4        UKEY_WIDE(KEY_F(0) + 4)
  #define UKEY_F5        UKEY_WIDE(KEY_F(0) + 5)
  #define UKEY_F6        UKEY_WIDE(KEY_F(0) + 6)
  #define UKEY_F7        UKEY_WIDE(KEY_F(0) + 7)
  #define UKEY_F8        UKEY_WIDE(KEY_F(0) + 8)
  #define UKEY_F9        UKEY_WIDE(KEY_F(0) + 9)
  #define UKEY_F10       UKEY_WIDE(KEY_F(0) + 10)

  #define UKEY_SH_F1     UKEY_WIDE(KEY_F(0) + 11)
  #define UKEY_SH_F2     UKEY_WIDE(KEY_F(0) + 12)
  #define UKEY_SH_F3     UKEY_WIDE(KEY_F(0) + 13)
  #define UKEY_SH_F4     UKEY_WIDE(KEY_F(0) + 14)
  #define UKEY_SH_F5     UKEY_WIDE(KEY_F(0) + 15)
  #define UKEY_SH_F6     UKEY_WIDE(KEY_F(0) + 16)
  #define UKEY_SH_F7     UKEY_WIDE(KEY_F(0) + 17)
  #define UKEY_SH_F8     UKEY_WIDE(KEY_F(0) + 18)
  #define UKEY_SH_F9     UKEY_WIDE(KEY_F(0) + 19)
  #define UKEY_SH_F10    UKEY_WIDE(KEY_F(0) + 20)
  #define UKEY_SH_F11    UKEY_WIDE(KEY_F(0) + 21)
  #define UKEY_SH_F12    UKEY_WIDE(KEY_F(0) + 22)

  #define UKEY_CTRL_F1   UKEY_WIDE(KEY_F(0) + 23)
  #define UKEY_CTRL_F2   UKEY_WIDE(KEY_F(0) + 24)
  #define UKEY_CTRL_F3   UKEY_WIDE(KEY_F(0) + 25)
  #define UKEY_CTRL_F4   UKEY_WIDE(KEY_F(0) + 26)
  #define UKEY_CTRL_F5   UKEY_WIDE(KEY_F(0) + 27)
  #define UKEY_CTRL_F6   UKEY_WIDE(KEY_F(0) + 28)
  #define UKEY_CTRL_F7   UKEY_WIDE(KEY_F(0) + 29)
  #define UKEY_CTRL_F8   UKEY_WIDE(KEY_F(0) + 30)
  #define UKEY_CTRL_F9   UKEY_WIDE(KEY_F(0) + 31)
  #define UKEY_CTRL_F10  UKEY_WIDE(KEY_F(0) + 32)
  #define UKEY_CTRL_F11  UKEY_WIDE(KEY_F(0) + 33)
  #define UKEY_CTRL_F12  UKEY_WIDE(KEY_F(0) + 34)
                                      
  #define UKEY_ALT_F1    0
  #define UKEY_ALT_F2    0
  #define UKEY_ALT_F3    0
  #define UKEY_ALT_F4    0
  #define UKEY_ALT_F5    0
  #define UKEY_ALT_F6    0
  #define UKEY_ALT_F7    0
  #define UKEY_ALT_F8    0
  #define UKEY_ALT_F9    0
  #define UKEY_ALT_F10   0

  #define UKEY_RESIZE   KEY_RESIZE

#elif defined(_TARGET_HAVE_YASCREEN)

  #define UKEY_INS       YAS_K_INS
  #define UKEY_BACKSPACE YAS_K_BSP
  #define UKEY_LEFT      YAS_K_LEFT
  #define UKEY_RIGHT     YAS_K_RIGHT
  #define UKEY_UP        YAS_K_UP
  #define UKEY_DOWN      YAS_K_DOWN
  #define UKEY_HOME      YAS_K_HOME
  #define UKEY_END       YAS_K_END
  #define UKEY_DEL       YAS_K_DEL
  #define UKEY_PGUP      YAS_K_PGUP
  #define UKEY_PGDN      YAS_K_PGDN

  #define UKEY_ALT_A YAS_K_ALT('a')
  #define UKEY_ALT_B YAS_K_ALT('b')
  #define UKEY_ALT_C YAS_K_ALT('c')
  #define UKEY_ALT_D YAS_K_ALT('d')
  #define UKEY_ALT_E YAS_K_ALT('e')
  #define UKEY_ALT_F YAS_K_ALT('f')
  #define UKEY_ALT_G YAS_K_ALT('g')
  #define UKEY_ALT_H YAS_K_ALT('h')
  #define UKEY_ALT_I YAS_K_ALT('i')
  #define UKEY_ALT_J YAS_K_ALT('j')
  #define UKEY_ALT_K YAS_K_ALT('k')
  #define UKEY_ALT_L YAS_K_ALT('l')
  #define UKEY_ALT_M YAS_K_ALT('m')
  #define UKEY_ALT_N YAS_K_ALT('n')
  #define UKEY_ALT_R YAS_K_ALT('r')
  #define UKEY_ALT_S YAS_K_ALT('s')
  #define UKEY_ALT_X YAS_K_ALT('x')
  #define UKEY_ALT_Z YAS_K_ALT('z')
  #define UKEY_ALT_0 YAS_K_ALT('0')
  #define UKEY_ALT_1 YAS_K_ALT('1')
  #define UKEY_ALT_2 YAS_K_ALT('2')
  #define UKEY_ALT_3 YAS_K_ALT('3')
  #define UKEY_ALT_4 YAS_K_ALT('4')
  #define UKEY_ALT_5 YAS_K_ALT('5')
  #define UKEY_ALT_6 YAS_K_ALT('6')
  #define UKEY_ALT_7 YAS_K_ALT('7')
  #define UKEY_ALT_8 YAS_K_ALT('8')
  #define UKEY_ALT_9 YAS_K_ALT('9')
  #define UKEY_ALT_EQ YAS_K_ALT('=')
  #define UKEY_ALT_MINUS YAS_K_ALT('-')
  #define UKEY_ALT_BACKSPACE YAS_K_ALT( UKEY_BACKSPACE )

  #define UKEY_F1        YAS_K_F1
  #define UKEY_F2        YAS_K_F2
  #define UKEY_F3        YAS_K_F3
  #define UKEY_F4        YAS_K_F4
  #define UKEY_F5        YAS_K_F5
  #define UKEY_F6        YAS_K_F6
  #define UKEY_F7        YAS_K_F7
  #define UKEY_F8        YAS_K_F8
  #define UKEY_F9        YAS_K_F9
  #define UKEY_F10       YAS_K_F10

  #define UKEY_SH_F1     YAS_K_S_F1
  #define UKEY_SH_F2     YAS_K_S_F2
  #define UKEY_SH_F3     YAS_K_S_F3
  #define UKEY_SH_F4     YAS_K_S_F4
  #define UKEY_SH_F5     YAS_K_S_F5
  #define UKEY_SH_F6     YAS_K_S_F6
  #define UKEY_SH_F7     YAS_K_S_F7
  #define UKEY_SH_F8     YAS_K_S_F8
  #define UKEY_SH_F9     YAS_K_S_F9
  #define UKEY_SH_F10    YAS_K_S_F10

  #define UKEY_CTRL_F1   YAS_K_C_F1
  #define UKEY_CTRL_F2   YAS_K_C_F2
  #define UKEY_CTRL_F3   YAS_K_C_F3
  #define UKEY_CTRL_F4   YAS_K_C_F4
  #define UKEY_CTRL_F5   YAS_K_C_F5
  #define UKEY_CTRL_F6   YAS_K_C_F6
  #define UKEY_CTRL_F7   YAS_K_C_F7
  #define UKEY_CTRL_F8   YAS_K_C_F8
  #define UKEY_CTRL_F9   YAS_K_C_F9
  #define UKEY_CTRL_F10  YAS_K_C_F10

  #define UKEY_ALT_F1    YAS_K_A_F1
  #define UKEY_ALT_F2    YAS_K_A_F2
  #define UKEY_ALT_F3    YAS_K_A_F3
  #define UKEY_ALT_F4    YAS_K_A_F4
  #define UKEY_ALT_F5    YAS_K_A_F5
  #define UKEY_ALT_F6    YAS_K_A_F6
  #define UKEY_ALT_F7    YAS_K_A_F7
  #define UKEY_ALT_F8    YAS_K_A_F8
  #define UKEY_ALT_F9    YAS_K_A_F9
  #define UKEY_ALT_F10   YAS_K_A_F10

  #define UKEY_RESIZE    YAS_SCREEN_SIZE

#else

  #error One of ncurses/yascreen libraries is required under UNIX

#endif

/******* common (part 2) ***************************************************/

  #define KEY_INSERT    KEY_IC
  #define KEY_INS       KEY_IC
  #define KEY_DELETE    KEY_DC
  #define KEY_DEL       KEY_DC

/****************************************************************************
**
** Functions
**
****************************************************************************/

  int con_init();  // should be called before any other con_xxx()
  void con_done(); // should be called at the end of the console io actions

  void con_suspend(); // suspends console (before system() for example)
  void con_restore(); // restores console after suspend
  void con_reset_screen_size(); // reset screen info on screen resize, called in SIGWINCH

  void con_ce( int attr = -1 ); // clear to end-of-line
  void con_cs( int attr = -1 ); // clear screen

  // following functions print string `s' at position `x,y' (col,row) with
  // color (attribute) `attr'
  void con_out( int x, int y, const char *s );
  void con_out( int x, int y, const char *s, int attr );
  void con_puts( const char *s );
  void con_puts( const char *s, int attr );

  void con_chide(); // cursor hide
  void con_cshow(); // cursor show

  int con_max_x(); // max screen x (column)
  int con_max_y(); // max screen y (row)
  int con_x();     // current screen x (column)
  int con_y();     // current screen y (row)

  void con_fg( int color ); // set foreground color
  void con_bg( int color ); // set background color
  void con_ta( int attr );  // set new attribute ( CONCOLOR(fg,bg) )

  void con_xy( int x, int y ); // move cursor to position x,y

  int con_kbhit(); // return != 0 if key is waiting in "keyboard" buffer
  void con_beep(); // make a "beep" sound

  int con_getch(); // get single char from the "keyboard"
  wchar_t con_getwch(); // return wide char or control key (control is in the range 0xF0000-0xFFFFD), 0 for none key

#endif /* _UNICON_H_ */
/****************************************************************************
** EOF
****************************************************************************/
