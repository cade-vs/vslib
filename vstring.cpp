/*
 *
 *  VSTRING Library
 *  (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-2000
 *  Distributed under the GPL license, see end of this file for full text!
 *
 *  SEE vstring.h FOR FURTHER INFORMATION AND CREDITS
 *
 * $Id: vstring.cpp,v 1.4 2002/04/14 10:14:44 cade Exp $
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "vstring.h"

#ifndef _HAVE_ITOA_
#define itoa(n,s,r) sprintf(s,"%d",n)
#endif

/****************************************************************************
**
** STRING Class
**
****************************************************************************/

  void String::resize( int newsize )
  {
    newsize++; /* for the trailing 0 */
    if ( !compact )
      {
      newsize = newsize / STR_BLOCK_SIZE  + ( newsize % STR_BLOCK_SIZE != 0 );
      newsize *= STR_BLOCK_SIZE;
      }
    if( s == NULL ) 
      { /* first time alloc */
      s = (char*)malloc( newsize );
      ASSERT(s);
      s[0] = 0;
      size = newsize;
      } else
    if ( size != newsize ) 
      { /* expand/shrink */
      s = (char*)realloc( s, newsize );
      s[newsize-1] = 0;
      size = newsize;
      }
  };

  void String::seti( const int n )
  {
    char tmp[64];
    itoa( n, tmp, 10 );
    set( tmp );
  }

  void String::setl( const long n )
  {
    char tmp[64];
    sprintf( tmp, "%ld", n );
    set( tmp );
  }

  void String::setf( const double d )
  {
    char tmp[64];
    sprintf( tmp, "%.10f", d );
    int z = strlen( tmp );
    while( tmp[z-1] == '0' ) z--;
    if ( tmp[z-1] == '.' ) z--;
    tmp[z] = 0;
    set( tmp );
  }

  void String::setfi( const double d ) // sets double as int (w/o frac)
  {
    char tmp[64];
    sprintf( tmp, "%.0f", d );
    int z = strlen( tmp );
    tmp[z] = 0;
    set( tmp );
  }

  void String::set( const char* ps )
  {
    if (ps == NULL || ps[0] == 0)
      {
      resize(0);
      sl = 0;
      s[0] = 0;
      }
    else
      {
      sl = strlen(ps);
      resize( sl );
      memcpy( s, ps, sl );
      s[sl] = 0;
      }
  };

  void String::cat( const char* ps )
  {
    if (ps == NULL) return;
    if (ps[0] == 0) return;
    int psl = strlen( ps );
    resize( sl + psl );
    memcpy( s + sl, ps, psl );
    s[sl+psl] = 0;
    sl += psl;
  };

  void String::setn( const char* ps, int len )
  {
    int z = strlen( ps );
    if ( len < z ) z = len;
    sl = z;
    resize( sl );
    memcpy( s, ps, z );
    s[z] = 0;
  };

  void String::catn( const char* ps, int len )
  {
    int z = strlen( ps );
    if ( len < z ) z = len;
    resize( sl + z );
    memcpy( s + sl, ps, z );
    sl += z;
    s[sl] = 0;
  };

