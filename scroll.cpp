/*
 *
 * (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-2003
 *
 * SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 * $Id: scroll.cpp,v 1.3 2003/01/21 19:56:35 cade Exp $
 *
 */

#include "scroll.h"

  void TScrollPos::home()
  {
    pos = min;
    page = min;
  };

  void TScrollPos::end()
  {
    pos = max;
    if (type)
      {
      if (pos > page + pagesize - 1 ) page = max - pagesize + 1;
      if (page < min) page = min;
      }
    else
      {
      page = (pos / pagesize)*pagesize;
      }
  };

  void TScrollPos::up()
  {
    pos--;
    if (pos < min)
      if (wrap)
        {
        gotopos( max );
        return;
        }
      else
        pos = min;
    if (type)
      {
      if (pos < page) page--;
      }
    else
      {
      page = (pos / pagesize)*pagesize;
      }
  };

  void TScrollPos::down()
  {
    pos++;
    if (pos > max)
      if (wrap)
        {
        gotopos( min );
        return;
        }
      else
        pos = max;
    if (type)
      {
      if (pos > page + pagesize - 1) page++;
      }
    else
      {
      page = (pos / pagesize)*pagesize;
      }
  };

  void TScrollPos::pageup()
  {
    if (type)
      {
      if (pos != page)
        {
        pos = page;
        }
      else
        {
        pos -= pagesize;
        if (pos < min) pos = min;
        page = pos;
        }
      }
    else
      {
      if (pos != page)
        {
        pos = page;
        }
      else
        {
        pos -= pagesize;
        if (pos < min) pos = min;
        page = (pos / pagesize)*pagesize;
        }
      }
  };

  void TScrollPos::pagedown()
  {
    if (type)
      {
      if (pos != page + pagesize -1)
        {
        pos = page + pagesize - 1;
        if (pos > max) pos = max;
        }
      else
        {
        pos += pagesize;
        if (pos > max) pos = max;
        if (page+pagesize <= max) page += pagesize;
        }
      }
    else
      {
      if (pos != page + pagesize -1)
        {
        pos = page + pagesize - 1;
        if (pos > max) pos = max;
        }
      else
        {
        pos += pagesize;
        if (pos > max) pos = max;
        page = (pos / pagesize)*pagesize;
        }
      }
  };

  void TScrollPos::gotopos( int newpos )
  {
    pos = newpos;
    page = (pos / pagesize)*pagesize;
  }


 // eof scroll.cpp
