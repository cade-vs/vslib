/*
 *
 *  VSTRING Library
 *  (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-2002
 *  Distributed under the GPL license, you should receive copy of GPL!
 *
 *  SEE vstring.h FOR FURTHER INFORMATION AND CREDITS
 *
 * $Id: vstring.cpp,v 1.11 2002/12/12 23:45:14 cade Exp $
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

  void String::i( const int n )
  {
    char tmp[64];
    itoa( n, tmp, 10 );
    set( tmp );
  }

  void String::l( const long n )
  {
    char tmp[64];
    sprintf( tmp, "%ld", n );
    set( tmp );
  }

  void String::f( const double d )
  {
    char tmp[64];
    sprintf( tmp, "%.10f", d );
    int z = strlen( tmp );
    while( tmp[z-1] == '0' ) z--;
    if ( tmp[z-1] == '.' ) z--;
    tmp[z] = 0;
    set( tmp );
  }

  void String::fi( const double d ) // sets double as int (w/o frac)
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
      resize( 0 );
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
    if ( pos < 0 || pos >= target.sl ) return;
    target.s[pos] = ch;
  };

  char str_get_ch( String &target, int pos ) // return char at position `pos'
  {
    if ( pos < 0 )
      pos = target.sl + pos;
    if ( pos < 0 || pos >= target.sl ) return 0;
    return target.s[pos];
  };

  void str_add_ch( String &target, const char ch ) // adds `ch' at the end
  {
    int sl = target.sl;
    target.resize( sl+1 );
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
    delete [] tmp;
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
    if ( pos < 0 || pos >= sl ) return;
    target[pos] = ch;
  };

  char str_get_ch( char* target, int pos ) // return char at position `pos'
  {
    int sl = str_len( target );
    if ( pos < 0 )
      pos = sl + pos;
    if ( pos < 0 || pos >= sl ) return 0;
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
    delete [] tmp;
    return target;
  }


  // insert `commas' for 1000's delimiter or use another delimiter
  // string supposed to be a integer or real w/o `e' format
  char* str_comma( char* target, char delim )
  {
    int dot = str_rfind( target, '.' );
    if (dot == -1) dot = strlen( target );
    dot -= 3;
    while( dot > 0 )
      {
      str_ins_ch( target, dot , delim );
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

  int str_str_count( const char* target, const char* s, int startpos ) // returns match count of `s' string into target
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

  while ( z >= 0 && ps[z] != '/' ) z--;
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
  delete [] t;
  return ext;
};

String& str_file_name( const char *ps, String& name )
{
  char *t = new char[ strlen( ps ) + 1 ];
  str_file_name( ps, t );
  name = t;
  delete [] t;
  return name;
};

String& str_file_name_ext( const char *ps, String& name )
{
  char *t = new char[ strlen( ps ) + 1 ];
  str_file_name_ext( ps, t );
  name = t;
  delete [] t;
  return name;
};

String& str_file_path( const char *ps, String& path )
{
  char *t = new char[ strlen( ps ) + 1 ];
  str_file_path( ps, t );
  path = t;
  delete [] t;
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