/****************************************************************************
**
** STRING Functions (for class String)
**
****************************************************************************/

  String &str_mul( String &target, int n ) // multiplies the string n times, i.e. `1'*5 = `11111'
  {
    target.resize( target.sl * n );
    str_mul( target.s, n );
    target.fix();
    return target;
  }

  String &str_del( String &target, int pos, int len ) // deletes `len' chars starting from `pos'
  {
    str_del( target.s, pos, len );
    target.fix();
    return target;
  };

  String &str_ins( String &target, int pos, const char* s ) // inserts `s' in position `pos'
  {
    if ( pos > target.sl || pos < 0 ) return target;
    target.resize( target.sl + strlen(s) );
    str_ins( target.s, pos, s );
    target.fixlen();
    return target;
  };

  String &str_ins_ch( String &target, int pos, char ch ) // inserts `ch' in position `pos'
  {
    if ( pos > target.sl || pos < 0 ) return target;
    target.resize( target.sl + 1 );
    str_ins_ch( target.s, pos, ch );
    target.fixlen();
    return target;
  };

  String &str_replace( String &target, const char* out, const char* in ) // replace `out' w. `in'
  {
    int outl = strlen( out );
    int inl = strlen( in );
    int z = str_find( target, out );
    while(z != -1)
      {
      str_del( target, z, outl );
      str_ins( target, z, in );
      z = str_find( target, out, z + inl );
      }
    ASSERT(target.check());
    return target;
  };

  String &str_copy( String &target, const char* source, int pos, int len ) // returns `len' chars from `pos'
  {
    target.resize( len );
    str_copy( target.s, source, pos, len );
    target.fix();
    ASSERT(target.check());
    return target;
  };

  String &str_left( String &target, const char* source, int len ) // returns `len' chars from the left
  {
    return str_copy( target, source, 0, len );
  };

  String &str_right( String &target, const char* source, int len ) // returns `len' chars from the right
  {
    return str_copy( target, source, strlen( source ) - len, len );
  };

  String &str_sleft( String &target, int len ) // SelfLeft -- just as 'Left' but works on `this'
  {
    if ( len < target.sl )
      {
      target.s[len] = 0;
      target.fix();
      }
    return target;
  };

  String &str_sright( String &target, int len ) // SelfRight -- just as 'Right' but works on `this'
  {
    str_sright( target.s, len );
    target.fix();
    return target;
  };


  String &str_trim_left( String &target, int len ) // trims `len' chars from the beginning (left)
  {
    str_trim_left( target.s, len );
    target.fix();
    return target;
  };

  String &str_trim_right( String &target, int len ) // trim `len' chars from the end (right)
  {
    str_trim_right( target.s, len );
    target.fix();
    return target;
  };

  String &str_cut_left( String &target, const char* charlist ) // remove all chars `charlist' from the beginning (i.e. from the left)
  {
    str_cut_left( target.s, charlist );
    target.fix();
    return target;
  };

  String &str_cut_right( String &target, const char* charlist ) // remove all chars `charlist' from the end (i.e. from the right)
  {
    str_cut_right( target.s, charlist );
    target.fix();
    return target;
  };

  String &str_cut( String &target, const char* charlist ) // does `CutR(charlist);CutL(charlist);'
  {
    str_cut_left( target.s, charlist );
    str_cut_right( target.s, charlist );
    target.fix();
    return target;
  };

  String &str_cut_spc( String &target ) // does `Cut(" ");'
  {
    return str_cut( target, " " );
  };

  String &str_pad( String &target, int len, char ch )
  {
    target.resize( (len > 0)?len:-len );
    str_pad( target.s, len, ch );
    target.fixlen();
    return target;
  };

  String &str_comma( String &target, char delim )
  {
    int new_size = str_len( target ) / 3 + str_len( target );
    target.resize( new_size );
    str_comma( target.s, delim );
    target.fix();
    return target;
  };

  void str_set_ch( String &target, int pos, const char ch ) // sets `ch' char at position `pos'
  {
    if ( pos < 0 )
      pos = target.sl + pos;
    if ( pos < 0 && pos >= target.sl ) return;
    target.s[pos] = ch;
  };

  char str_get_ch( String &target, int pos ) // return char at position `pos'
  {
    if ( pos < 0 )
      pos = target.sl + pos;
    if ( pos < 0 && pos >= target.sl ) return 0;
    return target.s[pos];
  };

  void str_add_ch( String &target, const char ch ) // adds `ch' at the end
  {
    int sl = target.sl;
    target.resize(sl+1);
    target.s[sl] = ch;
    target.s[sl+1] = 0;
    target.fix();
  };

  char* str_word( String &target, const char* delimiters, char* result )
  {
    str_word( target.s, delimiters, result );
    target.fix();
    return result[0] ? result : NULL;
  };

  char* str_rword( String &target, const char* delimiters, char* result )
  {
    str_rword( target.s, delimiters, result );
    target.fix();
    return result;
  };


  int sprintf( int init_size, String &target, const char *format, ... )
  {
    char *tmp = new char[init_size+1];
    va_list vlist;
    va_start( vlist, format );
    int res = vsprintf( tmp, format, vlist );
    va_end( vlist );
    target = tmp;
    delete tmp;
    return res;
  };

  int sprintf( String &target, const char *format, ... )
  {
    char tmp[1024];
    va_list vlist;
    va_start( vlist, format );
    int res = vsprintf( tmp, format, vlist );
    va_end( vlist );
    target = tmp;
    return res;
  };

  String& str_tr ( String& target, const char *from, const char *to ) 
  { 
    str_tr( target.s, from, to ); 
    return target; 
  };
  
  String& str_up ( String& target ) 
  { 
    str_up( target.s ); 
    return target; 
  };
  
  String& str_low( String& target ) 
  { 
    str_low( target.s ); 
    return target; 
  };
  
  String& str_flip_case( String& target ) 
  { 
    str_flip_case( target.s ); 
    return target; 
  };
  
  String& str_reverse  ( String& target ) 
  { 
    str_reverse( target.s ); 
    return target; 
  };

  String &str_squeeze( String &target, const char* sq_chars ) // squeeze repeating chars to one only
  {
    str_squeeze( target.s, sq_chars );
    target.fix();
    return target;
  }
  
