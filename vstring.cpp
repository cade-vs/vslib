/*
 *
 *  VSTRING Library
 *  (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-2002
 *  Distributed under the GPL license, you should receive copy of GPL!
 *
 *  SEE vstring.h FOR FURTHER INFORMATION AND CREDITS
 *
 *  $Id: vstring.cpp,v 1.15 2003/01/19 16:14:54 cade Exp $
 *
 *  This file (vstring.h and vstring.cpp) implements plain string-only 
 *  manipulations. For further functionality see vstrlib.h and vstrlib.cpp.
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
** VSTRING BOX
**
****************************************************************************/

  VStringBox* VStringBox::clone()
  {
    VStringBox* box = new VStringBox();
    box->resize_buf( size );
    box->sl = sl;
    box->compact = compact;
    memcpy( box->s, s, size );
    return box;
  };
  
  void VStringBox::resize_buf( int new_size )
  {
    if ( new_size == 0 )
      {
      sl = 0;
      if ( s ) free( s );
      s = NULL;
      size = 0;
      return;
      }
    new_size++; /* for the trailing 0 */
    if ( !compact )
      {
      new_size = new_size / STR_BLOCK_SIZE  + ( new_size % STR_BLOCK_SIZE != 0 );
      new_size *= STR_BLOCK_SIZE;
      }
    if( s == NULL ) 
      { /* first time alloc */
      s = (char*)malloc( new_size );
      ASSERT(s);
      s[0] = 0;
      size = new_size;
      } else
    if ( size != new_size ) 
      { /* expand/shrink */
      s = (char*)realloc( s, new_size );
      s[new_size-1] = 0;
      size = new_size;
      }
  };

/****************************************************************************
**
** VSTRING
**
****************************************************************************/

  void VString::detach()
  {
    if ( box->refs() == 1 ) return;
    VStringBox *new_box = box->clone();
    box->unref();
    box = new_box;
  }

  void VString::i( const int n )
  {
    char tmp[64];
    itoa( n, tmp, 10 );
    set( tmp );
  }

  void VString::l( const long n )
  {
    char tmp[64];
    sprintf( tmp, "%ld", n );
    set( tmp );
  }

  void VString::f( const double d )
  {
    char tmp[64];
    sprintf( tmp, "%.10f", d );
    int z = strlen( tmp );
    while( tmp[z-1] == '0' ) z--;
    if ( tmp[z-1] == '.' ) z--;
    tmp[z] = 0;
    set( tmp );
  }

  void VString::fi( const double d ) // sets double as int (w/o frac)
  {
    char tmp[64];
    sprintf( tmp, "%.0f", d );
    int z = strlen( tmp );
    tmp[z] = 0;
    set( tmp );
  }

  void VString::set( const char* ps )
  {
    if (ps == NULL || ps[0] == 0)
      {
      resize( 0 );
      }
    else
      {
      int sl = strlen(ps);
      resize( sl );
      memcpy( box->s, ps, sl );
      box->sl = sl;
      box->s[sl] = 0;
      }
  };

  void VString::cat( const char* ps )
  {
    if (ps == NULL) return;
    if (ps[0] == 0) return;
    int psl = strlen( ps );
    resize( box->sl + psl );
    memcpy( box->s + box->sl, ps, psl );
    box->s[ box->sl + psl ] = 0;
    box->sl += psl;
  };

  void VString::setn( const char* ps, int len )
  {
    if ( !ps || len < 1 ) 
      {
      resize( 0 );
      return;
      }
    int z = strlen( ps );
    if ( len < z ) z = len;
    resize( z );
    box->sl = z;
    memcpy( box->s, ps, z );
    box->s[z] = 0;
  };

  void VString::catn( const char* ps, int len )
  {
    if ( !ps || len < 1 ) return;
    int z = strlen( ps );
    if ( len < z ) z = len;
    resize( box->sl + z );
    memcpy( box->s + box->sl, ps, z );
    box->sl += z;
    box->s[ box->sl ] = 0;
  };

