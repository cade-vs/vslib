/*
 *
 *  VSTRING Library supporting structures and functions
 *  (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-2000
 *  Distributed under the GPL license, see end of this file for full text!
 *
 *  $Id: vstrlib.cpp,v 1.19 2003/01/16 10:12:16 cade Exp $
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
** VARRAYBOX
**
****************************************************************************/

  VArrayBox* VArrayBox::clone()
  {
    VArrayBox *new_box = new VArrayBox();
    new_box->resize( _size );
    new_box->_count = _count;
    int i;
    for( i = 0; i < _count; i++ )
      {
      new_box->_data[i] = new VString;
      *new_box->_data[i] = *_data[i];
      }
    return new_box;         
  };
  
  void VArrayBox::resize( int new_size )
  {
    ASSERT( new_size >= 0 );
    if ( new_size < 0 ) new_size = 0;
    while ( new_size < _count )
      {
      ASSERT( _data[ _count - 1 ] );
      delete _data[ _count - 1 ];
      _data[ _count - 1 ] = NULL;
      _count--;
      }
    if ( new_size == 0 )
      {
      if ( _data ) delete [] _data;
      _data = NULL;
      _size = 0;
      _count = 0;
      return;
      }
    new_size  = new_size / VARRAY_BLOCK_SIZE + (new_size % VARRAY_BLOCK_SIZE != 0);
    new_size *= VARRAY_BLOCK_SIZE;
    if ( new_size == _size ) return;
    String** new_data = new String*[ new_size ];
    ASSERT( new_data );
    memset( new_data, 0, new_size * sizeof(String*) );
    if ( _data )
      {
      memcpy( new_data, _data,
              (_size < new_size ? _size : new_size) * sizeof(String*) );
      delete [] _data;
      }
    _size = new_size;
    _data = new_data;
  };

