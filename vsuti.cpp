/****************************************************************************
 *
 *  Copyright (c) 1996-2022 Vladi Belperchinov-Shabanski "Cade"
 *  http://cade.noxrun.com/  <cade@noxrun.com> <cade@bis.bg> <cade@cpan.org>
 *
 * SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 ****************************************************************************/

#include "vsuti.h"
#include "vstring.h"
#include "vstrlib.h"
#include "vstruti.h"

/*###########################################################################*/

/* adler32.c -- compute the Adler-32 checksum of a data stream
 * Copyright (C) 1995-1996 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

#define BASE 65521L /* largest prime smaller than 65536 */
#define NMAX 5552
/* NMAX is the largest n such that 255n(n+1)/2 + (n+1)(BASE-1) <= 2^32-1 */

#define DO1(buf,i)  {s1 += buf[i]; s2 += s1;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);

/*---------------------------------------------------------------------------*/

unsigned long adler32(unsigned long adler, const char *buf, unsigned int len)
{
    unsigned long s1 = adler & 0xffff;
    unsigned long s2 = (adler >> 16) & 0xffff;
    int k;

    if (buf == NULL) return 1L;

    while (len > 0) {
        k = len < NMAX ? len : NMAX;
        len -= k;
        while (k >= 16) {
            DO16(buf);
            buf += 16;
            k -= 16;
        }
        if (k != 0) do {
            s1 += *buf++;
            s2 += s1;
        } while (--k);
        s1 %= BASE;
        s2 %= BASE;
    }
    return (s2 << 16) | s1;
}

/*---------------------------------------------------------------------------*/

adler32_t mem_adler32( const void* buff, int size )
{
  return adler32( adler32( 0, NULL, 0 ), (const char *)buff, size );
}

/*---------------------------------------------------------------------------*/

adler32_t str_adler32( const char *s )
{
  return mem_adler32( s, strlen(s) );
}

/*---------------------------------------------------------------------------*/

adler32_t file_adler32( FILE *f, long buffsize  )
{
  ASSERT( f );

  char *buff = (char*)malloc( buffsize );
  if (buff == NULL) return 0;

  adler32_t adler = adler32( 0, NULL, 0 );
  while(42)
    {
    long res = fread( buff, 1, buffsize, f );
    adler = adler32( adler, buff, res );
    if ( res != buffsize )
      {
      if ( ferror(f) )
        {
        free( buff );
        return 0;
        }
      break;
      }
    }
  free( buff );

  return adler;
}

/*---------------------------------------------------------------------------*/

adler32_t file_adler32( const char *fname, long buffsize  )
{
  FILE *f = fopen( fname, "rb" );
  if (!f) return 0;
  adler32_t adler = file_adler32( f, buffsize );
  fclose(f);
  return adler;
}

/*###########################################################################*/
/* FILE functions */

off_t file_size( const char *fname )
{
  struct stat st;
  if (stat( fname, &st )) return -1;
  return st.st_size;
}

/*---------------------------------------------------------------------------*/

off_t file_size( FILE *f )
{
  int res = 0;
  off_t opos = ftello( f );
  if (opos == -1) return -1;
  if (fseeko( f, 0, SEEK_END )) res++;
  off_t size = ftello( f );
  res += (size == -1);
  if (fseeko( f, opos, SEEK_SET )) res++;
  if (res) return -1;
  return size;
}

/*---------------------------------------------------------------------------*/

int file_load( FILE *f, void *buff, int size )
{
  return ( fread( buff, 1, size, f ) != (size_t)size);
}

/*---------------------------------------------------------------------------*/

int file_save( FILE *f, void *buff, int size )
{
  return (fwrite( buff, 1, size, f ) != (size_t)size);
}

/*---------------------------------------------------------------------------*/

int file_load( const char* fname, void *buff, int size )
{
  FILE *f = fopen( fname, "rb" );
  if (!f) return 1;
  int res = file_load( f, buff, size );
  fclose( f );
  return res;
}

/*---------------------------------------------------------------------------*/

int file_save( const char* fname, void *buff, int size )
{
  FILE *f = fopen( fname, "wb" );
  if (!f) return 1;
  int res = file_save( f, buff, size );
  fclose( f );
  return res;
}

/*---------------------------------------------------------------------------*/