/****************************************************************************
**
** STRING Functions (for char*)
**
****************************************************************************/

  char* str_mul( char* target, int n ) // multiplies the string n times, i.e. `1'*5 = `11111'
  {
    if ( n < 0 ) return target;
    if ( n == 0 )
      {
      target[0] = 0;
      return target;
      }
    int sl = strlen( target );
    int z = (n - 1) * sl;
    while( z > 0 )
      {
      strncpy( target + z, target, sl );
      z -= sl;
      }
    target[sl*n] = 0;
    return target;
  }

  int str_find( const char* target, const char  s, int startpos ) // returns first zero-based position of char, or -1 if not found
  {
    int sl = strlen( target );
    if (startpos >= sl) return -1;
    char* pc = strchr( target + startpos, s );
    if( ! pc )
      return -1;
    return  pc - target;
  }

  int str_rfind( const char* target, const char c ) // returns last zero-based position of char, or -1 if not found
  {
    char* pc = strrchr( target, c );
    if( ! pc )
      return -1;
    return  pc - target;
  }

  int str_find( const char* target, const char* s, int startpos ) // returns first zero-based position of string, or -1 if not found
  {
    int sl = strlen( target );
    if (startpos >= sl) return -1;
    char* pc = strstr( target + startpos, s );
    if( ! pc )
      return -1;
    return  pc - target;
  }

  int str_rfind( const char* target, const char* s ) // returns last zero-based position of string, or -1 if not found
  {
    int sl = strlen( target );
    int sls = strlen( s );
    int z = sl - sls;
    while ( z > 0 )
      {
      if ( strncmp( target + z, s, sls ) == 0 ) return z;
      z--;
      };
    return -1;
  }


  char* str_del( char* target, int pos, int len ) // deletes `len' chars starting from `pos'
  {
    int sl = strlen( target );
    if ( pos > sl || pos < 0 ) return target;
    int z = sl-pos-len;
    if ( pos + len < sl )
      strncpy(target+pos, target+pos+len, z+1);
    else
      target[pos] = 0;
    return target;
  }

  char* str_ins( char* target, int pos, const char* s ) // inserts `s' in position `pos'
  {
    int sl = strlen( target );
    if ( pos > sl || pos < 0 ) return target;
    int sls = strlen( s );
    if ( sls < 1 ) return target;

    int z = sl - pos;
    target[pos + z + sls] = 0;
    while(z)
      {
      target[pos + z + sls - 1] = target[pos + z - 1];
      z--;
      }

    for(z = 0; z < sls; z++) target[pos+z] = s[z];
    return target;
  }

  char* str_ins_ch( char* target, int pos, char ch ) // inserts `ch' in position `pos'
  {
    char tmp[2];
    tmp[0] = ch;
    tmp[1] = 0;
    str_ins( target, pos, tmp );
    return target;
  }

  char* str_replace( char* target, const char* out, const char* in ) // replace `out' w. `in'
  {
    int outl = strlen( out );
    int inl = strlen( in );
    int z = str_find( target, out );
    while(z != -1)
      {
      str_del( target, z, outl );
      str_ins( target, z, in );
      z = str_find( target, out, z + inl );
      }
    return target;
  }


  char* str_copy( char* target, const char* source, int pos, int len ) // returns `len' chars from `pos'
  {
    target[0] = 0;
    int sl = strlen( source );
    int s = pos;
    int l = len;
    if ( s < 0 || s >= sl ) return target;
    if ( s + l >= sl ) l = sl - s;

    strncpy( target, source + s, l );
    target[ l ] = 0;

    return target;
  }

  char* str_left( char* target, const char* source, int len ) // returns `len' chars from the left
  {
    return str_copy( target, source, 0, len );
  }

  char* str_right( char* target, const char* source, int len ) // returns `len' chars from the right
  {
    return str_copy( target, source, strlen(source) - len, len );
  }

  char* str_sleft( char* target, int len ) // SelfLeft -- just as 'Left' but works on `this'
  {
    if ( (size_t)len < strlen(target) ) target[len] = 0;
    return target;
  };

  char* str_sright( char* target, int len ) // SelfRight -- just as 'Right' but works on `this'
  {
    int sl = strlen( target );
    if ( len < sl )
      {
      strcpy( target, target + ( sl - len ));
      target[len] = 0;
      }
    return target;
  };

  char* str_trim_left( char* target, int len ) // trims `len' chars from the beginning (left)
  {
    int s = strlen( target ) - len;
    if ( s > 0 )  
      strcpy( target, target + len );
    else
      target[0] = 0;  
    return target;
  }

  char* str_trim_right( char* target, int len ) // trim `len' chars from the end (right)
  {
    int e = strlen(target) - len;
    if ( e > 0 ) 
      target[e] = 0;
    else
      target[0] = 0;  
    return target;
  }

  void str_set_ch( char* target, int pos, const char ch ) // sets `ch' char at position `pos'
  {
    int sl = str_len( target );
    if ( pos < 0 )
      pos = sl + pos;
    if ( pos < 0 && pos >= sl ) return;
    target[pos] = ch;
  };

  char str_get_ch( char* target, int pos ) // return char at position `pos'
  {
    int sl = str_len( target );
    if ( pos < 0 )
      pos = sl + pos;
    if ( pos < 0 && pos >= sl ) return 0;
    return target[pos];
  };

  void str_add_ch( char* target, const char ch ) // adds `ch' at the end
  {
    int sl = strlen( target );
    target[sl] = ch;
    target[sl+1] = 0;
  };



  // return first `word', i.e. from pos 0 to first found delimiter char
  // after that deletes this `word' from the string
  char* str_word( char* target, const char* delimiters, char* result )
  {
    int z = 0;
    int sl = strlen( target );
    while ((strchr(delimiters, target[z]) == NULL) && (target[z] != 0)) z++;
    strncpy(result, target, z);
    result[z] = 0;
    if ( z > 0 )
      strncpy( target, target + z + 1, sl - z + 1 );
    return result[0] ? result : NULL;
  }

  // ...same but `last' word
  char* str_rword( char* target, const char* delimiters, char* result )
  {
    result[0] = 0;
    int sl = strlen( target );
    int z = sl - 1;
    while ( strchr( delimiters, target[z] ) == NULL && z > 0 ) z--;
    if (z < 0) return NULL;
    strcpy( result, target + z + 1);
    target[z] = 0;
    return result;
  }


  char* str_cut_left( char* target, const char* charlist ) // remove all chars `charlist' from the beginning (i.e. from the left)
  {
    if (strlen(target) == 0) return target;
    int z = 0;
    while ((strchr(charlist, target[z]) != NULL) && (target[z] != 0)) z++;
    if (z == 0) return target;
    strcpy( target, target + z );
    return target;
  };

  char* str_cut_right( char* target, const char* charlist ) // remove all chars `charlist' from the end (i.e. from the right)
  {
    if (strlen(target) == 0) return target;
    int z = strlen(target) - 1;
    while ((strchr(charlist, target[z]) != NULL) && (z > 0)) z--;
    target[z+1] = 0;
    return target;
  };

  char* str_cut( char* target, const char* charlist ) // does `CutR(charlist);CutL(charlist);'
  {
    str_cut_left( target, charlist );
    str_cut_right( target, charlist );
    return target;
  };

  char* str_cut_spc( char* target ) // does `Cut(" ");'
  {
    str_cut_left( target, " " );
    str_cut_right( target, " " );
    return target;
  };

  // expand align in a field, filled w. `ch', if len > 0 then right, else left
  char* str_pad( char* target, int len, char ch )
  {
    int sl = strlen( target );
    int _len;
    _len = (len >= 0) ? len : - len;
    if ( _len <= sl ) return target;

    char *tmp = new char[_len + 1]; // result buffer -- need len + 1
    tmp[0] = ch;
    tmp[1] = 0;
    str_mul( tmp, _len - sl );

    if ( len < 0 )
      {
      strcat( target, tmp );
      }
    else
      {
      strcat( tmp, target );
      strcpy( target, tmp );
      }
    delete tmp;
    return target;
  }


  // insert `commas' for 1000's delimiter or use another delimiter
  // string supposed to be a integer or real w/o `e' format
  char* str_comma( char* target, char delim )
  {
    int dot = str_rfind( target, '.' );
    if (dot == -1) dot = strlen( target );
    while( dot >= 4 )
      {
      str_ins_ch( target, dot - 3 , delim );
      dot -= 3;
      }
    return target;
  }


  // translate chars from `from' to `to'
  // length of `from' MUST be equal to length of `to'
  char* str_tr( char* target, const char *from, const char *to )
  {
    ASSERT(strlen( from ) == strlen( to ));
    if (strlen( from ) != strlen( to )) return target;
    int z = 0;
    int sl = strlen( target );
    for( z = 0; z < sl; z++ )
      {
      char *pc = strchr( from, target[z] );
      if (pc) target[z] = to[ pc - from ];
      }
    return target;
  }


  char* str_up( char* target )
  {
    int sl = strlen( target );
    for( int z = 0; z < sl; z++ ) target[z] = toupper( target[z] );
    return target;
  }

  char* str_low( char* target )
  {
    int sl = strlen( target );
    for( int z = 0; z < sl; z++ ) target[z] = tolower( target[z] );
    return target;
  }

  char* str_flip_case( char* target ) // CUTE nali? :) // vladi
  {
    int sl = strlen( target );
    for( int z = 0; z < sl; z++ )
      {
      if ( target[z] >= 'a' && target[z] <= 'z' ) target[z] -= 32; else
      if ( target[z] >= 'A' && target[z] <= 'Z' ) target[z] += 32;
      }
    return target;
  }

  char* str_reverse( char* target ) // reverse the string: `abcde' becomes `edcba' :)
  {
    int z = 0;
    int x = strlen(target)-1;
    while( z < x )
      {
      char ch = target[ z ];
      target[ z++ ] = target[ x ];
      target[ x-- ] = ch;
      }
    return target;
  }

  char* str_squeeze( char* target, const char* sq_chars ) // squeeze repeating chars to one only
  {
  if (!target) return NULL;
  if (!sq_chars) return NULL;
  int rc = -1;
  int pos = 0;
  while( target[pos] )
    {
    if ( rc == -1 && strchr( sq_chars, target[pos] ) )
      {
      rc = target[pos];
      pos++;  
      }
    else if ( rc != -1 && target[pos] == rc )
      {
      str_del( target, pos, 1 );
      }
    else if ( rc != -1 && target[pos] != rc )
      {
      rc = -1;
      }
    else
      pos++;  
    }
  return target;  
  }

