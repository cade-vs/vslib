/*
 *
 * (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-2003
 *
 * SEE `README',LICENSE' OR COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 * $Id: vsuti.cpp,v 1.8 2003/01/21 19:56:35 cade Exp $
 *
 */

#include "vsuti.h"
#include "vstring.h"
#include "vstrlib.h"

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
};

/*---------------------------------------------------------------------------*/

adler32_t str_adler32( const char *s )
{
  return mem_adler32( s, strlen(s) );
};

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
    if (res == -1)
      {
      fclose( f );
      return 0;
      }
    adler = adler32( adler, buff, res );
    if ( res != buffsize ) break;
    }
  free( buff );

  return adler;
};

/*---------------------------------------------------------------------------*/

adler32_t file_adler32( const char *fname, long buffsize  )
{
  FILE *f = fopen( fname, "rb" );
  if (!f) return 0;
  adler32_t adler = file_adler32( f, buffsize );
  fclose(f);
  return adler;
};

/*###########################################################################*/
/* FILE functions */

long file_size( const char *fname )
{
  struct stat st;
  if (stat( fname, &st )) return -1;
  return st.st_size;
};

/*---------------------------------------------------------------------------*/

long file_size( FILE *f )
{
  int res = 0;
  long opos = ftell( f );
  if (opos == -1) return -1;
  if (fseek( f, 0, SEEK_END )) res++;
  long size = ftell( f );
  res += (size == -1);
  if (fseek( f, opos, SEEK_SET )) res++;
  if (res) return -1;
  return size;
};

/*---------------------------------------------------------------------------*/

int file_load( FILE *f, void *buff, int size )
{
  return ( fread( buff, 1, size, f ) != (size_t)size);
};

/*---------------------------------------------------------------------------*/

int file_save( FILE *f, void *buff, int size )
{
  return (fwrite( buff, 1, size, f ) != (size_t)size);
};

/*---------------------------------------------------------------------------*/

int file_load( const char* fname, void *buff, int size )
{
  FILE *f = fopen( fname, "rb" );
  if (!f) return 1;
  int res = file_load( f, buff, size );
  fclose( f );
  return res;
};

/*---------------------------------------------------------------------------*/

int file_save( const char* fname, void *buff, int size )
{
  FILE *f = fopen( fname, "wb" );
  if (!f) return 1;
  int res = file_save( f, buff, size );
  fclose( f );
  return res;
};

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
};

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
};


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


////////////////////////////////////////////////////////////////////////////
//
//  HEX pattern conversion
//

/*
  Thanks to Ivo Baylov <ivo@datamax.bg>
  for the KMP search and hex pattern conversion!
  see below...
*/

  static inline int __is_hex( int ch )
  {
    return ( ch >= '0' && ch <= '9' ) ||
           ( ch >= 'A' && ch <= 'F' ) ||
           ( ch >= 'a' && ch <= 'f' );
  }
  
  static inline int __hex_code( int ch )
  {
    if( ch >= '0' && ch <= '9' )
      return ch - '0';
    if( ch >= 'A' && ch <= 'F' )
      return ch - 'A' + 10;
    if( ch >= 'a' && ch <= 'f' )
      return ch - 'a' + 10;
    assert( 0 );
  }
  
  int hex_string_to_pattern( const char *str, char* pattern )
  {
     const char *pc = pattern;
     int hex;
     while( *str )
     {
       if( *str == ' ' || *str == '\t' )
         {
         str++;
         continue;
         }
       if( ! __is_hex( *str ) )
         return 0;
       hex = __hex_code( *str );
       hex <<= 4;
       str++;
       if( ! *str )
         *pattern = hex;
       else
       if( *str == ' ' || *str == '\t' )
         return 0;
       else
       {
         hex += __hex_code( *str );
         str ++;
         *pattern = hex;
         pattern ++;
       }
     }
     return pattern - pc;
  };

////////////////////////////////////////////////////////////////////////////
//
// Search interface functions
//

long file_find_string( const char *pattern, const char* file_name, int nocase, int spos )
{
  FILE *f = fopen( file_name, "rb" );
  if (!f) return -1;
  long pos = file_find_string( pattern, f, nocase, spos );
  fclose(f);
  return pos;
}

