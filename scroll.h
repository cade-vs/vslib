/*
 *
 * (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-1999
 *
 * SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 */

#ifndef _SCROLL_H_
#define _SCROLL_H_

class TScrollPos
  {
  public:
    int type; // 0 -- standart static, else -- extended non static
    int wrap; // 0 -- none, else -- wrap end/begin; NOTE: works only on up/down
    int min;
    int max;
    int pos;
    int page;
    int pagesize;
    int step;

    TScrollPos()
      { wrap = type = min = max = pos = page = pagesize = step = 0; };

    void create( int pmin, int pmax, int ppos, int ppage, int ppagesize, int pstep)
          { min = pmin; max = pmax; pos = ppos; page = ppage; pagesize = ppagesize; step = pstep; };

    void home();
    void end();
    void up();
    void down();

    void pageup();
    void pagedown();

    void ppage() { pageup(); }
    void npage() { pagedown(); }

    void gotopos( int newpos );
  };

#endif //_SCROLL_H_

 // eof scroll.h
