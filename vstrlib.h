/*
 *
 *  VSTRING Library supporting structures and functions
 *  (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-2000
 *  Distributed under the GPL license, you should receive copy of GPL!
 *
 *  VSTRLIB library provides string data structures which mimic Perl's.
 *  There are several classes: 
 *    
 *  VArray -- array of VString elements
 *  VTrie -- associative array (hash) of VString elements
 *  VRegexp -- regular expression helper class
 *
 *  $Id: vstrlib.h,v 1.9 2003/01/01 15:41:55 cade Exp $
 *
 */

#ifndef _VSTRLIB_H_
#define _VSTRLIB_H_

#include <assert.h>
#ifndef ASSERT
#define ASSERT assert
#endif

#include <stdlib.h>
#include "regexp3.h"
#include "vstring.h"

/***************************************************************************
**
** GLOBALS
**
****************************************************************************/

#define VARRAY_BLOCK_SIZE 2048
#define VCHARSET_BLOCK_SIZE 32

class VTrie;   /* forward */
class VArray;  /* forward */
class VRegexp; /* forward */

#define VHash   VTrie;   /* using casual names... */
#define VRegExp VRegexp; /* using casual names... */

/***************************************************************************
**
** VARRAY
**
****************************************************************************/

class VArray
{
  String**  _data;
  int       _size;
  int       _count;

  int       _fe; // foreach element index

  String    _ret_str; // return-container

  void resize( int new_size );

  void q_sort( int lo, int hi );

  public:

  int compact;
  
  VArray();
  VArray( const VArray& arr );
  VArray( const VTrie& tr );
  ~VArray();

  int count() { return _count; } // return element count

  void ins( int n, const char* s ); // insert at position `n'
  void del( int n ); // delete at position `n'
  void set( int n, const char* s ); // set/replace at position `n'
  const char* get( int n ); // get at position `n'

  void undef(); // clear the array (frees all elements)

  int push( const char* s ); // add to the end of the array
  const char* pop(); // get and remove the last element

  int unshift( const char* s ); // add to the beginning of the array
  const char* shift(); // get and remove the first element

  int merge( VTrie *tr ); // return new elements count (same as += )
  int merge( VArray *arr ); // return new elements count (same as += )

  void print(); // print array data to stdout (console)

  int fload( const char* fname ); // return 0 for ok
  int fsave( const char* fname ); // return 0 for ok
  int fload( FILE* f ); // return 0 for ok
  int fsave( FILE* f ); // return 0 for ok

  void sort( int rev = 0 ); // sort (optional reverse order)
  void reverse(); // reverse elements order
  void shuffle(); // randomize element order with Fisher-Yates shuffle

  // split `str' with `res' regexp
  void split( const char* res, const char* str, int maxcount = -1 );
  // split `str' with exact string `spl'
  void split_str( const char* spl, const char* str, int maxcount = -1 );
  // join array data to single string with `glue' string
  // returns the result string or store to optional `dest'
  const char* join( const char* glue = "", String* dest = NULL );

  String& operator []( int n )
    {
      if ( n < 0 ) { _ret_str = ""; return _ret_str; }
      if ( n >= _count ) set( n, "" );
      return *_data[n];
    }

  const VArray& operator = ( const VArray& arr )
    { undef(); merge( (VArray*)&arr ); return *this; };
  const VArray& operator = ( const VTrie& tr )
    { undef(); merge( (VTrie*)&tr ); return *this; };
  const VArray& operator = ( const VString& str )
    { undef(); push( str ); return *this; };
  const VArray& operator += ( const VArray& arr )
    { merge( (VArray*)&arr ); return *this; };
  const VArray& operator += ( const VTrie& tr )
    { merge( (VTrie*)&tr ); return *this; };
  const VArray& operator += ( const VString& str )
    { push( str ); return *this; };
  
  /* utilities */
  
  /* implement `foreach'-like interface */
  void reset() // reset position to beginning
    { _fe = -1; };
  const char* next() // get next item or NULL for the end
    { _fe++; return _fe < _count ? _data[_fe]->data() : NULL; };
  const char* current() // get latest item got from next() -- current one
    { return _fe < _count ? _data[_fe]->data() : NULL; };
  int current_index() // current index
    { return _fe < _count ? _fe : -1; };
    
