/*
 *
 * (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-1999
 *
 * SEE `README',LICENSE' OR COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 * $Id: vsuti.cpp,v 1.2 2001/10/28 13:53:02 cade Exp $
 *
 */

#include "vsuti.h"
#include "regexp2.h"
#include "vstring.h"

/*###########################################################################*/
/* CRC functions */

const unsigned long crc_32_tab[256] = {
0x00000000l, 0x77073096l, 0xee0e612cl, 0x990951bal, 0x076dc419l, 0x706af48fl, 0xe963a535l, 0x9e6495a3l,
0x0edb8832l, 0x79dcb8a4l, 0xe0d5e91el, 0x97d2d988l, 0x09b64c2bl, 0x7eb17cbdl, 0xe7b82d07l, 0x90bf1d91l,
0x1db71064l, 0x6ab020f2l, 0xf3b97148l, 0x84be41del, 0x1adad47dl, 0x6ddde4ebl, 0xf4d4b551l, 0x83d385c7l,
0x136c9856l, 0x646ba8c0l, 0xfd62f97al, 0x8a65c9ecl, 0x14015c4fl, 0x63066cd9l, 0xfa0f3d63l, 0x8d080df5l,
0x3b6e20c8l, 0x4c69105el, 0xd56041e4l, 0xa2677172l, 0x3c03e4d1l, 0x4b04d447l, 0xd20d85fdl, 0xa50ab56bl,
0x35b5a8fal, 0x42b2986cl, 0xdbbbc9d6l, 0xacbcf940l, 0x32d86ce3l, 0x45df5c75l, 0xdcd60dcfl, 0xabd13d59l,
0x26d930acl, 0x51de003al, 0xc8d75180l, 0xbfd06116l, 0x21b4f4b5l, 0x56b3c423l, 0xcfba9599l, 0xb8bda50fl,
0x2802b89el, 0x5f058808l, 0xc60cd9b2l, 0xb10be924l, 0x2f6f7c87l, 0x58684c11l, 0xc1611dabl, 0xb6662d3dl,
0x76dc4190l, 0x01db7106l, 0x98d220bcl, 0xefd5102al, 0x71b18589l, 0x06b6b51fl, 0x9fbfe4a5l, 0xe8b8d433l,
0x7807c9a2l, 0x0f00f934l, 0x9609a88el, 0xe10e9818l, 0x7f6a0dbbl, 0x086d3d2dl, 0x91646c97l, 0xe6635c01l,
0x6b6b51f4l, 0x1c6c6162l, 0x856530d8l, 0xf262004el, 0x6c0695edl, 0x1b01a57bl, 0x8208f4c1l, 0xf50fc457l,
0x65b0d9c6l, 0x12b7e950l, 0x8bbeb8eal, 0xfcb9887cl, 0x62dd1ddfl, 0x15da2d49l, 0x8cd37cf3l, 0xfbd44c65l,
0x4db26158l, 0x3ab551cel, 0xa3bc0074l, 0xd4bb30e2l, 0x4adfa541l, 0x3dd895d7l, 0xa4d1c46dl, 0xd3d6f4fbl,
0x4369e96al, 0x346ed9fcl, 0xad678846l, 0xda60b8d0l, 0x44042d73l, 0x33031de5l, 0xaa0a4c5fl, 0xdd0d7cc9l,
0x5005713cl, 0x270241aal, 0xbe0b1010l, 0xc90c2086l, 0x5768b525l, 0x206f85b3l, 0xb966d409l, 0xce61e49fl,
0x5edef90el, 0x29d9c998l, 0xb0d09822l, 0xc7d7a8b4l, 0x59b33d17l, 0x2eb40d81l, 0xb7bd5c3bl, 0xc0ba6cadl,
0xedb88320l, 0x9abfb3b6l, 0x03b6e20cl, 0x74b1d29al, 0xead54739l, 0x9dd277afl, 0x04db2615l, 0x73dc1683l,
0xe3630b12l, 0x94643b84l, 0x0d6d6a3el, 0x7a6a5aa8l, 0xe40ecf0bl, 0x9309ff9dl, 0x0a00ae27l, 0x7d079eb1l,
0xf00f9344l, 0x8708a3d2l, 0x1e01f268l, 0x6906c2fel, 0xf762575dl, 0x806567cbl, 0x196c3671l, 0x6e6b06e7l,
0xfed41b76l, 0x89d32be0l, 0x10da7a5al, 0x67dd4accl, 0xf9b9df6fl, 0x8ebeeff9l, 0x17b7be43l, 0x60b08ed5l,
0xd6d6a3e8l, 0xa1d1937el, 0x38d8c2c4l, 0x4fdff252l, 0xd1bb67f1l, 0xa6bc5767l, 0x3fb506ddl, 0x48b2364bl,
0xd80d2bdal, 0xaf0a1b4cl, 0x36034af6l, 0x41047a60l, 0xdf60efc3l, 0xa867df55l, 0x316e8eefl, 0x4669be79l,
0xcb61b38cl, 0xbc66831al, 0x256fd2a0l, 0x5268e236l, 0xcc0c7795l, 0xbb0b4703l, 0x220216b9l, 0x5505262fl,
0xc5ba3bbel, 0xb2bd0b28l, 0x2bb45a92l, 0x5cb36a04l, 0xc2d7ffa7l, 0xb5d0cf31l, 0x2cd99e8bl, 0x5bdeae1dl,
0x9b64c2b0l, 0xec63f226l, 0x756aa39cl, 0x026d930al, 0x9c0906a9l, 0xeb0e363fl, 0x72076785l, 0x05005713l,
0x95bf4a82l, 0xe2b87a14l, 0x7bb12bael, 0x0cb61b38l, 0x92d28e9bl, 0xe5d5be0dl, 0x7cdcefb7l, 0x0bdbdf21l,
0x86d3d2d4l, 0xf1d4e242l, 0x68ddb3f8l, 0x1fda836el, 0x81be16cdl, 0xf6b9265bl, 0x6fb077e1l, 0x18b74777l,
0x88085ae6l, 0xff0f6a70l, 0x66063bcal, 0x11010b5cl, 0x8f659effl, 0xf862ae69l, 0x616bffd3l, 0x166ccf45l,
0xa00ae278l, 0xd70dd2eel, 0x4e048354l, 0x3903b3c2l, 0xa7672661l, 0xd06016f7l, 0x4969474dl, 0x3e6e77dbl,
0xaed16a4al, 0xd9d65adcl, 0x40df0b66l, 0x37d83bf0l, 0xa9bcae53l, 0xdebb9ec5l, 0x47b2cf7fl, 0x30b5ffe9l,
0xbdbdf21cl, 0xcabac28al, 0x53b39330l, 0x24b4a3a6l, 0xbad03605l, 0xcdd70693l, 0x54de5729l, 0x23d967bfl,
0xb3667a2el, 0xc4614ab8l, 0x5d681b02l, 0x2a6f2b94l, 0xb40bbe37l, 0xc30c8ea1l, 0x5a05df1bl, 0x2d02ef8dl
};

