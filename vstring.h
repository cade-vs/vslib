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
 *  http://soul.datamax.bg/~cade
 *
 *  NOTE: vstring was initially (and loosely) based on 
 *        `cxstring' lib (c) Ivo Baylov 1998.
 *  NOTE: vstring is distributed standalone as well as a part from vslib.
 *
 *  This file (vstring.h and vstring.cpp) implements plain string-only 
 *  manipulations. For further functionality see vstrlib.h and vstrlib.cpp.
 *
 *  $Id: vstring.h,v 1.15 2003/01/19 16:14:54 cade Exp $
 *
 */

#ifndef _VSTRING_H_
#define _VSTRING_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>
#ifndef ASSERT
#define ASSERT assert
#endif

#define String VString

/***************************************************************************
**
** VREF
**
****************************************************************************/

class VRef
{
  int _ref;
  
public:

  VRef() { _ref = 1; }  // creator get first reference
  virtual ~VRef() { ASSERT( _ref == 0 ); }

  void ref() { _ref++; }
  void unref() { ASSERT( _ref > 0 ); _ref--; if ( _ref == 0 ) delete this; }
  
  int refs() { return _ref; }
};

/****************************************************************************
**
** VSTRING BOX
**
****************************************************************************/

class VStringBox: public VRef
{
public:

  int   sl;   // string buffer length
  int   size; // internal buffer size
  char* s;    // internal buffer

  int   compact;

  VStringBox() { s = NULL; sl = size = compact = 0; };
  ~VStringBox() { undef(); };
  
  VStringBox* clone();
  
  void resize_buf( int new_size );
  void undef() { resize_buf( 0 ); };
};


/****************************************************************************
**
** VSTRING
**
****************************************************************************/

#define STR_BLOCK_SIZE    256

class VString
{
  VStringBox* box;
  char retch; // used to return char& for off-range char index

  void detach();

public:

  VString( const VString& str )
    {
    box = str.box;
    box->ref();
    };
  
  VString()                      {  box = new VStringBox(); };
  VString( const char*    ps  )  {  box = new VStringBox(); set( ps);  };
  VString( const int      n   )  {  box = new VStringBox(); i(n);     };
  VString( const long     n   )  {  box = new VStringBox(); l(n);     };
  VString( const double   n   )  {  box = new VStringBox(); f(n);     };
  ~VString() { box->unref(); };

  void compact( int a_compact ) // set this != 0 for compact (memory preserving) behaviour
    { box->compact = a_compact; }; //FIXME: detach() first?

  void resize( int new_size ) { detach(); box->resize_buf( new_size ); };

  const VString& operator  = ( const VString& str )
    {
    box->unref();
    box = str.box;
    box->ref();
    return *this; 
    };
  
  const VString& operator  = ( const char*   ps   ) { set(ps);return *this; };
  const VString& operator  = ( const int     n    ) { i(n);   return *this; };
  const VString& operator  = ( const long    n    ) { l(n);   return *this; };
  const VString& operator  = ( const double  n    ) { f(n);   return *this; };

  const VString& operator += ( const VString& str ) 
        { cat( str.box->s ); return *this; };
  const VString& operator += ( const char*  ps    ) 
        { cat( ps ); return *this; };
  const VString& operator += ( const int    n     ) 
        { VString tmp = n; cat(tmp); return *this; };
  const VString& operator += ( const long   n     ) 
        { VString tmp = n; cat(tmp); return *this; };
  const VString& operator += ( const double n     ) 
        { VString tmp = n; cat(tmp); return *this; };

  const VString& operator *= ( const int    n     ) 
        { return str_mul( *this, n ); };

  friend VString operator + ( const VString& str1, const VString& str2 )
         { VString res = str1; res += str2; return res; };
  friend VString operator + ( const VString& str1, const char* ps )
         { VString res = str1; res += ps; return res; };
  friend VString operator + ( const char* ps, const VString& str2 )
         { VString res = ps; res += str2; return res; };

  friend VString operator + ( const VString& str1, const int    n )
         { VString res = str1; res +=    n; return res; };
  friend VString operator + ( const int    n, const VString& str2 )
         { VString res =    n; res += str2; return res; };
  friend VString operator + ( const VString& str1, const long   n )
         { VString res = str1; res +=    n; return res; };
  friend VString operator + ( const long   n, const VString& str2 )
         { VString res =    n; res += str2; return res; };
  friend VString operator + ( const VString& str1, const double n )
         { VString res = str1; res +=    n; return res; };
  friend VString operator + ( const double n, const VString& str2 )
         { VString res =    n; res += str2; return res; };

  friend int operator == ( const VString& s1, const VString& s2 ) { return strcmp( s1, s2 ) == 0; };
  friend int operator == ( const char*    s1, const VString& s2 ) { return strcmp( s1, s2 ) == 0; };
  friend int operator == ( const VString& s1, const char*    s2 ) { return strcmp( s1, s2 ) == 0; };

  friend int operator != ( const VString& s1, const VString& s2 ) { return strcmp( s1, s2 ) != 0; };
  friend int operator != ( const char*    s1, const VString& s2 ) { return strcmp( s1, s2 ) != 0; };
  friend int operator != ( const VString& s1, const char*    s2 ) { return strcmp( s1, s2 ) != 0; };

  friend int operator >  ( const VString& s1, const VString& s2 ) { return strcmp( s1, s2 ) >  0; };
  friend int operator >  ( const char*    s1, const VString& s2 ) { return strcmp( s1, s2 ) >  0; };
  friend int operator >  ( const VString& s1, const char*    s2 ) { return strcmp( s1, s2 ) >  0; };

  friend int operator >= ( const VString& s1, const VString& s2 ) { return strcmp( s1, s2 ) >= 0; };
  friend int operator >= ( const char*    s1, const VString& s2 ) { return strcmp( s1, s2 ) >= 0; };
  friend int operator >= ( const VString& s1, const char*    s2 ) { return strcmp( s1, s2 ) >= 0; };

  friend int operator <  ( const VString& s1, const VString& s2 ) { return strcmp( s1, s2 ) <  0; };
  friend int operator <  ( const char*    s1, const VString& s2 ) { return strcmp( s1, s2 ) <  0; };
  friend int operator <  ( const VString& s1, const char*    s2 ) { return strcmp( s1, s2 ) <  0; };

  friend int operator <= ( const VString& s1, const VString& s2 ) { return strcmp( s1, s2 ) <= 0; };
  friend int operator <= ( const char*    s1, const VString& s2 ) { return strcmp( s1, s2 ) <= 0; };
  friend int operator <= ( const VString& s1, const char*    s2 ) { return strcmp( s1, s2 ) <= 0; };

  operator const char* ( ) const { return (const char*)box->s; }
  const char* data() { return box->s; }
  
  char& operator [] ( int n )
      {
      if ( n < 0 ) n = box->sl + n;
      if ( n < 0 || n >= box->sl ) 
        {
        retch = 0;
        return retch;
        }
      detach();  
      return box->s[n]; 
      }

  void fixlen() 
       { box->sl = strlen(box->s); 
         ASSERT( box->sl ? box->sl < box->size : box->sl == box->size ); };
  void fix() 
       { box->sl = strlen(box->s); 
         box->resize_buf(box->sl); 
         ASSERT( box->sl ? box->sl < box->size : box->sl == box->size ); };

  void   i( const int n );
  void   l( const long n );
  void   f( const double d );
  void   fi( const double d ); // sets double as int (w/o frac)

  int    i() { return atoi( box->s ); };
  long   l() { return atol( box->s ); };
  double f() { return atof( box->s ); };
  double fi() { return atof( box->s ); };

  void   set( const char* ps );
  void   cat( const char* ps );
  void   setn( const char* ps, int len );
  void   catn( const char* ps, int len );

  /* for debugging only */
  int check() { int len = strlen(box->s); return ((len == box->sl)&&(len<box->size)); };

  /****************************************************************************
  ** VString Friend Functions (for class VString)
  ****************************************************************************/

  inline friend int str_len( VString& target ) { return target.box->sl; };
  inline friend VString& str_set( VString& target, const char* ps ) { target.set( ps ); return target; };

  friend VString& str_mul( VString& target, int n ); // multiplies the VString n times, i.e. `1'*5 = `11111'
  friend VString& str_del    ( VString& target, int pos, int len ); // deletes `len' chars starting from `pos'
  friend VString& str_ins    ( VString& target, int pos, const char* s ); // inserts `s' in position `pos'
  friend VString& str_ins_ch ( VString& target, int pos, char ch ); // inserts `ch' in position `pos'
  friend VString& str_replace( VString& target, const char* out, const char* in ); // replace `out' w. `in'

  friend VString& str_copy  ( VString& target, const char* source, int pos, int len = -1 ); // returns `len' chars from `pos'
  friend VString& str_left  ( VString& target, const char* source, int len ); // returns `len' chars from the left
  friend VString& str_right ( VString& target, const char* source, int len ); // returns `len' chars from the right
  friend VString& str_sleft ( VString& target, int len ); // SelfLeft -- just as 'Left' but works on `this'
  friend VString& str_sright( VString& target, int len ); // SelfRight -- just as 'Right' but works on `this'

  friend VString& str_trim_left ( VString& target, int len ); // trims `len' chars from the beginning (left)
  friend VString& str_trim_right( VString& target, int len ); // trim `len' chars from the end (right)

  friend VString& str_cut_left ( VString& target, const char* charlist ); // remove all chars `charlist' from the beginning (i.e. from the left)
  friend VString& str_cut_right( VString& target, const char* charlist ); // remove all chars `charlist' from the end (i.e. from the right)
  friend VString& str_cut      ( VString& target, const char* charlist ); // does `CutR(charlist);CutL(charlist);'
  friend VString& str_cut_spc  ( VString& target ); // does `Cut(" ");'

  friend VString& str_pad  ( VString& target, int len, char ch = ' ' );
  friend VString& str_comma( VString& target, char delim = ',' );

  // next 3 functions are safe! so if you get/set out f the VString range!
  friend void str_set_ch( VString& target, int pos, const char ch ); // sets `ch' char at position `pos'
  friend char str_get_ch( VString& target, int pos ); // return char at position `pos', -1 for the last char etc...
  friend void str_add_ch( VString& target, const char ch ); // adds `ch' at the end

  friend char*  str_word( VString& target, const char* delimiters, char* result );
  friend char*  str_rword( VString& target, const char* delimiters, char* result );
  // check VArray::split() instead of word() funtions...
  
  // this `sprintf'-like function works as follows:
  // 1. set `this.VString' length to `init_size'
  // 2. call `sprintf' with `format' and `...'
  // NOTE: You have to supply enough `init_size'! sorry...
  friend int sprintf( int init_size, VString& target, const char *format, ... );
  // this is equal to `printf( 1024, format, ... )', i.e. `init_size=1024'
  friend int sprintf( VString& target, const char *format, ... );

  friend VString& str_tr ( VString& target, const char *from, const char *to );
  friend VString& str_up ( VString& target );
  friend VString& str_low( VString& target );
  friend VString& str_flip_case( VString& target );
  
  friend VString& str_reverse  ( VString& target ); // reverse the VString: `abcde' becomes `edcba'
  friend VString& str_squeeze( VString& target, const char* sq_chars ); // squeeze repeating chars to one only

}; /* end of VString class */