/***************************************************************************
**
** VARRAY
**
****************************************************************************/

  VArray::VArray()
  {
    box = new VArrayBox();
    compact = 1;
  }

  VArray::VArray( const VArray& arr )
  {
    box = arr.box;
    box->ref();
    compact = 1;
  }

  VArray::VArray( const VTrie& tr )
  {
    box = new VArrayBox();
    compact = 1;
    *this = tr;
  }

  VArray::~VArray()
  {
    box->unref();
  }

  void VArray::detach()
  {
    if ( box->refs() == 1 ) return;
    VArrayBox *new_box = box->clone();
    box->unref();
    box = new_box;
  }

  void VArray::ins( int n, const char* s )
  {
    detach();
    ASSERT( n >= 0 && n <= box->_count );
    if ( box->_count == box->_size ) box->resize( box->_size + 1 );
    memmove( &box->_data[0] + n + 1,
             &box->_data[0] + n,
             ( box->_count - n ) * sizeof(String*) );
    box->_count++;

    box->_data[n] = new String;
    box->_data[n]->compact = compact;
    box->_data[n]->set( s );
  }

  void VArray::del( int n )
  {
    detach();
    if ( n < 0 || n >= box->_count ) return;
    delete box->_data[n];
    memmove( &box->_data[0] + n,
             &box->_data[0] + n + 1,
             ( box->_count - n ) * sizeof(String*) );
    box->_count--;
    if ( box->_size - box->_count > VARRAY_BLOCK_SIZE ) box->resize( box->_count );
  }

  void VArray::set( int n, const char* s )
  {
    detach();
    ASSERT( n >= 0 );
    if ( n >= box->_count )
      {
      int i = n - box->_count + 1;
      while ( i-- ) push( "" );
      }
    ASSERT( n < box->_count );
    box->_data[n]->set( s );
  }

  const char* VArray::get( int n )
  {
    if ( n < 0 || n >= box->_count )
      return NULL;
    else
      return box->_data[n]->data();
  }

  int VArray::push( const char* s )
  {
    ins( box->_count, s );
    return box->_count;
  }

  const char* VArray::pop()
  {
    if ( box->_count == 0 ) return NULL;
    _ret_str = get( box->_count - 1 );
    del( box->_count - 1 );
    return _ret_str.data();
  }

  int VArray::unshift( const char* s )
  {
    ins( 0, s );
    return box->_count;
  }

  const char* VArray::shift()
  {
    if ( box->_count == 0 ) return NULL;
    _ret_str = get( 0 );
    del( 0 );
    return _ret_str.data();
  }

  int VArray::merge( VTrie *tr )
  {
    VArray va;
    va = tr->keys();
    int cnt = count();
    for( int z = 0; z < cnt; z++ )
      {
      const char* key = va[z];
      push( key );
      push( tr->get( key ) );
      }
    return box->_count;
  };

  int VArray::merge( VArray *arr )
  {
    int cnt = arr->count();
    for( int z = 0; z < cnt; z++ )
      push( arr->get( z ) );
    return box->_count;
  };

  int VArray::fload( const char* fname )
  {
    FILE* f = fopen( fname, "rt" );
    if (!f) return 1;
    int r = fload( f );
    fclose(f);
    return r;
  };

  int VArray::fsave( const char* fname )
  {
    FILE* f = fopen( fname, "wt" );
    if (!f) return 1;
    int r = fsave( f );
    fclose(f);
    return r;
  };

  int VArray::fload( FILE* f )
  {
    char buf[1024];
    String str;
    while( fgets( buf, sizeof(buf)-1, f ) )
      {
      str += buf;
      if ( str_get_ch( str, -1 ) != '\n' && !feof(f) ) continue;
      while ( str_get_ch( str, -1 ) == '\n' ) str_trim_right( str, 1 );
      push( str );
      str = "";
      }
    return 0;
  };

  int VArray::fsave( FILE* f )
  {
    for( int z = 0; z < box->_count; z++ )
      {
      size_t len = strlen( get(z) );
      if ( fwrite( get(z), 1, len, f ) != len ) return 2;
      if ( fwrite( "\n", 1, 1, f ) != 1 ) return 2;
      }
    return 0;
  };

  void VArray::sort( int rev )
  {
    if ( count() > 1 )
      q_sort( 0, count() - 1 );
    if ( rev ) // FIXME: not optimal...
      reverse();
  };

  void VArray::q_sort( int lo, int hi )
  {
    int m, l, r;
    const char* v;

    m = ( hi + lo ) / 2;
    v = box->_data[m]->data();
    l = lo;
    r = hi;

    do
      {
      while( (l <= hi) && (strcmp(box->_data[l]->data(),v) < 0) ) l++;
      while( (r >= lo) && (strcmp(v,box->_data[r]->data()) < 0) ) r--;
      if ( l <= r )
        {
        String *t;
        t = box->_data[l];
        box->_data[l] = box->_data[r];
        box->_data[r] = t;
        l++;
        r--;
        }
      }
    while( l <= r );

    if ( lo < r ) q_sort( lo, r );
    if ( l < hi ) q_sort( l, hi );
  };

  void VArray::reverse()
  {
    int m = box->_count / 2;
    for( int z = 0; z < m; z++ )
      {
      String *t;
      t = box->_data[z];
      box->_data[z] = box->_data[box->_count-1-z];
      box->_data[box->_count-1-z] = t;
      }
  };

  void VArray::shuffle() /* Fisher-Yates shuffle */
  {
    int i = box->_count - 1;
    while( i >= 0 )
      {
      int j = rand() % ( i + 1 );
      String *t;
      t = box->_data[i];
      box->_data[i] = box->_data[j];
      box->_data[j] = t;
      i--;
      }
  };

  void VArray::print()
  {
    int z;
    for( z = 0; z < count(); z++ )
      printf( "%d=%s\n", z, get(z) );
  }

  int VArray::max_len()
  {
    if ( count() == 0 ) return 0;
    int l = 0;
    int z;
    for( z = 0; z < count(); z++ )
      {
      int sl = strlen(get(z));
      if ( sl > l )
        l = sl;
      }
    return l;  
  };
  
  int VArray::min_len()
  {
    if ( count() == 0 ) return 0;
    int l = strlen(get(0));
    int z;
    for( z = 0; z < count(); z++ )
      {
      int sl = strlen(get(z));
      if ( sl < l )
        l = sl;
      }
    return l;  
  };