int file_load_crc32( const char* fname, void *buff, int size )
{
  crc32_t crc;
  FILE *f = fopen( fname, "rb" );
  if (!f) return 1;
  int res = 0;
  res += ( fread( buff, 1, size, f ) != (size_t)size );
  res += ( fread( &crc, 1, sizeof(crc), f ) != sizeof(crc) );
  fclose(f);
  res += ( crc != mem_crc32( buff, size ) );
  return res;
}

/*---------------------------------------------------------------------------*/

int file_save_crc32( const char* fname, void *buff, int size )
{
  crc32_t crc = mem_crc32( buff, size );
  FILE *f = fopen( fname, "wb" );
  if (!f) return 1;
  int res = 0;
  res += ( fwrite( buff, 1, size, f ) != (size_t)size );
  res += ( fwrite( &crc, 1, sizeof(crc), f ) != sizeof(crc) );
  fclose(f);
  return res;
}


int file_is_link( const char* fname )
{
  #ifdef _TARGET_GO32_
  return 0;
  #else
  struct stat st;
  if (lstat( fname, &st )) return 0; /* consider it not link */
  return !!( S_ISLNK(st.st_mode) );
  #endif
}

/*---------------------------------------------------------------------------*/

int file_is_dir( const char* fname )
{
  struct stat st;
  if (stat( fname, &st )) return 0; /* consider it not link */
  return !!( S_ISDIR(st.st_mode) );
}

/*---------------------------------------------------------------------------*/

int file_is_dir( struct stat st )
{
  return !!( S_ISDIR(st.st_mode) );
}

int file_exists( const char* fname )
{
  return access( fname, F_OK ) == 0;
}

/*****************************************************************************
**
** tilde_expand() expands ~/path and ~name/path to real pathname.
** it uses /etc/passwd or $HOME environment variable for ~ substitution.
**
*****************************************************************************/

VString tilde_expand( const char* a_path )
{ //!ok
  if ( ! a_path || ! a_path[0] || a_path[0] != '~' )
    return VString( a_path );

  VString new_home;

  VString user;
  int user_end = str_find( a_path, '/' );
  if ( user_end == -1 && a_path[1] != 0 )       /* ~user */
    user = a_path + 1;
  else if ( user_end > 1 )                      /* ~user/foo */
    str_copy( user, a_path, 1, user_end - 1 );
  else                                          /* ~ or ~/... */
    user = getenv( "USER" );

  if( user != "" )
    {
    struct passwd* pwd = getpwnam( user );
    if( pwd && pwd->pw_dir ) new_home = pwd->pw_dir;
    }
  else
    {
    char* home_dir = getenv("HOME");
    if( home_dir ) new_home = home_dir;
    }
  if( new_home == "" ) return VString( a_path );

  if( user_end == -1 && new_home[-1] != '/' ) new_home += "/";

  if( user_end == -1 )
    return VString( new_home );
  else
    new_home += a_path + user_end;

  return new_home;
}

/*****************************************************************************
**
** make_path() create new directory including non-existing path entries.
** It can create /a/b/c/d/e/ without existing of `/a/' for example.
** return 0 for success
**
*****************************************************************************/

int mkpath( const char *path, mode_t mode )
{ //!ok
  char   tmp[PATH_MAX];
  size_t len;
  int    n;

  if( ! path || ! path[0] ) return -2;

  len = strlen( path  );
  if( len + 1 >= sizeof(tmp) ) return -3;

  strncpyz_buf( tmp, path );

  if( tmp[len - 1] != '/' ) strncatz_buf( tmp, "/" );

  for( n = 1; tmp[n]; n++ )
    {
    if( tmp[n] != '/' ) continue;
    tmp[n] = 0;
    if( mkdir( tmp, mode ) != 0 && errno != EEXIST ) return -1;
    tmp[n] = '/';
    }

  return 0;
}

/*****************************************************************************
**
** expand_path() resolves symlinks etc.
**
*****************************************************************************/

char* expand_path( const char *src, char *dest )
{ //!ok
  if( ! realpath( src, dest ) )
    strncpyz( dest, src, MAX_PATH );

  return dest;
}

VString expand_path( const char* src )
{ //!ok
  char temp[MAX_PATH];
  VString dest = expand_path( src, temp );
  return dest;
}

/*****************************************************************************
**
** shell_escape() escapes shell special characters    '\"`&;*()[]{}!^:
**
** this seems to work fine with bash, zsh, csh, tcsh, fish, ksh, dash
*****************************************************************************/