/****************************************************************************
**
** STRING Functions -- common (String class will pass transparently)
**
****************************************************************************/

  int str_count( const char* target, const char* charlist, int startpos ) // returns match count of all chars from `charlist'
  {
    if (!target) return 0;
    int sl = strlen( target );
    if ( startpos >= sl || startpos < 0 ) return 0;
    int z = startpos;
    int cnt = 0;
    for ( z = 0; z < sl; z++ )
      cnt += ( strchr( charlist, target[z]) != NULL );
    return cnt;
  }

  int str_str_count( const char* target, const char* s, int startpos = 0 ) // returns match count of `s' string into target
  {
    if (!target) return 0;
    int cnt = 0;
    int sl = strlen( s );
    const char* pc = target;
    while( (pc = strstr( pc, s )) )
      {
      pc += sl;
      cnt++;
      }
    return cnt;  
  }
  
  int str_is_int( const char* target ) // check if string is correct int value
  {
    if (!target) return 0;
    char *tmp = strdup( target );
    str_cut_spc( tmp );
    int dc = str_count( tmp, "0123456789" );
    int sl = strlen( tmp );
    free( tmp );
    return ( dc == sl );
  };

  int str_is_double( const char* target ) // check if string is correct double (w/o `e' format :( )
  {
    if (!target) return 0;
    char *tmp = strdup( target );
    str_cut_spc( tmp );
    int dc = str_count( tmp, "0123456789" );
    int cc = str_count( tmp, "." );
    int sl = strlen( tmp );
    free( tmp );
    return ( (dc + cc == sl) && ( cc == 1 ) );
  }