/***************************************************************************
**
** VTRIENODE
**
****************************************************************************/
 
  VTrieNode::VTrieNode()
  { 
    next = NULL;
    down = NULL; 
    c = 0; 
    data = NULL;
  }
 
  VTrieNode::~VTrieNode()
  {
    if ( next ) delete next;
    if ( down ) delete down;
    if ( data ) delete data;
  }

  void VTrieNode::del_node( const char *key, int branch = 0 )
  {
    if ( !key ) return;
    if ( !key[0] ) return;
    
    if ( key[1] == 0 )
      { // last char reached 
      if ( key[0] != c ) return;  // not found
      // key found
      if ( data ) delete data; // release key's value
      data = NULL;
      if ( down )
        { // there are more keys below
        if ( branch )
          { 
          delete down; // delete all keys below
          c = 0; // mark current as `not used'
          }
        }
      else
        { // there is no more keys below
        c = 0; // mark current as `not used'
        }
      }
    else
      { // last char is not reached
      if ( key[0] == c )
        { // current char is in the key
        if ( down )
          { // try key down
          down->del_node( key + 1, branch );
          if ( down->c == 0 )
            { // down node is not used--remove it
            ASSERT( down->down == NULL );
            VTrieNode *tmp = down;
            down = down->next;
            delete tmp;
            }
          }
        else
          return; // not found
        }
      else
        { 
        if ( next ) // char is not in the key, try next
          {
          next->del_node( key, branch );
          if ( next->c == 0 )
            { // down node is not used--remove it
            ASSERT( next->down == NULL );
            VTrieNode *tmp = next;
            next = next->next;
            delete tmp;
            }
          }
        else
          return; // not found  
        }
      }
  };

  VTrieNode* VTrieNode::find_node( const char* key, int create )
  {
    if ( !key || !key[0] ) return NULL;
    if ( key[0] == c )
      { // char in the current key
      if ( key[1] == 0 )
        { // last char and is in the key--found!
        return this;
        }
      else
        { // not last char...
        if ( ! down && create )
          { // nothing below but should create
          down = new VTrieNode();
          down->c = key[1];
          }
        if ( down )
          return down->find_node( key + 1, create ); // search below
        else
          return NULL; // not found
        }
      }
    else
      { // char not in the current key--try next
      if ( ! next && create )
        { // no next but should create
        next = new VTrieNode();
        next->c = key[0];
        }
      if ( next )
        return next->find_node( key, create ); // search next
      else  
        return NULL; // not found
      }
  };

  VTrieNode *VTrieNode::clone()
  {
    VTrieNode *tmp = new VTrieNode();
    tmp->c = c;
    if ( next ) tmp->next = next->clone();
    if ( down ) tmp->down = down->clone();
    if ( data ) tmp->data = new VString( *data );
    return tmp;  
  };

  void VTrieNode::print()
  {
    printf( "---------------------------------\n" );
    printf( "this = %p\n", this );
    printf( "key  = %c\n", c );
    printf( "next = %p\n", next );
    printf( "down = %p\n", down );
    printf( "data = %s\n", data ? data->data() : "(null)" );
    if (next) next->print();
    if (down) down->print();
  };
  
/***************************************************************************
**
** VTRIEBOX
**
****************************************************************************/

  VTrieBox* VTrieBox::clone()
  {
    VTrieBox *new_box = new VTrieBox();
    delete new_box->root;
    new_box->root = root->clone();
  };

