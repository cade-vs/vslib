/*
 *
 *  VSTRING Library
 *  (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-2002
 *  Distributed under the GPL license, you should receive copy of GPL!
 *
 *  VSTRING library provides vast set of string manipulation features
 *  including dynamic string object that can be freely exchanged with
 *  standard char* type, so there is no need to change function calls
 *  nor the implementation when you change from char* to String (and
 *  vice versa). The main difference from other similar libs is that
 *  the dynamic String class has no visible methods (except operators)
 *  so you will use it as a plain char* but it will expand/shrink as
 *  needed.
 *
 *  Thank you for the attention!
 *  If you find bug or you have note about vstring lib, please feel
 *  free to contact me at:
 *
 *  Vladi Belperchinov-Shabanski "Cade"
 *  <cade@biscom.net>
 *  <cade@datamax.bg>
 *  http://www.biscom.net/~cade
 *
 *  NOTE: vstring is loosely based on `cxstring' lib (c) Ivo Baylov 1998.
 *  NOTE: vstring is distributed standalone as well as a part from vslib.
 *
 * $Id: vstring.h,v 1.8 2002/10/29 04:09:03 cade Exp $
 *
 */

#ifndef _VSTRING_H_
#define _VSTRING_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <assert.h>
#ifndef ASSERT
#define ASSERT assert
#endif