/****************************************************************************
**
** VString Functions (for class VString)
**
****************************************************************************/

/****************************************************************************
**
** VString Functions (for char*)
**
****************************************************************************/

  inline int str_len( const char* ps ) { return strlen( ps ); };
  inline char* str_set( char* target, const char* ps ) { target[0] = 0; if (ps) strcpy( target, ps ); strcpy( target, ps ); return target; };

  char* str_mul( char* target, int n ); // multiplies the VString n times, i.e. `1'*5 = `11111'

  char* str_del    ( char* target, int pos, int len ); // deletes `len' chars starting from `pos'
  char* str_ins    ( char* target, int pos, const char* s ); // inserts `s' in position `pos'
  char* str_ins_ch ( char* target, int pos, char ch ); // inserts `ch' in position `pos'
  char* str_replace( char* target, const char* out, const char* in ); // replace `out' w. `in'

  char* str_copy  ( char* target, const char* source, int pos, int len = -1 ); // returns `len' chars from `pos'
  char* str_left  ( char* target, const char* source, int len ); // returns `len' chars from the left
  char* str_right ( char* target, const char* source, int len ); // returns `len' chars from the right
  char* str_sleft ( char* target, int len ); // just as 'left' but works on `target'
  char* str_sright( char* target, int len ); // just as 'right' but works on `target'

  char* str_trim_left ( char* target, int len ); // trims `len' chars from the beginning (left)
  char* str_trim_right( char* target, int len ); // trim `len' chars from the end (right)

  // next 3 functions are safe! so if you get/set out f the VString range!
  // (note: that `char*' funcs are slower because of initial strlen() check
  void str_set_ch( char* target, int pos, const char ch ); // sets `ch' char at position `pos'
  char str_get_ch( char* target, int pos ); // return char at position `pos', -1 for the last char etc...
  void str_add_ch( char* target, const char ch ); // adds `ch' at the end

  // return first `word', i.e. from pos 0 to first found delimiter char
  // after that deletes this `word' from the VString
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
  // VString supposed to be a integer or real w/o `e' format
  char* str_comma( char* target, char delim = ',' );

  // translate chars from `from' to `to'
  // length of `from' MUST be equal to length of `to'
  char* str_tr( char* target, const char *from, const char *to );

  char* str_up ( char* target );
  char* str_low( char* target );
  char* str_flip_case( char* target );

  char* str_reverse( char* target ); // reverse the VString: `abcde' becomes `edcba'

  char* str_squeeze( char* target, const char* sq_chars ); // squeeze repeating chars to one only

