/*
 *
 *  VSTRING Library supporting structures and functions
 *  (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-2000
 *  Distributed under the GPL license, see end of this file for full text!
 *
 * $Id: vstrlib.cpp,v 1.4 2002/04/14 10:14:44 cade Exp $
 *
 */

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
    _max_len = 0;
    _min_len = (unsigned int)-1; /* should underflow to max value :) */
    _ret_str = (char*)NULL;
  }

  VArray::VArray( const VArray& arr )
  {
    _data = NULL;
    _size = 0;
    _count = 0;
    _max_len = 0;
    _min_len = (unsigned int)-1; /* should underflow to max value :) */
    _ret_str = (char*)NULL;
    *this = arr;
  }

  VArray::VArray( const VTrie& tr )
  {
    _data = NULL;
    _size = 0;
    _count = 0;
    _max_len = 0;
    _min_len = (unsigned int)-1; /* should underflow to max value :) */
    _ret_str = (char*)NULL;
    *this = tr;
  }

  VArray::~VArray()
  {
    zap();
  }

  void VArray::resize( int new_size )
  {
    ASSERT( new_size >= 0 );
    if ( new_size == 0 )
      {
      delete _data;
      _data = NULL;
      _size = 0;
      _count = 0;
      return;
      }
    new_size  = new_size / VARRAY_BLOCK_SIZE + (new_size % VARRAY_BLOCK_SIZE != 0);
    new_size *= VARRAY_BLOCK_SIZE;
    if ( new_size == _size ) return;
    String** new_data = new (String*)[ new_size ];
    ASSERT( new_data );
    memset( new_data, 0, new_size * sizeof(String*) );
    if ( _data )
      {
      memcpy( new_data, _data,
              (_size < new_size ? _size : new_size) * sizeof(String*) );
      delete _data;
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
    _data[n]->compact = 1;
    _data[n]->set( s );

    int sl = str_len( *_data[n] );
    if ( sl < _min_len )  _min_len = sl;
    if ( sl > _max_len )  _max_len = sl;
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

  void VArray::zap()
  {
    if ( _count )
      for( int z = 0; z < _count; z++ )
        if ( _data[z] )
          delete _data[z];
    resize( 0 );

    _data = NULL;
    _size = 0;
    _count = 0;
    _max_len = 0;
    _min_len = (unsigned int)-1; /* should underflow to max value :) */
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
      if ( str_get_ch( str, -1 ) != '\n' ) continue;
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

  void VArray::sort()
  {
    if ( count() > 1 )
      q_sort( 0, count() - 1 );
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
    zap();
    
    regexp *re = regcomp( res );
    ASSERT( re );

    const char* ps = str;

    String s;
    while( regexec( re, ps ) )
      {
      if ( maxcount != -1 )
        {
        maxcount--;
        if ( maxcount == 0 ) break;
        }
      int l = re->startp[0] - ps;
      s.setn( ps, l );
      push( s );
      ps = re->endp[0];
      }
    push( ps );

    free( re );
  };

  void VArray::split_str( const char* res, const char* str, int maxcount )
  {
    const char* ps = str;
    const char* fs;
    
    zap();
    
    int rl = strlen( res );
    
    String s;
    while( fs = strstr( ps, res ) )
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

  const char* VArray::join( const char* glue )
  {
    _ret_str = "";
    for( int z = 0; z < _count-1; z++ )
      {
      _ret_str += get( z );
      _ret_str += glue;
      }
    _ret_str += get( _count-1 );
    return _ret_str.data();
  };

  void VArray::print()
  {
    int z;
    for( z = 0; z < count(); z++ )
      printf( "%d=%s\n", z, get(z) );
  }

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
    root = new VTrieNode();
    _nodes++;
  }

  VTrie::VTrie( const VArray& arr )
  {
    _count = 0;
    _depth = 0;
    _nodes = 0;
    root = new VTrieNode();
    _nodes++;
    *this = arr;
  };

  VTrie::VTrie( const VTrie& tr )
  {
    _count = 0;
    _depth = 0;
    _nodes = 0;
    root = new VTrieNode();
    _nodes++;
    *this = tr;
  };


  VTrie::~VTrie()
  {
    zap_node( root );
  }

  void VTrie::zap_node( VTrieNode *node )
  {
    if ( node->next ) zap_node( node->next );
    if ( node->down ) zap_node( node->down );
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
        return; /* not found */
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
      if ( ! current->data ) current->data = new String();
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
    current->data->set( data );
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

  void VTrie::zap()
  {
    zap_node( root );
    root = new VTrieNode();
    _nodes++;
  };

  int VTrie::keys( VArray* arr )
  {
    arr->zap();
    temp_key = "";
    key_node( root, arr );
    return arr->count();
  };

  int VTrie::values( VArray* arr )
  {
    arr->zap();
    temp_key = "";
    value_node( root, arr );
    return arr->count();
  };

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