/****************************************************************************
**
** STRING Utilities -- functions and classes
**
****************************************************************************/

  char*	str_dot_reduce( const char* s, char* dest, int width )
  {
    if ( s ) strcpy( dest, s );
    int sl = str_len( dest );
    if ( sl <= width ) return dest;
    int pos = (width-3) / 2;
    str_del( dest, pos, sl - width + 3 );
    str_ins( dest, pos, "..." );
    return dest;
  };
  
  String& str_dot_reduce( const char* s, String& dest, int width )
  {
    if ( s ) dest = s;
    int sl = str_len( dest );
    if ( sl <= width ) return dest;
    int pos = (width-3) / 2;
    str_del( dest, pos, sl - width + 3 );
    str_ins( dest, pos, "..." );
    return dest;
  };

/****************************************************************************
**
** STRING file names utilities -- functions and classes
** NOTE: does not use any external function calls!
**
****************************************************************************/

char* str_fix_path( char* s, int slashtype )
{
  size_t sl = strlen( s );
  if ( s[sl-1] != slashtype )
    {
    s[sl] = slashtype;
    s[sl+1] = 0;
    }
  return s;
}

const char* str_fix_path( String &s, int slashtype )
{
  size_t sl = str_len( s );
  if ( s[sl-1] != slashtype )
    str_add_ch( s, slashtype );
  return (const char*)s;
}

