/****************************************************************************
 *
 *  Copyright (c) 1996-2022 Vladi Belperchinov-Shabanski "Cade" 
 *  http://cade.noxrun.com/  <cade@noxrun.com> <cade@bis.bg> <cade@cpan.org>
 *
 * SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 ****************************************************************************/

#include <assert.h>

#ifndef ASSERT
#define ASSERT assert
#endif

#include "scroll.h"

  void ScrollPos::home()
  {
    if ( ! _size ) return;
    _pos  = _min;
    _page = _min;
    fix();
  }

  void ScrollPos::end()
  {
    if ( ! _size ) return;
    _pos = _max;
    fix();
  }

  void ScrollPos::up()
  {
    if ( ! _size ) return;
    ASSERT( check() );
    _pos--;
    if ( _pos < _min )
      {
      if ( wrap )
        _pos = _max;
      else
        _pos = _min;
      }
    // if ( _pos < _page ) _page--;
    if ( _pos < _page ) 
      if( int( _pagestep ) != 1 && _pos - _pagesize < 0 )
        _page = 0;
      else  
        {
        int pstep = _pagestep >= 1 ? int( _pagestep ) : _pagestep * _pagesize;
        _page -= pstep < 1 ? 1 : pstep;
        }
    fix();
  }

  void ScrollPos::down()
  {
    if ( ! _size ) return;
    ASSERT( check() );
    _pos++;
    if ( _pos > _max )
      {
      if ( wrap )
        _pos = _min;
      else
        _pos = _max;
      }
    //if ( _pos > _page + _pagesize - 1 ) _page++;
    if ( _pos > _page + _pagesize - 1 ) 
      if( int( _pagestep ) != 1 && _max - _pos < _pagesize ) 
        _page = _max - _pos;
      else  
        {
        int pstep = _pagestep >= 1 ? int( _pagestep ) : _pagestep * _pagesize;
        _page += pstep < 1 ? 1 : pstep;
        }
    fix();
  }

  void ScrollPos::pageup()
  {
    if ( ! _size ) return;
    ASSERT( check() );
    if ( _pos != _page)
      _pos  = _page;
    else
      _pos -= _pagesize;
    fix();
  }

  void ScrollPos::pagedown()
  {
    if ( ! _size ) return;
    ASSERT( check() );
    if ( _pos != _page + _pagesize -1 )
      _pos = _page + _pagesize - 1;
    else
      {
      _pos += _pagesize;
      if ( _page + _pagesize <= _max ) _page += _pagesize;
      }
    fix();
  }

  void ScrollPos::go( int new_pos )
  {
    _pos = new_pos;
    fix();
  }

  void ScrollPos::fix()
  {
    if ( _pos  < _min ) _pos  = _min;
    if ( _pos  > _max ) _pos  = _max;
    if ( _page < _min ) _page = _min;
    if ( _page > _max ) _page = _max;
    if ( _pos  < _page || _pos > _page + _pagesize - 1 )
      {
      if ( _pagesize )
        _page = ( _pos / _pagesize ) * _pagesize;
      else
        _page = 0;
      }
    ASSERT( check() );
  }

  int ScrollPos::check()
  {
    if ( ! _size ) return 1;
    if ( _pos  < _min ) return 0;
    if ( _pos  > _max ) return 0;
    if ( _page < _min ) return 0;
    if ( _page > _max ) return 0;
    if ( _pos  < _page ) return 0;
    if ( _pagesize < 0 ) return 0;
    if ( _pagestep < 0 ) return 0;
//    if ( _pos >= _page + _pagesize ) return 0;
    return 1;
  }

 // eof scroll.cpp