/****************************************************************************
**
** VString Functions -- common (VString class will pass transparently)
**
****************************************************************************/

  int str_find ( const char* target, const char  c, int startpos = 0 ); // returns first zero-based position of char, or -1 if not found
  int str_rfind( const char* target, const char  c ); // returns last zero-based position of char, or -1 if not found
  int str_find ( const char* target, const char* s, int startpos = 0 ); // returns first zero-based position of VString, or -1 if not found
  int str_rfind( const char* target, const char* s ); // returns last zero-based position of VString, or -1 if not found

  int str_count( const char* target, const char* charlist, int startpos = 0 ); // returns match count of all chars from `charlist'
  int str_str_count( const char* target, const char* s, int startpos = 0 ); // returns match count of `s' VString into target

  int str_is_int   ( const char* target ); // check if VString is correct int value
  int str_is_double( const char* target ); // check if VString is correct double (w/o `e' format :( )

/****************************************************************************
**
** VString Utility classes
**
****************************************************************************/

/* StrSplitter was removed, see VArray::split() in vstrlib.h */

/****************************************************************************
**
** VString Utility functions
**
****************************************************************************/

/* str_chop() removes last char from a VString (perl-like) */
inline char* str_chop( char* target ) { return str_trim_right( target, 1 ); };
inline VString& str_chop( VString& target ) { return str_trim_right( target, 1 ); };

/* reduces VString to the given width using dots:
   `this is long line' -> `this...ine'
   `s' can be NULL, then target will be reduced */
VString str_dot_reduce( const char* s, int width );

/****************************************************************************
**
** VString file names utilities -- functions and classes
** NOTE: does not use any external (outside this library) function calls!
**
****************************************************************************/

// adds trailing '/' if not exist
char* str_fix_path( char* s, int slashtype = '/' );
const char* str_fix_path( VString& s, int slashtype = '/' );

VString str_file_ext( const char *ps );       /* `ext'            */
VString str_file_name( const char *ps );     /* `filename'       */
VString str_file_name_ext( const char *ps ); /* `filename.ext'   */
VString str_file_path( const char *ps );     /* `/path/'         */

/* removes "/../"s, `path' can be NULL, then dest is fixed */
VString str_reduce_path( const char* path );

/****************************************************************************
**
** VString Conversions
**
****************************************************************************/

long hex2long( const char* s ); // hex to long

#endif /* _VSTRING_H_ */

/***************************************************************************
**
** EOF
**
****************************************************************************/