char* str_file_ext( const char *ps, char *ext )
{
ext[0] = 0;
int len = strlen(ps);
int z = len - 1;
while (ps[z] != '.' && ps[z] != '/' && z > 0) z--;
if (ps[z] == '.')
  if (!(z == 0 || (z > 0 && ps[z-1] == '/'))) // it is ".filename" --> no ext;
    strcpy( ext, ps + z + 1 );
  return ext;
}

char* str_file_name( const char *ps, char *name )
{
  int len = strlen(ps);
  int z = len - 1;

  while ( ps[z] != '/' && z >= 0) z--;
  strcpy( name, ps + z + 1 );

  z = strlen( name ) - 1;
  while (name[z] != '.' && name[z] != '/' && z > 0) z--;
  if (name[z] == '.')
    name[z] = 0;
  return name;
}

char* str_file_name_ext( const char *ps, char *path )
{
  int len = strlen(ps);
  int z = len - 1;

  while ( ps[z] != '/' && z >= 0) z--;
  strcpy( path, ps + z + 1 );

  return path;
}

char* str_file_path( const char *ps, char *path ) 
{
  int len = strlen(ps);
  int z = len - 1;
  strcpy( path, ps );

  while ( path[z] != '/' && z > 0) z--;
  if (path[z] == '/')
    path[z+1] = 0;
  else
    path[z] = 0;
  return path;
}

