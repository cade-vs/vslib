/*
 *
 *  VSTRING Library supporting structures and functions
 * (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-2003
 *  Distributed under the GPL license, you should receive copy of GPL!
 *
 *  VSTRLIB library provides string data structures which mimic Perl's.
 *  There are several classes: 
 *    
 *  VArray -- array of VString elements
 *  VTrie -- associative array (hash) of VString elements
 *  VRegexp -- regular expression helper class
 *
 *  $Id: vstrlib.h,v 1.18 2003/01/21 19:56:35 cade Exp $
 *
 */

#ifndef _VSTRLIB_H_
#define _VSTRLIB_H_

#include <assert.h>
#ifndef ASSERT
#define ASSERT assert
#endif

#include <stdlib.h>
#include <time.h>
#include <pcre.h>

#include "vstring.h"

/***************************************************************************
**
** GLOBALS
**
****************************************************************************/

#define VCHARSET_BLOCK_SIZE 32

/****************************************************************************
**
** VString aditional functions
**
****************************************************************************/

char*  time2str( const time_t tim );
time_t str2time( const char* timstr );

int str_find_regexp( const char* target, const char* pattern, int startpos = 0 );
// warning: str_rfind_regexp() is slow! it can execute pattern matching to `n'
// times where n is the target string length...
int str_rfind_regexp( const char* target, const char* pattern );

/***************************************************************************
**
** VREGEXP
**
****************************************************************************/
/*
** options are:
**               i -- case insensitive
**               m -- multiline matches
**               s -- single line (`.' matches and NEWLINE's)
**               x -- extended (ifnores whitespace and comments)
**
** for more docs see perlre(1) and pcre library docs
**
**
** WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING!
** extracting of the captured substring is only possible while subject input
** line which is used as matching target is intact! which means that if you
** change this line between match and substring extraction this will lead to
** segmentation fault!
**
*/

/* number of subpatterns which can be catched by VRegexp::m() */
#define VREGEXP_MAX_SUBS    32

class VRegexp
{

  pcre*       re;
  pcre_extra *pe;
  int         sp[VREGEXP_MAX_SUBS*3]; // sub pointers
  int         rc;
  const char *lp; // last line matched ptr
  
  VString substr;
  VString errstr;

  int get_options( const char* opt );
  
  public:

  VRegexp();
  VRegexp( const char* pattern, const char *opt = NULL ); // compiles new regexp
  ~VRegexp(); 

  int comp( const char* pattern, const char *opt = NULL ); // compile re, return > 0 for success
  int study(); // optimizing regexp for (big-size) multiple matches
  int ok(); // return 1 if regexp is compiled ok, 0 if not
  
  int m( const char* line ); // execute re against line, return 1 for match
  int m( const char* line, const char* pattern, const char *opt = NULL ); // same as exec, but compiles first

  const char* sub( int n ); // return n-th substring match
  int sub_sp( int n ); // return n-th substring start position
  int sub_ep( int n ); // return n-th substring end position
  
  VString& operator []( int n ) // same as sub()
    { substr = ""; sub( n ); return substr; }
    
  const char* error_str() { return errstr.data(); };  
};

/***************************************************************************
**
** VCHARSET
**
****************************************************************************/

class VCharSet
  {
    unsigned char *_data;
    int            _size;  // size (in bytes)

    void resize( int new_size );

    public:

    VCharSet();
    ~VCharSet();

    void push( int n, int val = 1 );
    void undef( int n );
    void undef();

    int  in( int n );
/*    
    
    push
    
    int  get ( int pn );

    void set_range1( int start, int end );
    void set_range0( int start, int end );
    
    void set_str1( const char* str );
    void set_str0( const char* str );

    int in( const char *str ); // return 1 if all str's chars are in the set
    int in( int pn )
        { if ( pn < 0 || pn >= size ) return 0; else return get( pn ); };
    
    void reverse() { for(int z = 0; z < datasize; z++) data[z] = ~data[z]; };
    void set( int pn, int val ) { if ( val ) set1( pn ); else set0( pn ); };
    void set_all1() { if ( data ) memset( data, 0xff, datasize ); };
    void set_all0() { if ( data ) memset( data, 0x00, datasize ); };

    const int operator [] ( int pn )
       { ASSERT( pn >= 0 && pn < size  ); return get( pn ); };

    int resize( int p_size );

    VCharSet& operator  = ( const VCharSet &b1 );
    VCharSet& operator &= ( const VCharSet &b1 );
    VCharSet& operator |= ( const VCharSet &b1 );
    VCharSet  operator  ~ ();

    friend VCharSet operator & ( const VCharSet &b1, const VCharSet &b2 );
    friend VCharSet operator | ( const VCharSet &b1, const VCharSet &b2 );
*/  
  };

/***************************************************************************
**
** UTILITIES
**
****************************************************************************/

  // split `source' with `regexp_str' regexp
  VArray str_split( const char* regexp_str, const char* source, int maxcount = -1 );
  // split `source' with exact string `delimiter_str'
  VArray str_split_simple( const char* delimiter_str, const char* source, int maxcount = -1 );
  // join array data to single string with `glue' string
  // returns the result string or store to optional `dest'
  VString str_join( VArray array, const char* glue = "" );


/***************************************************************************
**
** MISC FUNCTIONS
**
****************************************************************************/

		
		
#endif /* _VSTRLIB_H_ */

/***************************************************************************
**
** EOF
**
****************************************************************************/

