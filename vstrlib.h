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
 *  $Id: vstrlib.h,v 1.16 2003/01/19 16:44:03 cade Exp $
 *
 */

#ifndef _VSTRLIB_H_
#define _VSTRLIB_H_

#include <assert.h>
#ifndef ASSERT
#define ASSERT assert
#endif

#include <stdlib.h>
#include <time.h>
#include <pcre.h>

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

/****************************************************************************
**
** VString aditional functions
**
****************************************************************************/

char*  time2str( const time_t tim );
time_t str2time( const char* timstr );

int str_find_regexp( const char* target, const char* pattern, int startpos = 0 );
// warning: str_rfind_regexp() is slow! it can execute pattern matching to `n'
// times where n is the target string length...
int str_rfind_regexp( const char* target, const char* pattern );

/***************************************************************************
**
** VARRAYBOX
**
****************************************************************************/

class VArrayBox : public VRef
{
public:
  
  String**  _data;
  int       _size;
  int       _count;
  
  VArrayBox() { _data = NULL; _size = 0; _count = 0; };
  ~VArrayBox() { undef(); };
  
  VArrayBox* clone();
  
  void resize( int new_size );
  void undef() { resize( 0 ); };
};

/***************************************************************************
**
** VARRAY
**
****************************************************************************/

class VArray
{
  VArrayBox *box;
  
  int       _fe; // foreach element index

  String    _ret_str; // return-container

  void detach();
  void q_sort( int lo, int hi );

  public:

  int compact;
  
  VArray();
  VArray( const VArray& arr );
  VArray( const VTrie& tr );
  ~VArray();

  int count() { return box->_count; } // return element count

  void ins( int n, const char* s ); // insert at position `n'
  void del( int n ); // delete at position `n'
  void set( int n, const char* s ); // set/replace at position `n'
  const char* get( int n ); // get at position `n'

  void undef() // clear the array (frees all elements)
      { box->unref(); box = new VArrayBox(); _ret_str = ""; }
      
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

  String& operator []( int n )
    {
      if ( n < 0 ) { _ret_str = ""; return _ret_str; }
      if ( n >= box->_count ) 
        set( n, "" );
      else
        detach(); // I don't know if user will change returned VString?!  
      return *box->_data[n];
    }

  const VArray& operator = ( const VArray& arr )
    {
    box->unref();
    box = arr.box;
    box->ref();
    return *this; 
    };
    
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
    { _fe++; return _fe < box->_count ? box->_data[_fe]->data() : NULL; };
  const char* current() // get latest item got from next() -- current one
    { return _fe < box->_count ? box->_data[_fe]->data() : NULL; };
  int current_index() // current index
    { return _fe < box->_count ? _fe : -1; };
    
  int max_len(); // return the length of the longest string in the array
  int min_len(); // return the length of the shortest string in the array
};

/***************************************************************************
**
** VTRIENODE
**
****************************************************************************/

class VTrieNode
{
public:

  VTrieNode();
  ~VTrieNode();

  VTrieNode *next;
  VTrieNode *down;
  char      c;
  String    *data;

  void detach() { next = down = NULL; }
  void del_node( const char *key, int branch = 0 );
  VTrieNode* find_node( const char* key, int create = 0 );
  
  VTrieNode *clone();
  void print();
};

/***************************************************************************
**
** VTRIEBOX
**
****************************************************************************/

class VTrieBox : public VRef
{
public:
  
  VTrieNode *root;
  
  VTrieBox()  { root = new VTrieNode(); }
  ~VTrieBox() { ASSERT( root ); delete root; }
  
  VTrieBox* clone();
  void undef() { ASSERT( root ); delete root; root = new VTrieNode(); };
};

/***************************************************************************
**
** VTRIE
**
****************************************************************************/

class VTrie
{
  VTrieBox *box;
  
  void detach();
  void trace_node( VTrieNode *node, VArray* keys, VArray *vals );

  VString temp_key;
  
