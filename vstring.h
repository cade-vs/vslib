/*
 *
 *  VSTRING Library
 *  (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-2000
 *  Distributed under the GPL license, see end of this file for full text!
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
 * $Id: vstring.h,v 1.2 2001/10/28 13:53:02 cade Exp $
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
    String( const int     n   )  { s = NULL; sl = size = compact = 0; seti(n);    };
    String( const long    n   )  { s = NULL; sl = size = compact = 0; setl(n);    };
    String( const double  n   )  { s = NULL; sl = size = compact = 0; setf(n);    };
    ~String() { if ( s ) delete s; s = NULL; sl = size = compact = 0; };

    const String& operator = ( const String& str ) { set(str.s); return *this; };
    const String& operator = ( const char*   ps  ) { set(ps); return *this; };
    const String& operator = ( const int     n   ) { seti(n); return *this; };
    const String& operator = ( const long    n   ) { setl(n); return *this; };
    const String& operator = ( const double  n   ) { setf(n); return *this; };

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

    void   seti( const int n );
    void   setl( const long n );
    void   setf( const double d );
    void   setfi( const double d ); // sets double as int (w/o frac)

    int    geti() { return atoi( s ); };
    long   getl() { return atol( s ); };
    double getf() { return atof( s ); };
    double getfi() { return atof( s ); };

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
  char* str_sleft ( char* target, int len ); // SelfLeft -- just as 'Left' but works on `this'
  char* str_sright( char* target, int len ); // SelfRight -- just as 'Right' but works on `this'

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

  class StrSplitter
  {
    String m_delimiter;
    int m_max_words;
    int m_words;
    char *m_str;
    char **m_strarr;

    public:
    StrSplitter( const char* a_delimiter );
    ~StrSplitter();

    int set( const char* a_target );
	void set_delimiter( const char* a_delimiter ) { m_delimiter = a_delimiter; };
	
    void operator = ( const String& str ) { set( (const char*)(str) ); };
    void operator = ( const char*   ps  ) { set( ps ); };

    const char* operator [] ( int n )
       { if( n >= 0 && n < m_words && m_strarr ) return m_strarr[n]; else return NULL; }

    int count() { return m_words; }

    /* get top element and remove it from the list */
    String& pop( String& str );
    char* pop( char* ps );
  };

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

