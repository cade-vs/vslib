/*
 *
 *  VSTRING Library supporting structures and functions
 * (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-2003
 *  Distributed under the GPL license, see end of this file for full text!
 *
 *  $Id: vstrlib.cpp,v 1.22 2003/01/21 19:56:35 cade Exp $
 *
 */

#ifdef WIN32
#include "stdafx.h"
#endif
#include "vstrlib.h"

/****************************************************************************
**
** VString aditional functions
**
****************************************************************************/

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

  int str_find_regexp( const char* target, const char* pattern, int startpos = 0 )
  {
    VRegexp re;
    if ( ! re.comp( pattern ) ) return -1;
    if ( startpos < 0 ) return -1;
    int z = 0;
    while( startpos-- )
      {
      if ( target[z] == 0 ) return -1;
      z++;
      }
    if ( re.m( target + z ) )
      return z + re.sub_sp( 0 );
    else
      return -1;
  };
  
  int str_rfind_regexp( const char* target, const char* pattern )
  {
    VRegexp re;
    if ( ! re.comp( pattern ) ) return -1;
    int z = str_len( target );
    while(4)
      {
      z--;
      if ( re.m( target + z ) )
        return z + re.sub_sp( 0 );
      if ( z == 0 ) break;
      }
    return -1;  
  };

/***************************************************************************
**
** VREGEXP
**
****************************************************************************/

  VRegexp::VRegexp()
  { 
    re = NULL;
    pe = NULL;
    rc = 0;
    lp = NULL;
  };
  
  VRegexp::VRegexp( const char* rs, const char* opt )
  { 
    re = NULL; 
    pe = NULL;
    rc = 0;
    lp = NULL;
    comp( rs, opt ); 
  };
  
  VRegexp::~VRegexp() 
  { 
    if ( re ) pcre_free( re ); 
  };

  int VRegexp::get_options( const char* opt )
  {
    if ( ! opt    ) return 0;
    if ( ! opt[0] ) return 0;
    int options = 0;
    int sl = strlen( opt );
    int z;
    for( z = 0; z < sl; z++ )
      {
      switch( opt[z] )
        {
        case 'i': options |= PCRE_CASELESS; break;
        case 'm': options |= PCRE_MULTILINE; break;
        case 's': options |= PCRE_DOTALL; break;
        case 'x': options |= PCRE_EXTENDED; break;
        default: errstr = "invalid option, allowed are: imsx"; return -1;
        }
      }
    return options;  
  };

  int VRegexp::comp( const char* pattern, const char *opt )
  { 
    if ( re ) pcre_free( re ); 
    
    int options = get_options( opt );
    if( options == -1 ) return 0;
    
    const char *error;
    int erroffset;
    re = pcre_compile( pattern, options, &error, &erroffset, NULL );
    
    if ( re )
      {
      errstr = "";
      return 1;
      }
    else
      {
      errstr = error;
      return 0;
      }
  };
  
  int VRegexp::study()
  {
    return 1;
  };
  
  int VRegexp::ok()
  { 
    return re != NULL; 
  }
  
  int VRegexp::m( const char* line )
  { 
    if ( ! re ) 
      {
      errstr = "no pattern compiled";
      return 0;
      }
    if ( ! line )
      {
      return 0;
      }
    errstr = "";
    lp = line;
    rc = pcre_exec( re, pe, lp, strlen( lp ), 0, 0, sp, VREGEXP_MAX_SUBS*3 );
    ASSERT( rc >= -1 && rc != 0 );
    if ( rc > VREGEXP_MAX_SUBS ) rc = VREGEXP_MAX_SUBS;
    if ( rc < 1 ) rc = 0; // fail-safe, should throw exception above in debug mode
    return rc;
  };
  
  int VRegexp::m(  const char* line, const char* pattern, const char *opt )
  { 
    comp( pattern, opt ); 
    return m( line ); 
  };

  const char* VRegexp::sub( int n )
  {
    if ( ! re ) return NULL;
    if ( ! lp ) return NULL;
    if ( n < 0 || n >= rc ) return NULL;
    substr = "";
    
    int s = sp[n*2];
    int e = sp[n*2+1];
    int l = e - s;
    substr.setn( lp + s, l );
    return substr.data();
  };

  int VRegexp::sub_sp( int n )
  {
    if ( n < 0 || n >= rc ) return -1;
    return sp[n*2];
  };
  
  int VRegexp::sub_ep( int n )
  {
    if ( n < 0 || n >= rc ) return -1;
    return sp[n*2+1];
  };

/***************************************************************************
**
** VCHARSET
**
****************************************************************************/

  VCharSet::VCharSet() 
    {
    _data = NULL;
    _size = 0;
    };

  VCharSet::~VCharSet() 
    { 
    _data = NULL;
    _size = 0;
    };

  void VCharSet::resize( int new_size )
  {
    if ( new_size < 1 )
      {
      if ( _data ) delete [] _data;
      _data = NULL;
      _size = 0;
      return;
      }
    // calc required mem size in unsigned chars (bytes?)
    new_size = new_size / sizeof(unsigned char) + (new_size % sizeof(unsigned char) != 0);
    // pad mem size in blocks of VCHARSET_BLOCK_SIZE
    new_size = new_size / VCHARSET_BLOCK_SIZE + (new_size % VCHARSET_BLOCK_SIZE != 0);
    // calc size back to unsigned chars (bytes?)
    new_size *= VCHARSET_BLOCK_SIZE;
    unsigned char *new_data = new unsigned char[ new_size ];
    memset( new_data, 0, new_size );
    if ( _data )
      {
      memcpy( new_data, _data, _size < new_size ? _size : new_size );
      delete [] _data;
      }
    _data = new_data;
    _size = new_size;
  };


  void VCharSet::push( int n, int val )
  {
    if ( n < 0 ) return;
    if ( n >= _size * sizeof(unsigned char) ) resize( n + 1 );
    if ( val )
      _data[ n / sizeof(unsigned char) ] |= 1 << (n % sizeof(unsigned char));
    else
      _data[ n / sizeof(unsigned char) ] &= ~(1 << (n % sizeof(unsigned char)));
  };

  void VCharSet::undef( int n )
  {
    push( n, 0 );
  };

  void VCharSet::undef()
  {
    resize( 0 );
  }

  int VCharSet::in( int n )
  {
    if ( n < 0 || n >= _size * sizeof(unsigned char) ) return 0;
    return ( _data[ n / sizeof(unsigned char) ] & ( 1 << ( n % sizeof(unsigned char) ) ) ) != 0;
  };