/****************************************************************************
**
** VString Functions (for class VString)
**
****************************************************************************/

  VString &str_mul( VString &target, int n ) // multiplies the VString n times, i.e. `1'*5 = `11111'
  {
    target.resize( target.box->sl * n );
    str_mul( target.box->s, n );
    target.fix();
    return target;
  }

  VString &str_del( VString &target, int pos, int len ) // deletes `len' chars starting from `pos'
  {
    if ( pos > target.box->sl || pos < 0 ) return target;
    target.detach();
    str_del( target.box->s, pos, len );
    target.fix();
    return target;
  };

  VString &str_ins( VString &target, int pos, const char* s ) // inserts `s' in position `pos'
  {
    if ( pos > target.box->sl || pos < 0 ) return target;
    target.resize( target.box->sl + strlen(s) );
    str_ins( target.box->s, pos, s );
    target.fixlen();
    return target;
  };

  VString &str_ins_ch( VString &target, int pos, char ch ) // inserts `ch' in position `pos'
  {
    if ( pos > target.box->sl || pos < 0 ) return target;
    target.resize( target.box->sl + 1 );
    str_ins_ch( target.box->s, pos, ch );
    target.fixlen();
    return target;
  };

  VString &str_replace( VString &target, const char* out, const char* in ) // replace `out' w. `in'
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

  VString &str_copy( VString &target, const char* source, int pos, int len ) // returns `len' chars from `pos'
  {
    if ( len == -1 ) len = str_len( source ) - pos;
    if ( len < 1 )
      {
      target = "";
      return target;
      }
    target.resize( len );
    str_copy( target.box->s, source, pos, len );
    target.fix();
    ASSERT( target.check() );
    return target;
  };

  VString &str_left( VString &target, const char* source, int len ) // returns `len' chars from the left
  {
    return str_copy( target, source, 0, len );
  };

  VString &str_right( VString &target, const char* source, int len ) // returns `len' chars from the right
  {
    return str_copy( target, source, strlen( source ) - len, len );
  };

  VString &str_sleft( VString &target, int len ) // SelfLeft -- just as 'Left' but works on `this'
  {
    if ( len < target.box->sl )
      {
      target.detach();
      target.box->s[len] = 0;
      target.fix();
      }
    return target;
  };

  VString &str_sright( VString &target, int len ) // SelfRight -- just as 'Right' but works on `this'
  {
    target.detach();
    str_sright( target.box->s, len );
    target.fix();
    return target;
  };


  VString &str_trim_left( VString &target, int len ) // trims `len' chars from the beginning (left)
  {
    target.detach();
    str_trim_left( target.box->s, len );
    target.fix();
    return target;
  };

  VString &str_trim_right( VString &target, int len ) // trim `len' chars from the end (right)
  {
    target.detach();
    str_trim_right( target.box->s, len );
    target.fix();
    return target;
  };

  VString &str_cut_left( VString &target, const char* charlist ) // remove all chars `charlist' from the beginning (i.e. from the left)
  {
    target.detach();
    str_cut_left( target.box->s, charlist );
    target.fix();
    return target;
  };

  VString &str_cut_right( VString &target, const char* charlist ) // remove all chars `charlist' from the end (i.e. from the right)
  {
    target.detach();
    str_cut_right( target.box->s, charlist );
    target.fix();
    return target;
  };

  VString &str_cut( VString &target, const char* charlist ) // does `CutR(charlist);CutL(charlist);'
  {
    target.detach();
    str_cut_left( target.box->s, charlist );
    str_cut_right( target.box->s, charlist );
    target.fix();
    return target;
  };

  VString &str_cut_spc( VString &target ) // does `Cut(" ");'
  {
    return str_cut( target, " " );
  };

  VString &str_pad( VString &target, int len, char ch )
  {
    target.resize( (len > 0) ? len : -len );
    str_pad( target.box->s, len, ch );
    target.fixlen();
    return target;
  };

  VString &str_comma( VString &target, char delim )
  {
    int new_size = str_len( target ) / 3 + str_len( target );
    target.resize( new_size );
    str_comma( target.box->s, delim );
    target.fix();
    return target;
  };

  void str_set_ch( VString &target, int pos, const char ch ) // sets `ch' char at position `pos'
  {
    if ( pos < 0 ) pos = target.box->sl + pos;
    if ( pos < 0 || pos >= target.box->sl ) return;
    if (target.box->s[pos] != ch) target.detach();
    target.box->s[pos] = ch;
  };

  char str_get_ch( VString &target, int pos ) // return char at position `pos'
  {
    if ( pos < 0 ) pos = target.box->sl + pos;
    if ( pos < 0 || pos >= target.box->sl ) return 0;
    return target.box->s[pos];
  };

  void str_add_ch( VString &target, const char ch ) // adds `ch' at the end
  {
    int sl = target.box->sl;
    target.resize( sl+1 );
    target.box->s[sl] = ch;
    target.box->s[sl+1] = 0;
    target.fix();
  };

  char* str_word( VString &target, const char* delimiters, char* result )
  {
    target.detach();
    str_word( target.box->s, delimiters, result );
    target.fix();
    return result[0] ? result : NULL;
  };

  char* str_rword( VString &target, const char* delimiters, char* result )
  {
    target.detach();
    str_rword( target.box->s, delimiters, result );
    target.fix();
    return result;
  };

  int sprintf( int init_size, VString &target, const char *format, ... )
  {
    char *tmp = new char[init_size+1];
    va_list vlist;
    va_start( vlist, format );
    int res = vsnprintf( tmp, init_size, format, vlist );
    va_end( vlist );
    target = tmp;
    delete [] tmp;
    return res;
  };

  int sprintf( VString &target, const char *format, ... )
  {
    #define VSPRINTF_BUF_SIZE 1024
    char tmp[VSPRINTF_BUF_SIZE+1];
    va_list vlist;
    va_start( vlist, format );
    int res = vsnprintf( tmp, VSPRINTF_BUF_SIZE, format, vlist );
    va_end( vlist );
    target = tmp;
    return res;
  };

  VString& str_tr ( VString& target, const char *from, const char *to ) 
  { 
    target.detach();
    str_tr( target.box->s, from, to ); 
    return target; 
  };
  
  VString& str_up ( VString& target ) 
  { 
    target.detach();
    str_up( target.box->s ); 
    return target; 
  };
  
  VString& str_low( VString& target ) 
  { 
    target.detach();
    str_low( target.box->s ); 
    return target; 
  };
  
  VString& str_flip_case( VString& target ) 
  { 
    target.detach();
    str_flip_case( target.box->s ); 
    return target; 
  };
  
  VString& str_reverse( VString& target ) 
  { 
    target.detach();
    str_reverse( target.box->s ); 
    return target; 
  };

  VString &str_squeeze( VString &target, const char* sq_chars ) // squeeze repeating chars to one only
  {
    target.detach();
    str_squeeze( target.box->s, sq_chars );
    target.fix();
    return target;
  }
  
