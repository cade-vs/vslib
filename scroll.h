/****************************************************************************
 *
 *  Copyright (c) 1996-2022 Vladi Belperchinov-Shabanski "Cade" 
 *  http://cade.noxrun.com/  <cade@noxrun.com> <cade@bis.bg> <cade@cpan.org>
 *
 *  SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 ****************************************************************************/

#ifndef _SCROLL_H_
#define _SCROLL_H_

class ScrollPos
{
    int _min;
    int _max;
    int _pos;
    int _page;
    int _pagesize;
    
    
    /* 
    _pagestep -- step to change page on up/down out of the current page
    if _pagestep is between 0 and 1 it is considered percentage of _pagesize
    (with minimum step of 1)
    if _pagestep is 1 or above it is considered integer fixed step
    (limited to size of _pagesize)
    */
    double _pagestep; 
          

    int _size;

    void fix();
    int check();

  public:

    int wrap; // 0 -- none, else -- wrap end/begin; NOTE: works only on up/down

    ScrollPos()
      {
      wrap = _min = _max = _pos = _page = _pagesize = _size = 0;
      _pagestep = 1;
      };

    void set_min_max( int a_min, int a_max )
      { _min = a_min; _max = a_max; _size = _max - _min + 1; }
    void set_pos( int a_pos )
      { _pos = a_pos; }
    void set_page( int a_page )
      { _page = a_page; }
    void set_pagesize( int a_pagesize )
      { _pagesize = a_pagesize;
        if( _pagesize < 0 ) _pagesize = 0; 
        if( _pagestep >= 1 and _pagestep > _pagesize ) _pagestep = _pagesize; }
    void set_pagestep( double a_pagestep )
      { _pagestep = a_pagestep;
        if( _pagestep <= 0 ) _pagestep = 1; }

    int min() { return _min; }
    int max() { return _max; }
    int pos() { if ( ! _size ) return 0; return _pos; }
    int page() { if ( ! _size ) return 0; return _page; }
    int pagesize() { return _pagesize; }
    int step() { return _pagestep; }

    void home();
    void end();
    void up();
    void down();

    void pageup();
    void pagedown();

    void ppage() { pageup(); }
    void npage() { pagedown(); }

    void go( int new_pos );
};

#endif //_SCROLL_H_

 // eof scroll.h