/***************************************************************************
**
** VTRIE
**
****************************************************************************/

  VTrie::VTrie()
  {
    box = new VTrieBox();
  }

  VTrie::VTrie( const VArray& arr )
  {
    box = new VTrieBox();
    merge( (VArray*)&arr );
  };

  VTrie::VTrie( const VTrie& tr )
  {
    box = tr.box;
    box->ref();
  };


  VTrie::~VTrie()
  {
    box->unref();
  }

  void VTrie::detach()
  {
    if ( box->refs() == 1 ) return;
    VTrieBox *new_box = box->clone();
    box->unref();
    box = new_box;
  }

  void VTrie::trace_node( VTrieNode *node, VArray* keys, VArray *vals )
  {
    int kl = str_len( temp_key );
    if ( node->c ) str_add_ch( temp_key, node->c );
    if ( node->data ) 
      {
      if ( keys ) keys->push( temp_key );
      if ( vals ) vals->push( node->data->data() );
      }
    if ( node->down ) trace_node( node->down, keys, vals );
    str_sleft( temp_key, kl );
    if ( node->next ) trace_node( node->next, keys, vals );
  };

  void VTrie::set( const char* key, const char* value )
  {
    if ( !value || !key || !key[0] ) return;
    detach();
    VTrieNode *node = box->root->find_node( key, 1 );
    ASSERT( node );
    if ( ! node->data ) 
      node->data = new VString();
    node->data->set( value );
  }
  
  const char* VTrie::get( const char* key )
  {
    if ( !key || !key[0] ) return NULL;
    VTrieNode *node = box->root->find_node( key );
    if ( node && node->data )
      return node->data->data();
    else
      return NULL;
  };

  void VTrie::del( const char* key )
  {
    if ( !key || !key[0] ) return;
    detach();
    box->root->del_node( key );
  };

  void VTrie::keys_and_values( VArray *keys, VArray *values )
  {
    trace_node( box->root, keys, values );
  };

  VArray VTrie::keys()
  {
    VArray arr;
    keys_and_values( &arr, NULL );
    return arr;
  };

  VArray VTrie::values()
  {
    VArray arr;
    keys_and_values( NULL, &arr );
    return arr;
  };

  void VTrie::reverse()
  {
    VArray ka = keys();
    VArray va = values();
    ASSERT( ka.count() == va.count() );
    undef();
    int z = ka.count();
    while( z-- )
      {
      set( va.get( z ), ka.get( z ) );
      }
  }

  void VTrie::merge( VTrie *tr )
  {
    VArray ka = tr->keys();
    VArray va = tr->values();
    ASSERT( ka.count() == va.count() );
    int z = ka.count();
    while( z-- )
      {
      set( ka.get( z ), va.get( z ) );
      }
  };

  void VTrie::merge( VArray *arr )
  {
    int z = 0;
    while( z < arr->count() )
      {
      set( arr->get( z ), arr->get( z + 1 ) );
      z += 2;
      }
  };

  int VTrie::fload( const char* fname )
  {
    FILE* f = fopen( fname, "rt" );
    if (!f) return 1;
    int r = fload( f );
    fclose(f);
    return r;
  };

  int VTrie::fsave( const char* fname )
  {
    FILE* f = fopen( fname, "wt" );
    if (!f) return 1;
    int r = fsave( f );
    fclose(f);
    return r;
  };

  int VTrie::fload( FILE* f )
  {
    VArray arr;
    int r = arr.fload( f );
    if ( r == 0 )
      merge( &arr );
    return r;
  };

  int VTrie::fsave( FILE* f )
  {
    VArray arr;
    arr.merge( this );
    return arr.fsave( f );
  };
  
  void VTrie::print()
  {
    VArray ka = keys();
    VArray va = values();
    ASSERT( ka.count() == va.count() );
    while( ka.count() && va.count() )
      {
      printf( "%s=%s\n", ka.pop(), va.pop() );
      }
  }

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
      String s;
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
    
    String s;
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