  public:

  int compact;
  
  VTrie();
  VTrie( const VArray& arr );
  VTrie( const VTrie& tr );
  ~VTrie();

  void set( const char* key, const char* data ); // set data, same as []
  void del( const char* key ); // remove data associated with `key'
  const char* get( const char* key ); // get data by `key'

  int exists( const char* key ) // return != 0 if key exist (with data)
      { return box->root->find_node( key ) != NULL; }

  void undef() // delete all key+data pairs
    { box->unref(); box = new VTrieBox(); }
  
  void keys_and_values( VArray *keys, VArray *values );
  
  VArray keys(); // store keys to `arr', return keys count
  VArray values(); // store values to `arr',  return values count

  void reverse(); // reverse keys <-> values

  void merge( VTrie *tr ); // return new keys count (same as +=)
  void merge( VArray *arr ); // return new keys count (same as +=)

  //void print_nodes() { print_node( root ); }; // for debug only
  void print(); // print trie data to stdout (console)

  int fload( const char* fname ); // return 0 for ok
  int fsave( const char* fname ); // return 0 for ok
  int fload( FILE* f ); // return 0 for ok
  int fsave( FILE* f ); // return 0 for ok

  String& operator []( const char* key )
    {
    detach(); // I don't know if user will change returned VString?!
    VTrieNode *node = box->root->find_node( key, 1 );
    ASSERT( node );
    if ( ! node->data )
      node->data = new VString();
    return *(node->data);
    }
  
  const VTrie& operator = ( const VTrie& tr )
    {
    box->unref();
    box = tr.box;
    box->ref();
    return *this; 
    };
  
  const VTrie& operator = ( const VArray& arr )
    { undef(); merge( (VArray*)&arr ); return *this; };
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
/*
** options are:
**               i -- case insensitive
**               m -- multiline matches
**               s -- single line (`.' matches and NEWLINE's)
**               x -- extended (ifnores whitespace and comments)
**
** for more docs see perlre(1) and pcre library docs
**
**
** WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING! WARNING!
** extracting of the captured substring is only possible while subject input
** line which is used as matching target is intact! which means that if you
** change this line between match and substring extraction this will lead to
** segmentation fault!
**
*/

/* number of subpatterns which can be catched by VRegexp::m() */
#define VREGEXP_MAX_SUBS    32

class VRegexp
{

  pcre*       re;
  pcre_extra *pe;
  int         sp[VREGEXP_MAX_SUBS*3]; // sub pointers
  int         rc;
  const char *lp; // last line matched ptr
  
  String substr;
  String errstr;

  int get_options( const char* opt );
  
  public:

  VRegexp();
  VRegexp( const char* pattern, const char *opt = NULL ); // compiles new regexp
  ~VRegexp(); 

  int comp( const char* pattern, const char *opt = NULL ); // compile re, return > 0 for success
  int study(); // optimizing regexp for (big-size) multiple matches
  int ok(); // return 1 if regexp is compiled ok, 0 if not
  
  int m( const char* line ); // execute re against line, return 1 for match
  int m( const char* line, const char* pattern, const char *opt = NULL ); // same as exec, but compiles first

  const char* sub( int n ); // return n-th substring match
  int sub_sp( int n ); // return n-th substring start position
  int sub_ep( int n ); // return n-th substring end position
  
  String& operator []( int n ) // same as sub()
    { substr = ""; sub( n ); return substr; }
    
  const char* error_str() { return errstr.data(); };  
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
** UTILITIES
**
****************************************************************************/

  // split `source' with `regexp_str' regexp
  VArray str_split( const char* regexp_str, const char* source, int maxcount = -1 );
  // split `source' with exact string `delimiter_str'
  VArray str_split_simple( const char* delimiter_str, const char* source, int maxcount = -1 );
  // join array data to single string with `glue' string
  // returns the result string or store to optional `dest'
  VString str_join( VArray array, const char* glue = "" );


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

