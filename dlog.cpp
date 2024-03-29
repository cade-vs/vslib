/****************************************************************************
 *
 *  Copyright (c) 1996-2022 Vladi Belperchinov-Shabanski "Cade" 
 *  http://cade.noxrun.com/  <cade@noxrun.com> <cade@bis.bg> <cade@cpan.org>
 *
 *  SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 ****************************************************************************/

#include <time.h>
#include <string.h>
#include "dlog.h"

TLogFile::TLogFile()
{
  f = NULL;
  log_fn[0] = 0;
  keep_open = 0;
  on_stdout = 0;
  on_stderr = 0;
}

TLogFile::~TLogFile()
{
  close();
}

void TLogFile::create( const char *fname, int pkeep_open )
{
  strcpy( log_fn, fname );
  f = NULL;
  keep_open = pkeep_open;
  open();
  fprintf( f, "\n" );
  if (!keep_open) close();
}

void TLogFile::open()
{
  if ( f ) fclose( f );
  f = fopen( log_fn, "at" );
}

void TLogFile::close()
{
  if ( f ) fclose( f );
  f = NULL;
}

void TLogFile::log( const char *fname, int line, const char *msg )
{
  char tmp[1024];
  if (!keep_open) open();

  time_t now;
  time(&now);
  char stime[32];
  strcpy(stime, asctime(localtime(&now)));
  if (stime[strlen(stime) - 1] == '\n') stime[strlen(stime) - 1] = 0;


  if ( fname == NULL || line == -1 )
    sprintf( tmp, "%s : %s", stime, msg);
  else
    sprintf( tmp, "%s [%10s:%-5d] %s", stime, fname, line, msg);

  while(tmp[strlen(tmp) - 1] == '\n') tmp[strlen(tmp) - 1] = 0;
  strcat( tmp, "\n" );

  fputs( tmp, f );
  if (on_stdout) fputs( tmp, stdout );
  if (on_stderr) fputs( tmp, stderr );

  if (!keep_open && f != NULL) close();

}

void TLogFile::log( const char *msg )
{
  log( NULL, -1, msg );
}

void TLogFile::log( const char *msg, int n )
{
  char tmp[255];
  sprintf( tmp, msg, n );
  log( NULL, -1, tmp );
}

void TLogFile::log( const char *msg, const char *arg )
{
  char tmp[255];
  sprintf( tmp, msg, arg );
  log( NULL, -1, tmp );
}

void TLogFile::log( const char *fname, int line, const char *msg, int n )
{
  char tmp[255];
  sprintf( tmp, msg, n );
  log( fname, line, tmp );
}