  int max_len(); // return the length of the longest string in the array
  int min_len(); // return the length of the shortest string in the array
};

/***************************************************************************
**
** VTRIE
**
****************************************************************************/

struct VTrieNode
{
  VTrieNode() { next = down = NULL; c = 0; data = NULL; }

  VTrieNode *next;
  VTrieNode *down;
  char      c;
  String    *data;
};

class VTrie
{
  VTrieNode *root;
  int _count;
  int _depth;
  int _nodes;

  void undef_node( VTrieNode *node );
  void del_node( VTrieNode *parent, VTrieNode *node, const char* key );
  void key_node( VTrieNode *node,  VArray* arr );
  void value_node( VTrieNode *node,  VArray* arr );
  void print_node( VTrieNode* node );

  VTrieNode* find_node( const char* key );

  String temp_key; /* used for keys() */

  public:

  int compact;
  
  VTrie();
  VTrie( const VArray& arr );
  VTrie( const VTrie& tr );
  ~VTrie();

  int count() { return _count; } // return count of elements
  // the following 2 functions are used for profiling purposes mostly
  int depth() { return _depth; } // return current tree depth
  int nodes() { return _nodes; } // return number of used nodes

  int set( const char* key, const char* data ); // set data, same as []
  void del( const char* key ); // remove data associated with `key'
  const char* get( const char* key ); // get data by `key'

  int exists( const char* key ) // return != 0 if key exist (with data)
      { return find_node( key ) != NULL; }

  void undef(); // delete all key+data pairs
  
  int keys( VArray* arr ); // store keys to `arr', return keys count
  int values( VArray* arr ); // store values to `arr',  return values count

  void reverse(); // reverse keys <-> values

  int merge( VTrie *tr ); // return new keys count (same as +=)
  int merge( VArray *arr ); // return new keys count (same as +=)

  void print_nodes() { print_node( root ); }; // for debug only
  void print(); // print trie data to stdout (console)

  int fload( const char* fname ); // return 0 for ok
  int fsave( const char* fname ); // return 0 for ok
  int fload( FILE* f ); // return 0 for ok
  int fsave( FILE* f ); // return 0 for ok

  String& operator []( const char* key )
    {
    //FIXME: this is not optimal implementation...
    VTrieNode *node = find_node( key );
    if ( node ) return *(node->data);
    set( key, "" );
    node = find_node( key );
    ASSERT( node );
    return *(node->data);
    }
  /* FIXME: not needed
  String& operator []( String& key )
    {
    return (*this)[key.data()];
    }
  */
  
  const VTrie& operator = ( const VArray& arr )
    { undef(); merge( (VArray*)&arr ); return *this; };
  const VTrie& operator = ( const VTrie& tr )
    { undef(); merge( (VTrie*)&tr ); return *this; };
  const VTrie& operator += ( const VArray& arr )
    { merge( (VArray*)&arr ); return *this; };
  const VTrie& operator += ( const VTrie& tr )
    { merge( (VTrie*)&tr ); return *this; };
};

/***************************************************************************
**
** VREGEXP
**
****************************************************************************/

class VRegexp
{

  regexp* re;
  String substr;

  public:

  VRegexp() { re = NULL; };
  VRegexp( const char* rs ) // compiles new regexp
    { re = NULL; comp( rs ); };
  ~VRegexp() 
    { if ( re ) free( re ); };

  int comp( const char* rs ) // compile re, return 1 for success
    { if ( re ) free(re); re = regcomp( rs ); return re != NULL; };
  int ok() // return 1 if regexp is compiled ok, 0 if not
    { return re != NULL; }
  int exec( const char* line ) // execute re against line, return 1 for match
    { ASSERT( re ); if (!re) return 0; return regexec( re, line ); };
  int m( const char* line ) // same as exec
    { return exec( line ); };
  int m( const char* rs, const char* line ) // same as exec, but compiles first
    { comp( rs ); return m( line ); };

  const char* sub( int n ); // return n-th substring match
  String& operator []( int n ) // same as sub()
    { substr = ""; sub( n ); return substr; }
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
** MISC FUNCTIONS
**
****************************************************************************/

		
		
#endif /* _VSTRLIB_H_ */

/***************************************************************************
**
** EOF
**
****************************************************************************/

