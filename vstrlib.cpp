/*
 *
 *  VSTRING Library supporting structures and functions
 *  (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-2000
 *  Distributed under the GPL license, see end of this file for full text!
 *
 *  $Id: vstrlib.cpp,v 1.14 2003/01/01 15:41:55 cade Exp $
 *
 */

#ifdef WIN32
#include "stdafx.h"
#endif
#include "vstrlib.h"

/***************************************************************************
**
** VARRAY
**
****************************************************************************/

  VArray::VArray()
  {
    _data = NULL;
    _size = 0;
    _count = 0;
    compact = 1;
    _ret_str = "";
  }

  VArray::VArray( const VArray& arr )
  {
    _data = NULL;
    _size = 0;
    _count = 0;
    compact = 1;
    _ret_str = "";
    *this = arr;
  }

  VArray::VArray( const VTrie& tr )
  {
    _data = NULL;
    _size = 0;
    _count = 0;
    compact = 1;
    _ret_str = "";
    *this = tr;
  }

  VArray::~VArray()
  {
    undef();
  }

  void VArray::resize( int new_size )
  {
    ASSERT( new_size >= 0 );
    if ( new_size == 0 )
      {
      delete [] _data;
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
  }

  void VArray::ins( int n, const char* s )
  {
    ASSERT( n >= 0 && n <= _count );
    if ( _count == _size ) resize( _size + 1 );
    memmove( &_data[0] + n + 1,
             &_data[0] + n,
             ( _count - n ) * sizeof(String*) );
    _count++;

    _data[n] = new String;
    _data[n]->compact = compact;
    _data[n]->set( s );
  }

  void VArray::del( int n )
  {
    if ( n < 0 || n >= _count ) return;
    delete _data[n];
    memmove( &_data[0] + n,
             &_data[0] + n + 1,
             ( _count - n ) * sizeof(String*) );
    _count--;
    if ( _size - _count > VARRAY_BLOCK_SIZE ) resize( _count );
  }

  void VArray::set( int n, const char* s )
  {
    ASSERT( n >= 0 );
    if ( n >= _count )
      {
      int i = n - _count + 1;
      while ( i-- ) push( "" );
      }
    ASSERT( n < _count );
    _data[n]->set( s );
  }

  const char* VArray::get( int n )
  {
    if ( n < 0 || n >= _count )
      return NULL;
    else
      return _data[n]->data();
  }

  void VArray::undef()
  {
    if ( _count )
      for( int z = 0; z < _count; z++ )
        if ( _data[z] )
          delete _data[z];
    resize( 0 );

    _data = NULL;
    _size = 0;
    _count = 0;
    _ret_str = "";
  }

  int VArray::push( const char* s )
  {
    ins( _count, s );
    return _count;
  }

  const char* VArray::pop()
  {
    if ( _count == 0 ) return NULL;
    _ret_str = get( _count - 1 );
    del( _count - 1 );
    return _ret_str.data();
  }

  int VArray::unshift( const char* s )
  {
    ins( 0, s );
    return _count;
  }

  const char* VArray::shift()
  {
    if ( _count == 0 ) return NULL;
    _ret_str = get( 0 );
    del( 0 );
    return _ret_str.data();
  }

  int VArray::merge( VTrie *tr )
  {
    VArray va;
    int cnt = tr->keys( &va );
    for( int z = 0; z < cnt; z++ )
      {
      const char* key = va[z];
      push( key );
      push( tr->get( key ) );
      }
    return _count;
  };

  int VArray::merge( VArray *arr )
  {
    int cnt = arr->count();
    for( int z = 0; z < cnt; z++ )
      push( arr->get( z ) );
    return _count;
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
    for( int z = 0; z < _count; z++ )
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
    v = _data[m]->data();
    l = lo;
    r = hi;

    do
      {
      while( (l <= hi) && (strcmp(_data[l]->data(),v) < 0) ) l++;
      while( (r >= lo) && (strcmp(v,_data[r]->data()) < 0) ) r--;
      if ( l <= r )
        {
        String *t;
        t = _data[l];
        _data[l] = _data[r];
        _data[r] = t;
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
    int m = _count / 2;
    for( int z = 0; z < m; z++ )
      {
      String *t;
      t = _data[z];
      _data[z] = _data[_count-1-z];
      _data[_count-1-z] = t;
      }
  };

  void VArray::shuffle() /* Fisher-Yates shuffle */
  {
    int i = _count - 1;
    while( i >= 0 )
      {
      int j = rand() % ( i + 1 );
      String *t;
      t = _data[i];
      _data[i] = _data[j];
      _data[j] = t;
      i--;
      }
  };

  void VArray::split( const char* res, const char* str, int maxcount )
  {
    regexp *re = regcomp( res );
    ASSERT( re );

    undef();

    const char* ps = str;

    while( ps && ps[0] && regexec( re, ps ) )
      {
      if ( maxcount != -1 )
        {
        maxcount--;
        if ( maxcount == 0 ) break;
        }
      String s;
      int l = re->startp[0] - ps;
      s.setn( ps, l );
      push( s );
      ps = re->endp[0];
      }
    push( ps );

    free( re );
  };

  void VArray::split_str( const char* spl, const char* str, int maxcount )
  {
    const char* ps = str;
    const char* fs;
    
    undef();
    
    int rl = strlen( spl );
    
    String s;
    while( (fs = strstr( ps, spl )) )
      {
      if ( maxcount != -1 )
        {
        maxcount--;
        if ( maxcount == 0 ) break;
        }
      int l = fs - ps;
      s.setn( ps, l );
      push( s );
      ps = s + rl;
      }
    push( ps );
  };

  const char* VArray::join( const char* glue, String* dest )
  {
    if( ! dest ) dest = &_ret_str;
    
    *dest = "";
    for( int z = 0; z < _count-1; z++ )
      {
      *dest += get( z );
      *dest += glue;
      }
    *dest += get( _count-1 );
    return dest->data();
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
** VTRIE
**
****************************************************************************/

  VTrie::VTrie()
  {
    _count = 0;
    _depth = 0;
    _nodes = 0;
    compact = 1;
    root = new VTrieNode();
    _nodes++;
  }

  VTrie::VTrie( const VArray& arr )
  {
    _count = 0;
    _depth = 0;
    _nodes = 0;
    compact = 1;
    root = new VTrieNode();
    _nodes++;
    *this = arr;
  };

  VTrie::VTrie( const VTrie& tr )
  {
    _count = 0;
    _depth = 0;
    _nodes = 0;
    compact = 1;
    root = new VTrieNode();
    _nodes++;
    *this = tr;
  };


  VTrie::~VTrie()
  {
    undef_node( root );
  }

  void VTrie::undef_node( VTrieNode *node )
  {
    if ( node->next ) undef_node( node->next );
    if ( node->down ) undef_node( node->down );
    if ( node->data ) delete node->data;
    delete node;
    _nodes--;
  }

  void VTrie::del_node( VTrieNode *parent, VTrieNode *node, const char* key )
  {
    ASSERT( key[0] );
    if ( key[1] == 0 )
      {
      if ( key[0] == node->c )
        {
        if ( node->data )
          delete node->data;
        node->data = NULL;
        }
      else
        {
        if ( node->next )
          del_node( node, node->next, key );
        else
          return; /* not found */
        }  
      }
    else
      {
      if ( key[0] == node->c )
        {
        if ( node->down )
          del_node( node, node->down, key+1 );
        else
          return; /* not found */
        }
      else
        {
        if ( node->next )
          del_node( node, node->next, key );
        else
          return; /* not found */
        }
      }

      if ( node->data ) return;
      if ( node->down ) return;
      if ( node->next )
        {
        if ( parent->down == node )
          parent->down = node->next;
        else
          parent->next = node->next;
        }
      else
        {
        if ( parent->down == node )
          parent->down = NULL;
        else
          parent->next = NULL;
        }
      delete node;
      _nodes--;
  }

  void VTrie::key_node( VTrieNode *node,  VArray* arr )
  {
    int kl = str_len( temp_key );
    if ( node->c ) str_add_ch( temp_key, node->c );
    if ( node->data ) arr->push( temp_key );
    if ( node->down ) key_node( node->down, arr );
    str_sleft( temp_key, kl );
    if ( node->next ) key_node( node->next, arr );
  };

  void VTrie::value_node( VTrieNode *node,  VArray* arr )
  {
    if ( node->data ) arr->push( node->data->data() );
    if ( node->down ) value_node( node->down, arr );
    if ( node->next ) value_node( node->next, arr );
  };

  VTrieNode* VTrie::find_node( const char* key )
  {
    if ( !key || !key[0] ) return NULL;

    VTrieNode *current = root;
    const char* keyp = key;
    while( *keyp )
      {
      current = current->down;
      if ( !current )
        {
        return NULL;
        }
      else
        {
        while( current && current->c != *keyp )
          {
          current = current->next;
          }
        if ( !current )
          {
          return NULL;
          }
        }
      keyp++;
      }
    if ( current )
      {
      if ( !current->data ) 
        {
        current->data = new String();
        current->data->compact = compact;
        }
      return current;
      }
    return NULL;
  };

  int VTrie::set( const char* key, const char* data )
  {
    if ( !key || !key[0] ) return _count;
    int sl = strlen( key );
    if ( sl > _depth )
      _depth = sl;

    VTrieNode *current = root;
    const char* keyp = key;
    while( *keyp )
      {
      VTrieNode *parent = current;
      current = current->down;
      if ( !current )
        {
        current = new VTrieNode();
        parent->down = current;
        current->c = *keyp;
        _nodes++;
        }
      else
        {
        while( current && current->c != *keyp )
          {
          parent = current;
          current = current->next;
          }
        if ( !current )
          {
          current = new VTrieNode();
          parent->next = current;
          current->c = *keyp;
          _nodes++;
          }
        }
      keyp++;
      }
    if ( current->data )
      delete current->data;
    else
      _count++;
    current->data = new String;
    current->data->compact = compact;
    current->data->set( data );
    
    sl = str_len( *current->data );
    
    return _count;
  };

  const char* VTrie::get( const char* key )
  {
    VTrieNode* node = find_node( key );
    if ( node )
      return node->data->data();
    else
      return NULL;
  };

  void VTrie::del( const char* key )
  {
    if ( !key || !key[0] || !root->down ) return;
    del_node( root, root->down, key );
  };

  void VTrie::undef()
  {
    undef_node( root );
    root = new VTrieNode();
    _nodes++;
  };

  int VTrie::keys( VArray* arr )
  {
    arr->undef();
    temp_key = "";
    key_node( root, arr );
    return arr->count();
  };

  int VTrie::values( VArray* arr )
  {
    arr->undef();
    temp_key = "";
    value_node( root, arr );
    return arr->count();
  };

  void VTrie::reverse()
  {
    VArray va;
    va = *this;
    undef();
    while( va.count() )
      {
      set( va[1], va[0] );
      va.shift();
      va.shift();
      }
  }

  int VTrie::merge( VTrie *tr )
  {
    VArray arr;
    int cnt = tr->keys( &arr );
    for( int z = 0; z < cnt; z++ )
      set( arr[z], tr->get( arr[z] ) );
    return _count;
  };

  int VTrie::merge( VArray *arr )
  {
    while( arr->get( 0 ) )
      {
      if ( arr->get( 1 ) )
        {
        set( arr->get( 0 ), arr->get( 1 ) );
        arr->del( 0 );
        arr->del( 0 );
        }
      else
        {
        set( arr->get( 0 ), "" );
        arr->del( 0 );
        }
      }
    return _count;
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

  void VTrie::print_node( VTrieNode* node )
  {
    if (!node) return;
    printf( "---------------------------------\n" );
    printf( "this = %p\n", node );
    printf( "key  = %c\n", node->c );
    printf( "next = %p\n", node->next );
    printf( "down = %p\n", node->down );
    printf( "data = %s\n", node->data->data() );
    print_node( node->next );
    print_node( node->down );
  };

  void VTrie::print()
  {
    VArray va;
    keys( &va );
    int z;
    for( z = 0; z < va.count(); z++ )
      printf( "%s=%s\n", va[z].data(), get( va[z] ) );
  }

/***************************************************************************
**
** VREGEXP
**
****************************************************************************/

  const char* VRegexp::sub( int n )
  {
    if(re->startp[n] != NULL && re->endp[n] != NULL)
      {
      int i = (int)(re->endp[n] - re->startp[n]);
      substr.setn(re->startp[n],i);
      return substr.data();
      }
    else
      {
      substr = "";
      return NULL;
      }
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
** EOF
**
****************************************************************************/