/* should start with `0xffffffff' for `crc' and result is crc = ~crc; */
crc32_t update_crc32( const unsigned char octet, const crc32_t crc )
{
  return (crc_32_tab[(unsigned char)((unsigned char)crc ^ octet)] ^ ((crc >> 8) & 0x00FFFFFFl));
};

crc32_t mem_crc32( const void* buff, int size )
{
  crc32_t crc = 0xffffffff;
  int z;
  for ( z = 0; z < size; z++ ) crc = update_crc32( ((unsigned char*)buff)[z], crc );
  return ~crc;
};

crc32_t str_crc32( const char *s )
{
  return mem_crc32( s, strlen(s) );
};

crc32_t file_crc32( FILE *f, long buffsize ) /* return CRC32NULL for error */
{
  ASSERT( f );

  char *buff = (char*)malloc( buffsize );
  if (buff == NULL) return CRC32NULL;

  crc32_t crc = CRC32NULL;
  while(42)
    {
    long res = fread( buff, 1, buffsize, f );
    if (res == -1)
      {
      fclose( f );
      return CRC32NULL;
      }
    long z;
    for ( z = 0; z < res; z++ ) crc = update_crc32( buff[z], crc );
    if ( res != buffsize ) break;
    }
  free( buff );

  return ~crc;
};

crc32_t file_crc32( const char *fname, long buffsize  ) // return `0xffffffff' for error
{
  FILE *f = fopen( fname, "rb" );
  if (!f) return CRC32NULL;
  crc32_t crc = file_crc32( f, buffsize );
  fclose(f);
  return crc;
};

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

adler32_t file_adler32( FILE *f, long buffsize = 256*1024  )
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

adler32_t file_adler32( const char *fname, long buffsize = 256*1024  )
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

long file_find_string( const char *pattern, const char* file_name, int nocase, int spos = -1 )
{
  FILE *f = fopen( file_name, "rb" );
  if (!f) return -1;
  long pos = file_find_string( pattern, f, nocase, spos );
  fclose(f);
  return pos;
}

long file_find_string( const char *pattern, FILE* f, int nocase, int spos = -1 )
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

long file_find_pattern( const char *pattern, int pattern_size, const char* file_name, int nocase, int spos = -1 )
{
  FILE *f = fopen( file_name, "rb" );
  if (!f) return -1;
  long pos = file_find_pattern( pattern, pattern_size, f, nocase, spos );
  fclose(f);
  return pos;
};

long file_find_pattern( const char *pattern, int pattern_size, FILE* f, int nocase, int spos = -1 )
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

long file_kmp_search( const char *pattern, int pattern_size, FILE* f, int nocase, int spos = -1 )
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

long file_grep( const char *re_string, const char* file_name, int nocase, int spos = -1 )
{    
  FILE *f = fopen( file_name, "rb" );
  if (!f) return -1;
  long pos = file_grep( re_string, f, nocase, spos );
  fclose(f);
  return pos;
}	

int file_grep_max_line = MAX_GREP_LINE;
int file_grep_lines_read = 0;
long file_grep( const char *re_string, FILE* f, int nocase, int spos = -1 )
{
  if ( strlen(re_string) >= (size_t)file_grep_max_line ) return -2; // just in case, and for now...

  char newpat[MAX_PATTERN+1];
  strcpy( newpat, re_string );
  if ( nocase ) str_up( newpat );
  regexp *re = regcomp( newpat );
  if (re == NULL) return -2;
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
    if ( regexec( re, line ) )
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
    cpos += ( re->startp[0] - line );

  free(line);
  free(re);
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

String& tilde_expand( String &str )
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
  
  String this_path = path;
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
            int level = -1 )
{
  int r;
  
  if ( !origin || !func || !origin[0] ) return 255;

  String o = origin;
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