/****************************************************************************
**
** STRING Class
**
****************************************************************************/

  #define STR_BLOCK_SIZE    256

  class String
  {
    int   sl;   // string length
    int   size; // internal buffer size
    char* s;    // internal buffer

    void resize( int newsize );

  public:

    int compact; // set this != 0 for compact (memory preserving) behaviour

    String()                     { s = NULL; sl = size = compact = 0; resize(sl); };
    String( const String& str )  { s = NULL; sl = size = compact = 0; set(str);   };
    String( const char*   ps  )  { s = NULL; sl = size = compact = 0; set(ps);    };
    String( const int     n   )  { s = NULL; sl = size = compact = 0; i(n);    };
    String( const long    n   )  { s = NULL; sl = size = compact = 0; l(n);    };
    String( const double  n   )  { s = NULL; sl = size = compact = 0; f(n);    };
    ~String() { if ( s ) free( s );s = NULL; sl = size = compact = 0; };

    const String& operator = ( const String& str ) { set(str.s); return *this; };
    const String& operator = ( const char*   ps  ) { set(ps); return *this; };
    const String& operator = ( const int     n   ) { i(n); return *this; };
    const String& operator = ( const long    n   ) { l(n); return *this; };
    const String& operator = ( const double  n   ) { f(n); return *this; };

    const String& operator += ( const String& str ) { cat( str.s ); return *this; };
    const String& operator += ( const char*  ps   ) { cat( ps ); return *this; };
    const String& operator += ( const int    n    ) { String tmp = n; cat(tmp); return *this; };
    const String& operator += ( const long   n    ) { String tmp = n; cat(tmp); return *this; };
    const String& operator += ( const double n    ) { String tmp = n; cat(tmp); return *this; };

    friend String operator + ( const String& str1, const String& str2 )
           { String res = str1; res += str2; return res; };
    friend String operator + ( const String& str1, const char* ps )
           { String res = str1; res += ps; return res; };
    friend String operator + ( const char* ps, const String& str2 )
           { String res = ps; res += str2; return res; };

    friend String operator + ( const String& str1, const int    n )
           { String res = str1; res +=    n; return res; };
    friend String operator + ( const int    n, const String& str2 )
           { String res =    n; res += str2; return res; };
    friend String operator + ( const String& str1, const long   n )
           { String res = str1; res +=    n; return res; };
    friend String operator + ( const long   n, const String& str2 )
           { String res =    n; res += str2; return res; };
    friend String operator + ( const String& str1, const double n )
           { String res = str1; res +=    n; return res; };
    friend String operator + ( const double n, const String& str2 )
           { String res =    n; res += str2; return res; };

    friend int operator == ( const String& s1, const String& s2 ) { return strcmp( s1, s2 ) == 0; };
    friend int operator == ( const char*   s1, const String& s2 ) { return strcmp( s1, s2 ) == 0; };
    friend int operator == ( const String& s1, const char*   s2 ) { return strcmp( s1, s2 ) == 0; };

    friend int operator != ( const String& s1, const String& s2 ) { return strcmp( s1, s2 ) != 0; };
    friend int operator != ( const char*   s1, const String& s2 ) { return strcmp( s1, s2 ) != 0; };
    friend int operator != ( const String& s1, const char*   s2 ) { return strcmp( s1, s2 ) != 0; };

    friend int operator >  ( const String& s1, const String& s2 ) { return strcmp( s1, s2 ) >  0; };
    friend int operator >  ( const char*   s1, const String& s2 ) { return strcmp( s1, s2 ) >  0; };
    friend int operator >  ( const String& s1, const char*   s2 ) { return strcmp( s1, s2 ) >  0; };

    friend int operator >= ( const String& s1, const String& s2 ) { return strcmp( s1, s2 ) >= 0; };
    friend int operator >= ( const char*   s1, const String& s2 ) { return strcmp( s1, s2 ) >= 0; };
    friend int operator >= ( const String& s1, const char*   s2 ) { return strcmp( s1, s2 ) >= 0; };

    friend int operator <  ( const String& s1, const String& s2 ) { return strcmp( s1, s2 ) <  0; };
    friend int operator <  ( const char*   s1, const String& s2 ) { return strcmp( s1, s2 ) <  0; };
    friend int operator <  ( const String& s1, const char*   s2 ) { return strcmp( s1, s2 ) <  0; };

    friend int operator <= ( const String& s1, const String& s2 ) { return strcmp( s1, s2 ) <= 0; };
    friend int operator <= ( const char*   s1, const String& s2 ) { return strcmp( s1, s2 ) <= 0; };
    friend int operator <= ( const String& s1, const char*   s2 ) { return strcmp( s1, s2 ) <= 0; };

    operator const char* ( ) const { return (const char*)s; }
    char& operator [] ( int n )
         { ASSERT( n >= 0 && n <= sl ); return s[n]; }

    const char* data() { return s; }

    void fixlen() { sl = strlen(s); ASSERT( sl < size ); };
    void fix() { sl = strlen(s); resize(sl); ASSERT( sl < size ); };

    void   i( const int n );
    void   l( const long n );
    void   f( const double d );
    void   fi( const double d ); // sets double as int (w/o frac)

    int    i() { return atoi( s ); };
    long   l() { return atol( s ); };
    double f() { return atof( s ); };
    double fi() { return atof( s ); };

    void   set ( const char* ps );
    void   cat ( const char* ps );
    void   setn( const char* ps, int len );
    void   catn( const char* ps, int len );

    /* for debugging only */
    int check() { int len = strlen(s); return ((len == sl)&&(len<size)); };

    /****************************************************************************
    ** STRING Friend Functions (for class String)
    ****************************************************************************/

    inline friend int str_len( String& target ) { return target.sl; };
    inline friend String& str_set( String& target, const char* ps ) { target.set( ps ); return target; };

    friend String& str_mul( String& target, int n ); // multiplies the string n times, i.e. `1'*5 = `11111'
    friend String& str_del    ( String& target, int pos, int len ); // deletes `len' chars starting from `pos'
    friend String& str_ins    ( String& target, int pos, const char* s ); // inserts `s' in position `pos'
    friend String& str_ins_ch ( String& target, int pos, char ch ); // inserts `ch' in position `pos'
    friend String& str_replace( String& target, const char* out, const char* in ); // replace `out' w. `in'

    friend String& str_copy  ( String& target, const char* source, int pos, int len ); // returns `len' chars from `pos'
    friend String& str_left  ( String& target, const char* source, int len ); // returns `len' chars from the left
    friend String& str_right ( String& target, const char* source, int len ); // returns `len' chars from the right
    friend String& str_sleft ( String& target, int len ); // SelfLeft -- just as 'Left' but works on `this'
    friend String& str_sright( String& target, int len ); // SelfRight -- just as 'Right' but works on `this'

    friend String& str_trim_left ( String& target, int len ); // trims `len' chars from the beginning (left)
    friend String& str_trim_right( String& target, int len ); // trim `len' chars from the end (right)

    friend String& str_cut_left ( String& target, const char* charlist ); // remove all chars `charlist' from the beginning (i.e. from the left)
    friend String& str_cut_right( String& target, const char* charlist ); // remove all chars `charlist' from the end (i.e. from the right)
    friend String& str_cut      ( String& target, const char* charlist ); // does `CutR(charlist);CutL(charlist);'
    friend String& str_cut_spc  ( String& target ); // does `Cut(" ");'

    friend String& str_pad  ( String& target, int len, char ch = ' ' );
    friend String& str_comma( String& target, char delim = ',' );

    // next 3 functions are safe! so if you get/set out f the string range!
    friend void str_set_ch( String& target, int pos, const char ch ); // sets `ch' char at position `pos'
    friend char str_get_ch( String& target, int pos ); // return char at position `pos', -1 for the last char etc...
    friend void str_add_ch( String& target, const char ch ); // adds `ch' at the end

    friend char*  str_word( String& target, const char* delimiters, char* result );
    friend char*  str_rword( String& target, const char* delimiters, char* result );

    // this `sprintf'-like function works as follows:
    // 1. set `this.string' length to `init_size'
    // 2. call `sprintf' with `format' and `...'
    // NOTE: You have to supply enough `init_size'! sorry...
    friend int sprintf( int init_size, String& target, const char *format, ... );
    // this is equal to `printf( 1024, format, ... )', i.e. `init_size=1024'
    friend int sprintf( String& target, const char *format, ... );

    friend String& str_tr ( String& target, const char *from, const char *to );
    friend String& str_up ( String& target );
    friend String& str_low( String& target );
    friend String& str_flip_case( String& target );
    
    friend String& str_reverse  ( String& target ); // reverse the string: `abcde' becomes `edcba'
    friend String& str_squeeze( String& target, const char* sq_chars ); // squeeze repeating chars to one only

  }; /* end of String class */