long file_find_string( const char *pattern, FILE* f, int nocase, int spos )
{
  int pattern_size = strlen(pattern);
  ASSERT( pattern_size < MAX_PATTERN );

  if (pattern[0] == '~')
    { // regular expression
	return file_grep( pattern+1, f, nocase, spos );
    } else
  if (pattern[0] == '$')
    { // hex pattern
	char new_pattern[MAX_PATTERN+1];
	int z = hex_string_to_pattern( pattern+1, new_pattern );
	if (z > 0)
	  return file_kmp_search( new_pattern, z, f, nocase, spos );
	else
	  return -2;
    }
  else
    { // normal patters string
	if (pattern[0] == '\\')
	  return file_kmp_search( pattern+1, pattern_size-1, f, nocase, spos );
	else
	  return file_kmp_search( pattern, pattern_size, f, nocase, spos );
    }
};

long file_find_pattern( const char *pattern, int pattern_size, const char* file_name, int nocase, int spos )
{
  FILE *f = fopen( file_name, "rb" );
  if (!f) return -1;
  long pos = file_find_pattern( pattern, pattern_size, f, nocase, spos );
  fclose(f);
  return pos;
};

long file_find_pattern( const char *pattern, int pattern_size, FILE* f, int nocase, int spos )
{
  return file_kmp_search( pattern, pattern_size, f, nocase, spos );    
};


////////////////////////////////////////////////////////////////////////////
//
// Knuth-Morris-Pratt search
//

void __kmp_preprocess_pattern( const char* pattern, int pattern_size, int* next )
{
 int i=0;
 int j=next[0]=-1;
 while (i < pattern_size) 
   {
   while ((j > -1) && (pattern[i] != pattern[j])) j=next[j];
   i++;
   j++;
   if (pattern[i] == pattern[j]) 
     next[i]=next[j];
   else 
     next[i]=j;
   }
}

long file_kmp_search( const char *pattern, int pattern_size, FILE* f, int nocase, int spos )
{
  int  next[MAX_PATTERN+1];
  char newpat[MAX_PATTERN+1];
  int  j;
  int  c;
 
  long opos = ftell( f );
 
  if (spos != -1) fseek( f, spos, SEEK_SET );
 
  if( pattern_size < 1 || pattern_size > MAX_PATTERN ) return -2;
  
  memcpy( newpat, pattern, pattern_size );
  if (nocase)
    for ( j = 0; j < pattern_size; j++)
      if ( newpat[j] >= 'a' && newpat[j] <= 'z' )
        newpat[j] = toupper(newpat[j]);
		
  memset( &next, 0, sizeof(next) );
  __kmp_preprocess_pattern( newpat, pattern_size, next );

  j = 0;
  while( (c = getc(f)) != EOF )
    {
    if (nocase) c = toupper( c );
    while( j > -1 && newpat[j] != (char)c ) j = next[j];
    j++;
    if ( j >= pattern_size )
      {
      fflush( f );
      int rpos = ftell( f );
      fseek( f, opos, SEEK_SET );
      ASSERT( rpos - pattern_size >= 0 );
      return rpos - pattern_size;
      // to continue:
      // j = next[j];
      }
    }
  fseek( f, opos, SEEK_SET );
  return( -1 );
}

/* FGrep -- regular expression search (I know `G' here stands for <nothing>:)) */

long file_grep( const char *re_string, const char* file_name, int nocase, int spos )
{    
  FILE *f = fopen( file_name, "rb" );
  if (!f) return -1;
  long pos = file_grep( re_string, f, nocase, spos );
  fclose(f);
  return pos;
}	

int file_grep_max_line = MAX_GREP_LINE;
int file_grep_lines_read = 0;
long file_grep( const char *re_string, FILE* f, int nocase, int spos )
{
  if ( strlen(re_string) >= (size_t)file_grep_max_line ) return -2; // just in case, and for now...

  char newpat[MAX_PATTERN+1];
  strcpy( newpat, re_string );
  if ( nocase ) str_up( newpat );
  
  VRegexp re;
  if ( ! re.comp( newpat ) ) return -2;
  char *line = (char*)malloc( file_grep_max_line+1 );

  long filesize = file_size( f );
  int opos = ftell( f );
  ASSERT( spos >= -1 );
  if (spos != -1) fseek( f, spos, SEEK_SET );
  int cpos = ftell( f );

  file_grep_lines_read = 0;
  int found = 0;
  while( fgets( line, file_grep_max_line, f ) )
    {
    if ( nocase ) str_up( line );
    if ( re.m( line ) )
      {
      found = 1;
      break;
      }
    cpos = ftell( f );
    file_grep_lines_read++;
    if (cpos >= filesize) break;
    }

  fseek( f, opos, SEEK_SET );
  if (found)
    cpos += ( re.sub_sp( 0 ) );

  free(line);
  file_grep_max_line = MAX_GREP_LINE;
  return found ? cpos : -1;
}

/*###########################################################################*/
/* FILENAMES functions */