/*  
  int VCharSet::get( int pn )
    {
      if ( pn < 0 || pn >= size ) return 0;
      return (data[pn / 8] & (1 << (pn % 8))) != 0;
    };

  void VCharSet::set_range1( int start, int end ) // set range
  {
    char s = ( start < end ) ? start : end;
    char e = ( start > end ) ? start : end;
    for( int z = s; z <= e; z++) set1( z );
  };

  void VCharSet::set_range0( int start, int end ) // set range
  {
    char s = ( start < end ) ? start : end;
    char e = ( start > end ) ? start : end;
    for( int z = s; z <= e; z++) set0( z );
  };

  void VCharSet::set_str1( const char* str )
  {
    int sl = strlen( str );
    for( int z = 0; z < sl; z++ )
      set1( str[z] );
  };

  void VCharSet::set_str0( const char* str )
  {
    int sl = strlen( str );
    for( int z = 0; z < sl; z++ )
      set0( str[z] );
  };

  int VCharSet::in( const char *str )
  {
    int sl = strlen( str );
    for( int z = 0; z < sl; z++ )
      if ( !in( str[z] ) ) return 0;
    return 1;  
  };

  int VCharSet::resize( int p_size )
    {
      ASSERT( p_size > 0 );
      int new_size = p_size;
      int new_datasize = p_size / 8 + (p_size % 8 != 0);
      char *new_data = (char*)malloc( new_datasize );
      if (new_data == NULL) return CE_MEM;
      memset( new_data, 0, new_datasize );
      if (data)
        {
        memcpy( new_data, data, datasize < new_datasize ? datasize : new_datasize );
        free( data );
        data = NULL;
        }
      data = new_data;
      size = new_size;
      datasize = new_datasize;
      return CE_OK;
    }

  VCharSet& VCharSet::operator  = ( const VCharSet &b1 )
  {
    resize( b1.size );
    memcpy( data, b1.data, datasize );
    return *this;
  };

  VCharSet& VCharSet::operator &= ( const VCharSet &b1 )
  {
    int z;
    for(z = 0; z < (datasize < b1.datasize ? datasize : b1.datasize ); z++)
      data[z] &= b1.data[z];
    return *this;
  };

  VCharSet& VCharSet::operator |= ( const VCharSet &b1 )
  {
    int z;
    for(z = 0; z < (datasize < b1.datasize ? datasize : b1.datasize ); z++)
      data[z] |= b1.data[z];
    return *this;
  };

  VCharSet VCharSet::operator ~ ()
  {
    VCharSet b;
    b = *this;
    int z;
    for(z = 0; z < b.datasize; z++)
      b.data[z] = ~b.data[z];
    return b;
  };

  VCharSet operator & ( const VCharSet &b1, const VCharSet &b2 )
  {
    VCharSet b;
    b = b1;
    b &= b2;
    return b;
  };

  VCharSet operator | ( const VCharSet &b1, const VCharSet &b2 )
  {
    VCharSet b;
    b = b1;
    b |= b2;
    return b;
  };
*/

/***************************************************************************
**
** UTILITIES
**
****************************************************************************/

  // split `source' with `regexp_str' regexp
  VArray str_split( const char* regexp_str, const char* source, int maxcount = -1 )
  {
    VArray arr;
    VRegexp re;
    int z = re.comp( regexp_str );
    ASSERT( z );
    if ( ! z ) return arr;

    const char* ps = source;

    while( ps && ps[0] && re.m( ps ) )
      {
      if ( maxcount != -1 )
        {
        maxcount--;
        if ( maxcount == 0 ) break;
        }
      VString s;
      s.setn( ps, re.sub_sp( 0 ) );
      arr.push( s );
      ps += re.sub_ep( 0 );
      }
    if ( ps && ps[0] ) 
      arr.push( ps );
    return arr;  
  };
  
  // split `source' with exact string `delimiter_str'
  VArray str_split_simple( const char* delimiter_str, const char* source, int maxcount = -1 )
  {
    VArray arr;
    const char* ps = source;
    const char* fs;
    
    int rl = strlen( delimiter_str );
    
    VString s;
    while( (fs = strstr( ps, delimiter_str )) )
      {
      if ( maxcount != -1 )
        {
        maxcount--;
        if ( maxcount == 0 ) break;
        }
      int l = fs - ps;
      s.setn( ps, l );
      arr.push( s );
      ps = s + rl;
      }
    if ( ps && ps[0] ) 
      arr.push( ps );
    return arr;  
  };

  // join array data to single string with `glue' string
  // returns the result string or store to optional `dest'
  VString str_join( VArray array, const char* glue = "" )
  {
    VString str;
    for( int z = 0; z < array.count()-1; z++ )
      {
      str += array.get( z );
      str += glue;
      }
    str += array.get( array.count()-1 );
    return str;
  };

/***************************************************************************
**
** EOF
**
****************************************************************************/