/****************************************************************************
**
** STRING Functions (for class String)
**
****************************************************************************/

/****************************************************************************
**
** STRING Functions (for char*)
**
****************************************************************************/

  inline int str_len( const char* ps ) { return strlen( ps ); };
  inline char* str_set( char* target, const char* ps ) { target[0] = 0; if (ps) strcpy( target, ps ); strcpy( target, ps ); return target; };

  char* str_mul( char* target, int n ); // multiplies the string n times, i.e. `1'*5 = `11111'

  char* str_del    ( char* target, int pos, int len ); // deletes `len' chars starting from `pos'
  char* str_ins    ( char* target, int pos, const char* s ); // inserts `s' in position `pos'
  char* str_ins_ch ( char* target, int pos, char ch ); // inserts `ch' in position `pos'
  char* str_replace( char* target, const char* out, const char* in ); // replace `out' w. `in'

  char* str_copy  ( char* target, const char* source, int pos, int len ); // returns `len' chars from `pos'
  char* str_left  ( char* target, const char* source, int len ); // returns `len' chars from the left
  char* str_right ( char* target, const char* source, int len ); // returns `len' chars from the right
  char* str_sleft ( char* target, int len ); // just as 'left' but works on `target'
  char* str_sright( char* target, int len ); // just as 'right' but works on `target'

  char* str_trim_left ( char* target, int len ); // trims `len' chars from the beginning (left)
  char* str_trim_right( char* target, int len ); // trim `len' chars from the end (right)

  // next 3 functions are safe! so if you get/set out f the string range!
  // (note: that `char*' funcs are slower because of initial strlen() check
  void str_set_ch( char* target, int pos, const char ch ); // sets `ch' char at position `pos'
  char str_get_ch( char* target, int pos ); // return char at position `pos', -1 for the last char etc...
  void str_add_ch( char* target, const char ch ); // adds `ch' at the end

  // return first `word', i.e. from pos 0 to first found delimiter char
  // after that deletes this `word' from the string
  char* str_word ( char* target, const char* delimiters, char* result );
  // ...same but `last' word reverse/rear
  char* str_rword( char* target, const char* delimiters, char* result );

  char* str_cut_left ( char* target, const char* charlist ); // remove all chars `charlist' from the beginning (i.e. from the left)
  char* str_cut_right( char* target, const char* charlist ); // remove all chars `charlist' from the end (i.e. from the right)
  char* str_cut      ( char* target, const char* charlist ); // does `CutR(charlist);CutL(charlist);'
  char* str_cut_spc  ( char* target ); // does `Cut(" ");'

  // expand align in a field, filled w. `ch', if len > 0 then right, else left
  char* str_pad( char* target, int len, char ch = ' ' );

  // insert `commas' for 1000's delimiter or use another delimiter
  // string supposed to be a integer or real w/o `e' format
  char* str_comma( char* target, char delim = ',' );

  // translate chars from `from' to `to'
  // length of `from' MUST be equal to length of `to'
  char* str_tr( char* target, const char *from, const char *to );

  char* str_up ( char* target );
  char* str_low( char* target );
  char* str_flip_case( char* target );

  char* str_reverse( char* target ); // reverse the string: `abcde' becomes `edcba'

  char* str_squeeze( char* target, const char* sq_chars ); // squeeze repeating chars to one only