/*
-----------------------------------------------------------COPYING----------

            GNU GENERAL PUBLIC LICENSE
		       Version 2, June 1991

 Copyright (C) 1989, 1991 Free Software Foundation, Inc.
                          675 Mass Ave, Cambridge, MA 02139, USA
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.

			    Preamble

  The licenses for most software are designed to take away your
freedom to share and change it.  By contrast, the GNU General Public
License is intended to guarantee your freedom to share and change free
software--to make sure the software is free for all its users.  This
General Public License applies to most of the Free Software
Foundation's software and to any other program whose authors commit to
using it.  (Some other Free Software Foundation software is covered by
the GNU Library General Public License instead.)  You can apply it to
your programs, too.

  When we speak of free software, we are referring to freedom, not
price.  Our General Public Licenses are designed to make sure that you
have the freedom to distribute copies of free software (and charge for
this service if you wish), that you receive source code or can get it
if you want it, that you can change the software or use pieces of it
in new free programs; and that you know you can do these things.

  To protect your rights, we need to make restrictions that forbid
anyone to deny you these rights or to ask you to surrender the rights.
These restrictions translate to certain responsibilities for you if you
distribute copies of the software, or if you modify it.

  For example, if you distribute copies of such a program, whether
gratis or for a fee, you must give the recipients all the rights that
you have.  You must make sure that they, too, receive or can get the
source code.  And you must show them these terms so they know their
rights.

  We protect your rights with two steps: (1) copyright the software, and
(2) offer you this license which gives you legal permission to copy,
distribute and/or modify the software.

  Also, for each author's protection and ours, we want to make certain
that everyone understands that there is no warranty for this free
software.  If the software is modified by someone else and passed on, we
want its recipients to know that what they have is not the original, so
that any problems introduced by others will not reflect on the original
authors' reputations.

  Finally, any free program is threatened constantly by software
patents.  We wish to avoid the danger that redistributors of a free
program will individually obtain patent licenses, in effect making the
program proprietary.  To prevent this, we have made it clear that any
patent must be licensed for everyone's free use or not licensed at all.

  The precise terms and conditions for copying, distribution and
modification follow.

		    GNU GENERAL PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. This License applies to any program or other work which contains
a notice placed by the copyright holder saying it may be distributed
under the terms of this General Public License.  The "Program", below,
refers to any such program or work, and a "work based on the Program"
means either the Program or any derivative work under copyright law:
that is to say, a work containing the Program or a portion of it,
either verbatim or with modifications and/or translated into another
language.  (Hereinafter, translation is included without limitation in
the term "modification".)  Each licensee is addressed as "you".

Activities other than copying, distribution and modification are not
covered by this License; they are outside its scope.  The act of
running the Program is not restricted, and the output from the Program
is covered only if its contents constitute a work based on the
Program (independent of having been made by running the Program).
Whether that is true depends on what the Program does.

  1. You may copy and distribute verbatim copies of the Program's
source code as you receive it, in any medium, provided that you
conspicuously and appropriately publish on each copy an appropriate
copyright notice and disclaimer of warranty; keep intact all the
notices that refer to this License and to the absence of any warranty;
and give any other recipients of the Program a copy of this License
along with the Program.

You may charge a fee for the physical act of transferring a copy, and
you may at your option offer warranty protection in exchange for a fee.

  2. You may modify your copy or copies of the Program or any portion
of it, thus forming a work based on the Program, and copy and
distribute such modifications or work under the terms of Section 1
above, provided that you also meet all of these conditions:

    a) You must cause the modified files to carry prominent notices
    stating that you changed the files and the date of any change.

    b) You must cause any work that you distribute or publish, that in
    whole or in part contains or is derived from the Program or any
    part thereof, to be licensed as a whole at no charge to all third
    parties under the terms of this License.

    c) If the modified program normally reads commands interactively
    when run, you must cause it, when started running for such
    interactive use in the most ordinary way, to print or display an
    announcement including an appropriate copyright notice and a
    notice that there is no warranty (or else, saying that you provide
    a warranty) and that users may redistribute the program under
    these conditions, and telling the user how to view a copy of this
    License.  (Exception: if the Program itself is interactive but
    does not normally print such an announcement, your work based on
    the Program is not required to print an announcement.)

These requirements apply to the modified work as a whole.  If
identifiable sections of that work are not derived from the Program,
and can be reasonably considered independent and separate works in
themselves, then this License, and its terms, do not apply to those
sections when you distribute them as separate works.  But when you
distribute the same sections as part of a whole which is a work based
on the Program, the distribution of the whole must be on the terms of
this License, whose permissions for other licensees extend to the
entire whole, and thus to each and every part regardless of who wrote it.

Thus, it is not the intent of this section to claim rights or contest
your rights to work written entirely by you; rather, the intent is to
exercise the right to control the distribution of derivative or
collective works based on the Program.

In addition, mere aggregation of another work not based on the Program
with the Program (or with a work based on the Program) on a volume of
a storage or distribution medium does not bring the other work under
the scope of this License.

  3. You may copy and distribute the Program (or a work based on it,
under Section 2) in object code or executable form under the terms of
Sections 1 and 2 above provided that you also do one of the following:

    a) Accompany it with the complete corresponding machine-readable
    source code, which must be distributed under the terms of Sections
    1 and 2 above on a medium customarily used for software interchange; or,

    b) Accompany it with a written offer, valid for at least three
    years, to give any third party, for a charge no more than your
    cost of physically performing source distribution, a complete
    machine-readable copy of the corresponding source code, to be
    distributed under the terms of Sections 1 and 2 above on a medium
    customarily used for software interchange; or,

    c) Accompany it with the information you received as to the offer
    to distribute corresponding source code.  (This alternative is
    allowed only for noncommercial distribution and only if you
    received the program in object code or executable form with such
    an offer, in accord with Subsection b above.)

The source code for a work means the preferred form of the work for
making modifications to it.  For an executable work, complete source
code means all the source code for all modules it contains, plus any
associated interface definition files, plus the scripts used to
control compilation and installation of the executable.  However, as a
special exception, the source code distributed need not include
anything that is normally distributed (in either source or binary
form) with the major components (compiler, kernel, and so on) of the
operating system on which the executable runs, unless that component
itself accompanies the executable.

If distribution of executable or object code is made by offering
access to copy from a designated place, then offering equivalent
access to copy the source code from the same place counts as
distribution of the source code, even though third parties are not
compelled to copy the source along with the object code.

  4. You may not copy, modify, sublicense, or distribute the Program
except as expressly provided under this License.  Any attempt
otherwise to copy, modify, sublicense or distribute the Program is
void, and will automatically terminate your rights under this License.
However, parties who have received copies, or rights, from you under
this License will not have their licenses terminated so long as such
parties remain in full compliance.

  5. You are not required to accept this License, since you have not
signed it.  However, nothing else grants you permission to modify or
distribute the Program or its derivative works.  These actions are
prohibited by law if you do not accept this License.  Therefore, by
modifying or distributing the Program (or any work based on the
Program), you indicate your acceptance of this License to do so, and
all its terms and conditions for copying, distributing or modifying
the Program or works based on it.

  6. Each time you redistribute the Program (or any work based on the
Program), the recipient automatically receives a license from the
original licensor to copy, distribute or modify the Program subject to
these terms and conditions.  You may not impose any further
restrictions on the recipients' exercise of the rights granted herein.
You are not responsible for enforcing compliance by third parties to
this License.

  7. If, as a consequence of a court judgment or allegation of patent
infringement or for any other reason (not limited to patent issues),
conditions are imposed on you (whether by court order, agreement or
otherwise) that contradict the conditions of this License, they do not
excuse you from the conditions of this License.  If you cannot
distribute so as to satisfy simultaneously your obligations under this
License and any other pertinent obligations, then as a consequence you
may not distribute the Program at all.  For example, if a patent
license would not permit royalty-free redistribution of the Program by
all those who receive copies directly or indirectly through you, then
the only way you could satisfy both it and this License would be to
refrain entirely from distribution of the Program.

If any portion of this section is held invalid or unenforceable under
any particular circumstance, the balance of the section is intended to
apply and the section as a whole is intended to apply in other
circumstances.

It is not the purpose of this section to induce you to infringe any
patents or other property right claims or to contest validity of any
such claims; this section has the sole purpose of protecting the
integrity of the free software distribution system, which is
implemented by public license practices.  Many people have made
generous contributions to the wide range of software distributed
through that system in reliance on consistent application of that
system; it is up to the author/donor to decide if he or she is willing
to distribute software through any other system and a licensee cannot
impose that choice.

This section is intended to make thoroughly clear what is believed to
be a consequence of the rest of this License.

  8. If the distribution and/or use of the Program is restricted in
certain countries either by patents or by copyrighted interfaces, the
original copyright holder who places the Program under this License
may add an explicit geographical distribution limitation excluding
those countries, so that distribution is permitted only in or among
countries not thus excluded.  In such case, this License incorporates
the limitation as if written in the body of this License.

  9. The Free Software Foundation may publish revised and/or new versions
of the General Public License from time to time.  Such new versions will
be similar in spirit to the present version, but may differ in detail to
address new problems or concerns.

Each version is given a distinguishing version number.  If the Program
specifies a version number of this License which applies to it and "any
later version", you have the option of following the terms and conditions
either of that version or of any later version published by the Free
Software Foundation.  If the Program does not specify a version number of
this License, you may choose any version ever published by the Free Software
Foundation.

  10. If you wish to incorporate parts of the Program into other free
programs whose distribution conditions are different, write to the author
to ask for permission.  For software which is copyrighted by the Free
Software Foundation, write to the Free Software Foundation; we sometimes
make exceptions for this.  Our decision will be guided by the two goals
of preserving the free status of all derivatives of our free software and
of promoting the sharing and reuse of software generally.

			    NO WARRANTY

  11. BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY
FOR THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN
OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES
PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS
TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
REPAIR OR CORRECTION.

  12. IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING
WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR
REDISTRIBUTE THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES,
INCLUDING ANY GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING
OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED
TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY
YOU OR THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER
PROGRAMS), EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE
POSSIBILITY OF SUCH DAMAGES.

		     END OF TERMS AND CONDITIONS

	    How to Apply These Terms to Your New Programs

  If you develop a new program, and you want it to be of the greatest
possible use to the public, the best way to achieve this is to make it
free software which everyone can redistribute and change under these terms.

  To do so, attach the following notices to the program.  It is safest
to attach them to the start of each source file to most effectively
convey the exclusion of warranty; and each file should have at least
the "copyright" line and a pointer to where the full notice is found.

    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 19yy  <name of author>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

Also add information on how to contact you by electronic and paper mail.

If the program is interactive, make it output a short notice like this
when it starts in an interactive mode:

    Gnomovision version 69, Copyright (C) 19yy name of author
    Gnomovision comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
    This is free software, and you are welcome to redistribute it
    under certain conditions; type `show c' for details.

The hypothetical commands `show w' and `show c' should show the appropriate
parts of the General Public License.  Of course, the commands you use may
be called something other than `show w' and `show c'; they could even be
mouse-clicks or menu items--whatever suits your program.

You should also get your employer (if you work as a programmer) or your
school, if any, to sign a "copyright disclaimer" for the program, if
necessary.  Here is a sample; alter the names:

  Yoyodyne, Inc., hereby disclaims all copyright interest in the program
  `Gnomovision' (which makes passes at compilers) written by James Hacker.

  <signature of Ty Coon>, 1 April 1989
  Ty Coon, President of Vice

This General Public License does not permit incorporating your program into
proprietary programs.  If your program is a subroutine library, you may
consider it more useful to permit linking proprietary applications with the
library.  If this is what you want to do, use the GNU Library General
Public License instead of this License.

-----------------------------------------------------------COPYING-end------
*/