String& str_file_ext( const char *ps, String& ext )
{
  char *t = new char[ strlen( ps ) + 1 ];
  str_file_ext( ps, t );
  ext = t;
  delete t;
  return ext;
};

String& str_file_name( const char *ps, String& name )
{
  char *t = new char[ strlen( ps ) + 1 ];
  str_file_name( ps, t );
  name = t;
  delete t;
  return name;
};

String& str_file_name_ext( const char *ps, String& name )
{
  char *t = new char[ strlen( ps ) + 1 ];
  str_file_name_ext( ps, t );
  name = t;
  delete t;
  return name;
};

String& str_file_path( const char *ps, String& path )
{
  char *t = new char[ strlen( ps ) + 1 ];
  str_file_path( ps, t );
  path = t;
  delete t;
  return path;
};


char* str_reduce_path( const char* path, char* dest ) // removes ".."s
{
  if ( path != NULL ) strcpy( dest, path );
  str_replace( dest, "/./", "/" );
  int i = -1;
  while( (i = str_find( dest, "/../" ) ) != -1 )
    {
    int j = i - 1;
    while( j >= 0 && dest[j] != '/' ) j--;
    ASSERT( j >= -1 );
	if ( j < 0 )
	  {
	  if ( dest[0] == '/' )
	    str_del( dest, 0, 3 );
	  }
	else  
	  str_del( dest, j+1, i+3-j );
    }
  return dest;
}

/****************************************************************************
**
** STRING Conversions
**
****************************************************************************/

  long hex2long( const char* s ) // hex to long
  {
    long P = 1;
    long C = 0;
    char tmp[255];
    strcpy( tmp, s ); str_up( tmp ); str_cut_spc( tmp );
    int sl = strlen(tmp);
    for( int z=sl-1; z >= 0; z-- )
      {
      int i = -1;
      if( (i = str_find( "0123456789ABCDEF", tmp[z] )) != -1)
        C = C + P*i;
      else
        C = 0;
      P = P*16;
      }
    return C;
  }

  char* time2str( const time_t tim )
  {
    time_t t = tim;
    return ctime( &t );
  };

  time_t str2time( const char* timstr )
  {
    if (strlen( timstr ) < 24) return 0;
    char ts[32];
    struct tm m; memset( &m, 0, sizeof(m) );

    strcpy( ts, timstr );
    str_up( ts );
    //  0    5   10    5   20   4
    // "Wed Jun 30 21:49:08 1993\n"
    ts[24] = 0; m.tm_year = atoi( ts + 20 ) - 1900;
    ts[19] = 0; m.tm_sec  = atoi( ts + 17 );
    ts[16] = 0; m.tm_min  = atoi( ts + 14 );
    ts[13] = 0; m.tm_hour = atoi( ts + 11 );
    ts[10] = 0; m.tm_mday = atoi( ts +  8 );
    ts[ 7] = 0; m.tm_mon  = str_find( "JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC", ts+4 ) / 3;
    m.tm_yday = 0;
    m.tm_wday = 0;
    m.tm_isdst = -1;
    time_t tim = mktime( &m );
    return tim;
  };

/***************************************************************************
** EOF
***************************************************************************/

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

