/*
 *
 * (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-1999
 *
 * SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 * $Id: vsuti.h,v 1.2 2001/10/28 13:53:02 cade Exp $
 *
 */

#ifndef _VSUTI_H_
#define _VSUTI_H_

#include <target.h>

#include <sys/stat.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#ifdef _TARGET_UNIX_
#include <pwd.h>
#include <sys/types.h>
#include <sys/param.h>
#endif

#include "vstring.h"

#include <assert.h>
#ifndef ASSERT
#define ASSERT assert
#endif

/*###########################################################################*/
/* MISC defines */

/* max filename length */
#ifndef MAX_PATH
#define MAX_PATH	512
#endif

/* max pattern length for file_find_*() and ... */
#ifndef MAX_PATTERN
#define MAX_PATTERN		2048
#endif

/* max file_grep() text line input length... :| */
#ifndef MAX_GREP_LINE
#define MAX_GREP_LINE		4096
#endif

/*###########################################################################*/
/* CRC functions */

typedef unsigned long int crc32_t;

#define CRC32NULL (0xffffffff)

/* should start with `0xffffffff' for `crc' and result is crc = ~crc; */
crc32_t update_crc32( const unsigned char octet, const crc32_t crc );

crc32_t mem_crc32( const void* buff, int size );
crc32_t str_crc32( const char *s );
crc32_t file_crc32( FILE *f, long buffsize = 256*1024  );
crc32_t file_crc32( const char *fname, long buffsize = 256*1024  );

/*###########################################################################*/

typedef unsigned long int adler32_t;

adler32_t adler32(adler32_t adler, const char *buf, unsigned int len);

adler32_t mem_adler32( const void* buff, int size );
adler32_t str_adler32( const char *s );
adler32_t file_adler32( FILE *f, long buffsize = 256*1024  );
adler32_t file_adler32( const char *fname, long buffsize = 256*1024  );

/*###########################################################################*/
/* FILE functions */

long file_size( const char *fname );
long file_size( FILE *f );

int file_load( FILE *f, void *buff, int size = -1 );
int file_save( FILE *f, void *buff, int size = -1 );
int file_load( const char* fname, void *buff, int size = -1 );
int file_save( const char* fname, void *buff, int size = -1 );

int file_load_crc32( const char* fname, void *buff, int size );
int file_save_crc32( const char* fname, void *buff, int size );

int file_is_link( const char* fname );
int file_is_dir( const char* fname );
int file_is_dir( struct stat st );
int file_exists( const char* fname );

/*
 Converts hex-string to binary pattern (data)
 example: `ff 01 ab 11 2c' -> ...
 returns result pattern length
*/
int hex_string_to_pattern( const char *str, char* pattern );

/*
 This function is used for searching for string patterns
 spos    -- defines what file start offset should be accepted					 
 pattern format is:
 ~pattern -- regular expression ( start with `~' => `~^[123]+\t')
 			 this spawns file_grep() function
 $pattern -- hex pattern ( start with `$' => `$ FF 0A B4...' )
 			 converts it into new binary pattern and call file_find_pattern
  pattern -- normal exact string pattern ( use `\' to escape first char:
             example => `\~tilde_expansion' or `\$440 is too much' )
  			 just calculates string length and call file_find_pattern
 tip: if you want to search normal pattern which starts with `\' you
      have to enter this: `\\start slash etc.' or
	  use file_find_pattern() with strlen() etc.
 return:
 -1    -- pattern not found
 -2    -- pattern incorrect (for regexp's and hex patterns)
 else  -- found position
*/

long file_find_string( const char *pattern, const char* file_name, int nocase, int spos = -1 );
long file_find_string( const char *pattern, FILE* f, int nocase, int spos = -1 );

/*
 This function is used for searching for arbitrary (binary) patterns
 return:
 -1    -- pattern not found
 -2    -- pattern incorrect (for regexp's and hex patterns)
 else  -- found position
*/
long file_find_pattern( const char *pattern, int pattern_size, const char* file_name, int nocase, int spos = -1 );
long file_find_pattern( const char *pattern, int pattern_size, FILE* f, int nocase, int spos = -1 );

/*
 This function is kind of `low level' one and you should use
 file_find_*() functions
*/
long file_kmp_search( const char *pattern, int pattern_size, FILE* f, int nocase, int spos = -1 );

/*
 This function reads lines from a text file and runs regexp on it.
 file_grep_max_line defines the max line length read (1024)
 file_grep_lines_read reports how many lines are read in during the
                      last file_grep() call
 re_string is regexp string, not arbitrary (binary) pattern
 spos defines what file start offset should be accepted					 
*/
extern int file_grep_max_line;
extern int file_grep_lines_read;
long file_grep( const char *re_string, const char* file_name, int nocase, int spos = -1 );
long file_grep( const char *re_string, FILE* f, int nocase, int spos = -1 );

/*###########################################################################*/
/* FILENAMES functions */

char* tilde_expand( char* path ); // expands ~/path and ~name/path
String& tilde_expand( String &str ); // expands ~/path and ~name/path

/*###########################################################################*/

/*
 Creates new directory path, i.e. can create /a/b/c/d/e/ without existing
 of `/a/' for example...
*/
int make_path( const char *s, long mode = S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH );

/*
 Expands given path to the real one, resolves symlinks and removes
 `/./' and `/../'s ( actually calls system function call )
 return pointer to dest or NULL if error
*/
char* expand_path( const char *src, char *dest );

/*###########################################################################*/

#define FTWALK_F        1 /* file (regular) */
#define FTWALK_D        2 /* dir */
#define FTWALK_DX       3 /* call on exit directory */
#define FTWALK_NS       4 /* stat() failed */

/* func() should return 0 for ok, -1 */
int ftwalk( const char *origin_dir,
            int (*func)( const char* origin,    /* origin path */
                         const char* fname,     /* full file name */
                         const struct stat* st, /* stat struture or NULL */
                         int is_link,           /* 1 if link */
                         int flag ), 
            int level = -1 );

#ifdef _TARGET_GO32_
#include <dirent.h>
#include <dir.h>
int dosstat( DIR *dir, struct stat *stbuf );
#endif

/*###########################################################################*/
/* 
 CONFIG functions

 this returns `$HOME/$RC_PREFIX/$dir_prefix/'
 where $HOME and $RC_PREFIX are env.vars and `$dir_prefix' is arg. here
 this also makes sure that directory exists (i.e. create it)
 if $HOME is not available `/tmp/' is returned
*/ 
const char* get_rc_directory( const char* dir_prefix, char *rc_dir );

#endif /* _VUTILS_H_ */

/* eof vutils.h */