/****************************************************************************
**
** STRING Functions -- common (String class will pass transparently)
**
****************************************************************************/

  int str_find ( const char* target, const char  c, int startpos = 0 ); // returns first zero-based position of char, or -1 if not found
  int str_rfind( const char* target, const char  c ); // returns last zero-based position of char, or -1 if not found
  int str_find ( const char* target, const char* s, int startpos = 0 ); // returns first zero-based position of string, or -1 if not found
  int str_rfind( const char* target, const char* s ); // returns last zero-based position of string, or -1 if not found

  int str_count( const char* target, const char* charlist, int startpos = 0 ); // returns match count of all chars from `charlist'
  int str_str_count( const char* target, const char* s, int startpos = 0 ); // returns match count of `s' string into target

  int str_is_int   ( const char* target ); // check if string is correct int value
  int str_is_double( const char* target ); // check if string is correct double (w/o `e' format :( )

/****************************************************************************
**
** STRING Utility classes
**
****************************************************************************/

/* StrSplitter was removed, see VArray::split() in vstrlib.h */

/****************************************************************************
**
** STRING Utility functions
**
****************************************************************************/

  /* str_chop() removes last char from a string (perl-like) */
  inline char* str_chop( char* target ) { return str_trim_right( target, 1 ); };
  inline String& str_chop( String& target ) { return str_trim_right( target, 1 ); };

  /* reduces string to the given width using dots:
     `this is long line' -> `this...ine'
     `s' can be NULL, then target will be reduced */
  char*	str_dot_reduce( const char* s, char* dest, int width );
  String& str_dot_reduce( const char* s, String& dest, int width );

/****************************************************************************
**
** STRING file names utilities -- functions and classes
** NOTE: does not use any external function calls!
**
****************************************************************************/

// adds trailing '/' if not exist
char* str_fix_path( char* s, int slashtype = '/' );
const char* str_fix_path( String& s, int slashtype = '/' );

// following func's return from `/path/filename.ext'
char* str_file_ext( const char *ps, char *ext );       /* `ext'            */
char* str_file_name( const char *ps, char *name );     /* `filename'       */
char* str_file_name_ext( const char *ps, char *name ); /* `filename.ext'   */
char* str_file_path( const char *ps, char *path );     /* `/path/'         */

String& str_file_ext( const char *ps, String& ext );       /* `ext'            */
String& str_file_name( const char *ps, String& name );     /* `filename'       */
String& str_file_name_ext( const char *ps, String& name ); /* `filename.ext'   */
String& str_file_path( const char *ps, String& path );     /* `/path/'         */

/* removes "/../"s, `path' can be NULL, then dest is fixed */
char* str_reduce_path( const char* path, char* dest );
String& str_reduce_path( const char* path, String& dest );


/****************************************************************************
**
** STRING Conversions
**
****************************************************************************/

long hex2long( const char* s ); // hex to long

#ifndef htol
#define htol
#endif

char*  time2str( const time_t tim );
time_t str2time( const char* timstr );

/***************************************************************************
** EOF
***************************************************************************/
#endif

