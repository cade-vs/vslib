/****************************************************************************
 *
 *  Copyright (c) 1996-2022 Vladi Belperchinov-Shabanski "Cade" 
 *  http://cade.noxrun.com/  <cade@noxrun.com> <cade@bis.bg> <cade@cpan.org>
 *
 *  SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 ****************************************************************************/

#ifndef _DLOG_H_
#define _DLOG_H_

#include <stdio.h>

#define LOG( what ) log( __FILE__, __LINE__, what )
#define LOGN( what, n ) log( __FILE__, __LINE__, what, n )

class TLogFile
{
  FILE *f;
  char log_fn[255];
  int keep_open;

  public:

  int on_stdout;
  int on_stderr;

  TLogFile();
  ~TLogFile();

  void create( const char *fname, int pkeep_open );
  void open();
  void close();

  void log( const char *fname, int line, const char *msg );
  void log( const char *fname, int line, const char *msg, int n );
  void log( const char *msg );
  void log( const char *msg, int n );
  void log( const char *msg, const char *arg );

};

#endif //_DLOG_H_