char* tilde_expand( char* path ) // expands ~/path and ~name/path
{
#ifdef _TARGET_UNIX_
  struct passwd* pwd;
  char temp[MAX_PATH] = "";
  if ( path[0] != '~' ) return path;

  str_fix_path( path );

  int z = 1; // first after ~
  while( path[z] != '/' && path[z] != 0 )
    temp[z-1] = path[z++];
  temp[z-1] = 0;
  z--; // length of `temp' username

  if (z)
    {
    pwd = getpwnam( temp );
    if (!pwd) return path;
    strcpy( temp, pwd->pw_dir );
    }
  else
    {
    char* pw_dir = getenv("HOME");
    if (!pw_dir) return path;
    strcpy( temp, pw_dir );
    }
  str_fix_path( temp ); // add slash to the end if not exist
  strcat( temp, path + z + 2 );
  strcpy( path, temp );
#endif //_TARGET_UNIX_
  return path;
};

VString& tilde_expand( VString &str )
{
#ifdef _TARGET_UNIX_
  char t[MAX_PATH];
  strcpy( t, str );
  tilde_expand( t );
  str = t;
#endif //_TARGET_UNIX_
  return str;
};

/*###########################################################################*/

int make_path( const char *s, long mode )
{
  char str[MAX_PATH];
  char tmp[MAX_PATH];
  strcpy( tmp, s );
  str_fix_path( tmp );
  int l = strlen( tmp );
  strcpy( str, tmp ); // to hold original expanded path
  while(1)
    {
    while ( l >= 0 && tmp[l] != '/' ) l--;
    ASSERT( l < 0 || tmp[l] == '/' );
    if ( l < 0 )
      break;
    else
      tmp[l+1] = 0;
    if ( access( tmp, F_OK ) == 0 ) break;
    l--;
    }
  while(1)
    {
      l++;
      while ( str[l] != 0 && str[l] != '/' ) l++;
      if ( str[l] == 0 ) break;
      strncpy( tmp, str, l );
      tmp[l] = 0;
      int res = mkdir( tmp, mode );
      if (res) return res;
    }
  return 0;
}

/*###########################################################################*/

char* expand_path( const char *src, char *dest )
{
  #ifdef _TARGET_UNIX_
  realpath( src, dest);
  #endif

  #ifdef _TARGET_GO32_
  _fixpath( src, dest );
  #endif
  
  return dest;
}

/*###########################################################################*/

#ifdef _TARGET_GO32_

/*
  This is specific to djgpp/libc 2.01 -- if it changes later this
  must be changed too...
*/

struct ___DIR {
  int num_read;
  char *name;
  int flags;
  struct ffblk ff;
  struct dirent de;
  int need_fake_dot_dotdot; /* 0=no, 1=.., 2=. */
};

/* Convert file date and time to time_t value suitable for
   struct stat fields.  */
time_t _file_time_stamp(unsigned int dos_ftime)
{
  struct tm file_tm;
  memset(&file_tm, 0, sizeof(struct tm));
  file_tm.tm_isdst = -1;    /* let mktime() determine if DST is in effect */
  file_tm.tm_sec  = (dos_ftime & 0x1f) * 2;
  file_tm.tm_min  = (dos_ftime >>  5) & 0x3f;
  file_tm.tm_hour = (dos_ftime >> 11) & 0x1f;
  file_tm.tm_mday = (dos_ftime >> 16) & 0x1f;
  file_tm.tm_mon  = ((dos_ftime >> 21) & 0x0f) - 1; /* 0 = January */
  file_tm.tm_year = (dos_ftime >> 25) + 80;
  return mktime(&file_tm);
}

int dosstat( DIR *dir, struct stat *statbuf )
{
  #define ff_blk (((___DIR*)(dir))->ff)

  #define READ_ACCESS     (S_IRUSR | S_IRGRP | S_IROTH)
  #define WRITE_ACCESS    S_IWUSR
  #define EXEC_ACCESS     (S_IXUSR | S_IXGRP | S_IXOTH)

  memset(statbuf, 0, sizeof(struct stat));

  unsigned dos_ftime = 0;
  dos_ftime = ( (unsigned short)ff_blk.ff_fdate << 16 ) +
                (unsigned short)ff_blk.ff_ftime;

  statbuf->st_uid     = getuid();
  statbuf->st_gid     = getgid();
  statbuf->st_nlink   = 1;
  statbuf->st_size    = ff_blk.ff_fsize;
  statbuf->st_mode   |= READ_ACCESS;
  if ( !(ff_blk.ff_attrib & 0x07) )  /* no R, H or S bits set */
     statbuf->st_mode |= WRITE_ACCESS;
  if (ff_blk.ff_attrib & 0x10)
     statbuf->st_mode |= (S_IFDIR | EXEC_ACCESS);
  /* Set regular file bit.  */
  statbuf->st_mode |= S_IFREG;

  /* Time fields. */
  statbuf->st_atime = statbuf->st_mtime = statbuf->st_ctime =
    _file_time_stamp(dos_ftime);

  if ( ! strcmp(ff_blk.lfn_magic,"LFN32") )
    {
      unsigned xtime;
      xtime = *(unsigned *)&ff_blk.lfn_ctime;
      if(xtime)                 /* May be zero if file written w/o lfn active */
        statbuf->st_ctime = _file_time_stamp(xtime);
      xtime = *(unsigned *)&ff_blk.lfn_atime;
      if(xtime > dos_ftime)     /* Accessed time is date only, no time */
        statbuf->st_atime = _file_time_stamp(xtime);
    }

  return 0;
  #undef ff_blk
}