/****************************************************************************
**
** VString Functions (for char*)
**
****************************************************************************/

  char* str_mul( char* target, int n ) // multiplies the VString n times, i.e. `1'*5 = `11111'
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
    if (startpos < 0) return -1;
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

  int str_find( const char* target, const char* s, int startpos ) // returns first zero-based position of VString, or -1 if not found
  {
    if (startpos < 0) return -1;
    int sl = strlen( target );
    if (startpos >= sl) return -1;
    char* pc = strstr( target + startpos, s );
    if( ! pc )
      return -1;
    return  pc - target;
  }

  int str_rfind( const char* target, const char* s ) // returns last zero-based position of VString, or -1 if not found
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
    int sl = str_len( source );
    
    if ( pos < 0 || pos >= sl ) return target;
    if ( len == -1 ) len = sl - pos;
    if ( len < 1 ) return target;
    if ( pos + len >= sl ) len = sl - pos;

    strncpy( target, source + pos, len );
    target[ len ] = 0;
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
  // after that deletes this `word' from the VString
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
  // VString supposed to be a integer or real w/o `e' format
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

  char* str_reverse( char* target ) // reverse the VString: `abcde' becomes `edcba' :)
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
** VString Functions -- common (VString class will pass transparently)
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

  int str_str_count( const char* target, const char* s, int startpos ) // returns match count of `s' VString into target
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
  
  int str_is_int( const char* target ) // check if VString is correct int value
  {
    if (!target) return 0;
    char *tmp = strdup( target );
    str_cut_spc( tmp );
    int dc = str_count( tmp, "0123456789" );
    int sl = strlen( tmp );
    free( tmp );
    return ( dc == sl );
  };

  int str_is_double( const char* target ) // check if VString is correct double (w/o `e' format :( )
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
** VString Utilities -- functions and classes
**
****************************************************************************/

  VString str_dot_reduce( const char* s, int width )
  {
    VString dest;
    dest = s;
    int sl = str_len( dest );
    if ( sl <= width ) return dest;
    int pos = (width-3) / 2;
    str_del( dest, pos, sl - width + 3 );
    str_ins( dest, pos, "..." );
    return dest;
  };

/****************************************************************************
**
** VString file names utilities -- functions and classes
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

const char* str_fix_path( VString &s, int slashtype )
{
  size_t sl = str_len( s );
  if ( s[sl-1] != slashtype )
    str_add_ch( s, slashtype );
  return (const char*)s;
}

VString str_file_ext( const char *ps )
{
  VString ext;
  int len = strlen(ps);
  int z = len - 1;
  while ( ps[z] != '.' && ps[z] != '/' && z > 0 ) z--;
  if ( ps[z] == '.' )
    if ( !(z == 0 || (z > 0 && ps[z-1] == '/')) ) // `.name' has no extension!
      ext = ps + z + 1;
  return ext;
};

VString str_file_name( const char *ps )
{
  VString name;
  
  int len = strlen( ps );
  int z = len - 1;

  while ( z >= 0 && ps[z] != '/' ) z--;
  name = ps + z + 1;

  z = str_len( name ) - 1;
  while ( z > 0 && name[z] != '.' && name[z] != '/' ) z--;
  if ( z > 0 && name[z] == '.') // `.name' has no extension!
    str_sleft( name, z );
  return name;
};

VString str_file_name_ext( const char *ps )
{
  VString name;
  
  int len = strlen( ps );
  int z = len - 1;

  while ( z >= 0 && ps[z] != '/' ) z--;
  name = ps + z + 1;
  return name;
};

VString str_file_path( const char *ps )
{
  VString name;
  
  int len = strlen( ps );
  int z = len;

  while ( z >= 0 && ps[z] != '/' ) z--;
  name = ps;
  str_sleft( name, z+1 );
  return name;
};


VString str_reduce_path( const char* path ) // removes ".."s
{
  VString dest;
  dest = path;
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
** VString Conversions
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

/***************************************************************************
**
** EOF
**
****************************************************************************/