VString& shell_escape( VString &dest )
{ //!ok
  VString out = "'";
  int sl = strlen( dest );
  for( int i = 0; i < sl; i++ )
    {
    if( dest[i] == '\'' )
      out += "'\\''";        /* close, escape, reopen */
    else
      out += dest[i];
    }
  out += "'";
  dest = out;
  return dest;
}

VString shell_escape( const char* src )
{ //!ok
  VString dest;
  dest = src;
  shell_escape( dest );
  return dest;
}

/*****************************************************************************
**
** ftwalk() traverses directory tree and calls func() for every entry, dir exit
**
*****************************************************************************/

int __ftwalk_process( const char *origin,
                      const char *path,
                      int (*func)( const char* origin,    /* origin path    */
                                   const char* fname,     /* full file name */
                                   const struct stat* st, /* stat, zeroed if error */
                                   int is_link,           /* 1 if link      */
                                   int flag ),
                      int level = -1 )
{ //!ok
  DIR           *dir;
  struct dirent *de;
  struct stat   st;
  int           flag;

  if ( level != -1 && level == 0) return 0; /* required level reached */

  VString this_path = path;
  int this_path_len = str_len( this_path );

  dir = opendir( this_path );
  if (!dir) return 0; /* consider it ok */

  while( (de = readdir(dir)) )
    {
    if ( strcmp( de->d_name, "." ) == 0 || strcmp(de->d_name, "..") == 0 ) continue;

    this_path += de->d_name;
    int is_link = file_is_link( this_path );
    if (stat(this_path, &st))
      {
      flag = FTWALK_NS;
      memset( &st, 0, sizeof(st) );
      }
    else if (S_ISDIR(st.st_mode))
      flag = FTWALK_D;
    else
      flag = FTWALK_F;

    int r;

    // entry handled, directory enter as well if dir
    r = func( origin, this_path, &st, is_link, flag );
    if ( r )
      {
      // func returned non-zero (error) stop here and return back, propagating r
      closedir(dir);
      return r;
      }

    if( flag == FTWALK_D && ! is_link )
      {
      this_path += "/";
      r = __ftwalk_process( origin, this_path, func, level - 1 );
      if( r )
        {
        // __ftwalk_process returned non-zero (error) stop here and return back, propagating r
        closedir(dir);
        return r;
        }
      str_trim_right( this_path, 1 ); /* remove trailing `/' */

      // directory exit
      r = func( origin, this_path, &st, is_link, FTWALK_DX );
      if( r )
        {
        // func returned non-zero (error) stop here and return back, propagating r
        closedir(dir);
        return r;
        }
      }
    str_sleft( this_path, this_path_len );
    } /* while readdir(dir) */

  closedir(dir);
  return 0;
}

int ftwalk( const char *origin,
            int (*func)( const char* origin,    /* origin path */
                         const char* fname,     /* full file name */
                         const struct stat* st, /* stat, zeroed if error */
                         int is_link,           /* 1 if link */
                         int flag ),
            int level )
{ //!ok
  if ( !origin || !func || !origin[0] ) return 255;

  VString o = origin;
  str_fix_path( o, '/' );

  if ( ! file_is_dir( o ) ) return 255;
  int r = __ftwalk_process( o, o, func, level );
  return r;
}

/*****************************************************************************
**
** get_rc_directory() return application rc directory (and possibly create it)
** returned dir is $HOME/.dir_prefix or $HOME/$RC_PREFIX/dir_prefix depending
** on $RC_PREFIX existence.
**
*****************************************************************************/


VString get_rc_directory( const char* dir_prefix )
{ //!ok
  VString rc_dir;
  rc_dir = getenv("HOME");
  if ( rc_dir == "" ) rc_dir = "/tmp/";
  str_fix_path( rc_dir, '/' );

  const char *rc_prefix = getenv("RC_PREFIX");
  if( ! rc_prefix || ! rc_prefix[0] ) rc_prefix = NULL;
  if( rc_prefix )
    rc_dir += rc_prefix;
  str_fix_path( rc_dir, '/' );
  if ( dir_prefix && dir_prefix[0] )
    {
    if ( rc_prefix )
      rc_dir += dir_prefix;
    else
      {
      rc_dir += ".";
      rc_dir += dir_prefix;
      }
    str_fix_path( rc_dir, '/' );
    }
  make_path( rc_dir );
  return rc_dir;
}

/*****************************************************************************
**
** EOF
**
*****************************************************************************/