#endif /* _TARGET_GO32_ */

/*###########################################################################*/

int __ftwalk_process( const char *origin,
                      const char *path,
                      int (*func)( const char* origin,    /* origin path */
                                   const char* fname,     /* full file name */
                                   const struct stat* st, /* stat or NULL */
                                   int is_link,           /* 1 if link */
                                   int flag ), 
                      int level = -1 )
{
  DIR           *dir;
  struct dirent *de;
  struct stat   st;
  int           flag;
  
  if ( level != -1 && level == 0) return 0; /* required level reqched */
  
  VString this_path = path;
  int this_path_len = str_len( this_path );
  
  dir = opendir( this_path );
  if (!dir) return 0; /* consider it ok */

  while( (de = readdir(dir)) )
    {
    if ( strcmp( de->d_name, "." ) == 0 || strcmp(de->d_name, "..") == 0 ) continue;
    
    this_path += de->d_name;
    int is_link = file_is_link( this_path );
    #ifdef _TARGET_GO32_
    if (dosstat(dir, &st)) /* dosstat() will never return != 0 */
    #else
    if (stat(this_path, &st))
    #endif
      flag = FTWALK_NS;
    else if (S_ISDIR(st.st_mode))
      flag = FTWALK_D;
    else
      flag = FTWALK_F;
    
    int r = func( origin, this_path, &st, is_link, flag );
    if ( r )
      {
        closedir(dir);
        return r;
      }

    if ( flag == FTWALK_D && !is_link )
      {
      this_path += "/";
      r = __ftwalk_process( origin, this_path, func, level - 1 );
      if ( r )        
        {
        closedir(dir);
        return r;
        }
      str_trim_right( this_path, 1 ); /* remove trailing `/' */  
      int r = func( origin, this_path, &st, is_link, FTWALK_DX );
      if ( r )
        {
          closedir(dir);
          return r;
        }
      }
    str_sleft( this_path, this_path_len );  
    } /* while readdir(dir) */

  closedir(dir);
  return 0;
}

/*---------------------------------------------------------------------------*/

int ftwalk( const char *origin,
            int (*func)( const char* origin,    /* origin path */
                         const char* fname,     /* full file name */
                         const struct stat* st, /* stat struture or NULL */
                         int is_link,           /* 1 if link */
                         int flag ), 
            int level )
{
  int r;
  
  if ( !origin || !func || !origin[0] ) return 255;

  VString o = origin;
  str_fix_path( o );

  if ( !file_is_dir( o ) ) return 255;
  r = __ftwalk_process( o, o, func, level );
  return r;
}

/*###########################################################################*/

const char* get_rc_directory( const char* dir_prefix, char *rc_dir )
{
  rc_dir[0] = 0;
  if (getenv("HOME"))
    strcpy( rc_dir, getenv("HOME"));
  else
    strcpy( rc_dir, "/tmp/" );
  #ifdef _TARGET_GO32_
  str_tr( rc_dir, "\\", "/" );
  #endif
  str_fix_path( rc_dir );
  int rcprefix = 1;
  if (getenv("RC_PREFIX"))
    strcat( rc_dir, getenv("RC_PREFIX"));
  else
    rcprefix = 0;
  str_fix_path( rc_dir );
  if (dir_prefix)
    {
    if ( rcprefix )
      strcat( rc_dir, dir_prefix );
    else
      {
      #ifdef _TARGET_GO32_
      strcat( rc_dir, "_" );
      #else
      strcat( rc_dir, "." );
      #endif
      strcat( rc_dir, dir_prefix );
      }
    str_fix_path( rc_dir );
    }
  make_path( rc_dir );
  return rc_dir;
};

/*###########################################################################*/

// eof vutils.cpp
