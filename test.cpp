#include <stdio.h>
#include "vstring.h"
#include "vstrlib.h"

void test1()
{
  String str = "Hello";
  str += " World"; // str is `Hello World' now
  str_reverse( str ); // str is `dlroW olleH' now
  str_low( str ); // lower case

  VArray va;
  va.split( " +", str ); // array contains `dlroW' at pos 0 and `olleH' at 1
  
  va.reverse(); // array reversed: `dlroW' at pos 1 and `olleH' at 0
  
  int z;
  for( z = 0; z < va.count(); z++ )
    {
    str_reverse( va[z] ); // reverses each string element
    }
  
  
  str = va.join( " " ); // joins into temporary string

  printf( "************************ test 1 result is: %s\n", str.data() ); // this should print `hello world'
}

void test2()
{
  VArray va;
  va.push( "hello" ); // pos 0
  va.push( "world" ); // pos 1
  
  va.ins( 1, "your" ); // pos 1 shifted
  
  va[1] = "my"; // replaces `your'
  va[3] = "!";  // set outside the size, array is extended
  
  String str = va.pop(); // pops last element, str is now `!'
  
  va.join( "-", &str ); // joins to given string
  
  str_tr( str, "-", " " ); // replaces dashes with spaces
  
  str_replace( str, " my ", " " ); // removes ` my '
  
  printf( "************************ test 2 result is: %s\n", str.data() ); // this should print `hello world'
}

void test3()
{
  VTrie tr; // hash-like
  VArray va;
  
  // inserting keys and values
  tr[ "tralala" ] = "data1";
  tr[ "opala"   ] = "data2";
  tr[ "keynext" ] =  "data3";

  // inserting elements into the array
  va.push( "this" );
  va.push( "just" );
  va.push( "test" );
  va.push( "simple" );

  // adding string to the first element of the array
  va[1] += " x2";

  // the array is converted to trie (hash) and merged into `tr'
  tr += va; // same as: tr.merge( &va );

  // clear the array--remove all elements
  va.undef();

  // take keys from `tr' as array and store them into va, returns count
  // i.e. i = tr.count();
  int i = tr.keys( &va );

  printf( "keys count = %d\n", i );

  // printing the array and trie data
  for( i = 0; i < va.count(); i++ )
    {
    printf( "%d -> %s (%s)\n", i, va[i].data(), tr[ va[i] ].data() );
    }

  VArray v1;

  printf( "--------------------\n" );
  v1 = tr;    // same as: v1.undef; v1.merge( &tr );
  v1.print(); // print array data

  VRegexp re( "a([0-9]+)" ); // compiling new regexp

  if( re.m( "tralala85." ) ) // match against regexp
    printf( "sub 1 = %s\n", re[1].data() ); // re[1] returns `85'

  if( re.m( "tralala85.", "(la)+" ) ) // match against regexp
    {
    printf( "sub 0 = %s\n", re[0].data() ); // `lala'
    printf( "sub 1 = %s\n", re[1].data() ); // `la'
    }

  printf( "--------------------\n" );
  v1.undef(); // clear
  v1.split( " +", "tralala  opala and another   one" ); // splits on spaces
  v1.print();

  printf( "joined: %s\n", v1.join( "---" ) ); // join the same data back

  printf( "--------------------\n" );
  v1.undef();
  v1.split( " +", "tralala  opala and another   one", 3 ); // splits data on spaces up to 3 elements
  v1.print();

  printf( "--------------------\n" );
  v1[1] = "hack this one here"; // set (overwrite) element 1
  str_sleft( v1[2], 11 ); // reset element 2 to the left 11 chars only
  v1[0] = 12345; // convert integer into string
  v1.print();

  printf( "--------------------\n" );
  VArray aa[3]; // array of arrays

  aa[0].split( " ", "this is just a simple test" );
  aa[1].split( " ", "never ending story" );
  aa[2].split( " ", "star-wars rulez" );

  aa[0][1] = "was"; // first array, second element, replaces `is' with `was'
  aa[2][0] = "slackware"; // third array, first element, `star-wars' is now `slackware'

  // expands the array from 3 to 11 elements
  aa[1][10] = "king of the hill";

  for( i = 0; i < 3; i++ )
    {
    printf("---\n");
    aa[i].print();
    }
  printf( "************************ test 3 ends here\n" );
}

void test4()
{
  // this is regression test, please ignore it...

  int i;
  int ii;
  
  VArray va;
  ii = 20;
  i = ii;
  while( i-- )
    {
    va.undef();
    va.split( ",", "this is, just a simple. but fixed, nonsense test, voila :)" );
    printf( "%d%% va count = %d\n", (100*i)/ii, va.count() );
    }
  
  String set;
  String cat;
  String setn;
  String catn;
  String sete;
  String setp;

  i = 2000;
  
  while( i-- )
    {
    set.set( "this is, just a simple. but fixed, nonsense test, voila :)" );
    cat.cat( "this is, just a simple. but fixed, nonsense test, voila :)" );
    setn.setn( "this is, just a simple. but fixed, nonsense test, voila :)", 20 );
    catn.catn( "this is, just a simple. but fixed, nonsense test, voila :)", 20 );
    
    sete = "this is, just a simple. but fixed, nonsense test, voila :)";
    setp += "this is, just a simple. but fixed, nonsense test, voila :)";
    }

  printf( "set  = %d\n", str_len( set  ) );
  printf( "cat  = %d\n", str_len( cat  ) );
  printf( "setn = %d\n", str_len( setn ) );
  printf( "catn = %d\n", str_len( catn ) );
  printf( "sete = %d\n", str_len( sete ) );
  printf( "setp = %d\n", str_len( setp ) );

  printf( "--------------------\n" );
  
  i = 2000;
  while( i-- )
    {
    set = "this is, just a simple. but fixed, nonsense test, voila :)";
    setn = set;
    str_del( set, 20, 10 );
    str_ins( set, 30, "***opa***" );
    str_replace( setn, "i", "[I]" );
    }
  printf( "set  = %s\n", set.data() );
  printf( "setn = %s\n", setn.data() );
  
  printf( "---array sort-------\n" );
  va.undef();
  va.split( "[, \t]+", "this is, just a simple. but fixed, nonsense test, voila :)" );
  va.sort();
  va.print();
  printf( "--------------------\n" );
  va.sort( 1 );
  va.print();
  printf( "--------------------\n" );
  
}

void test5()
{
  VTrie tr; // hash-like
  VArray va;
  
  // inserting keys and values
  tr[ "key1" ] = "data1";
  tr[ "key2" ] = "data2";
  tr[ "key3" ] = "data3";
  
  tr.print();
  tr.reverse();
  tr.print();
  tr.reverse();
  tr.print();

  VCharSet cs;
  
  cs.push( 'a' );
  printf( "char_set: %d, %d\n", cs.in( 'a' ), cs.in( 'z' ) );
  cs.undef( 'a' );
  printf( "char_set: %d, %d\n", cs.in( 'a' ), cs.in( 'z' ) );
  cs.undef();

  int i = 2000;
  while( i-- )
    {
    cs.push( i );
    }
  cs.undef();  

  
  printf( "************************ test 5 ends here\n" );
}

void test6()
{
  VRegexp re;
  VArray va;
  
  re.comp( "^([^!]+)!(.+)=apquxz(.+)$" );
  int i = re.m( "abc!pqr=apquxz.ixr.zzz.ac.uk" );
  i--;
  while( i >= 0 )
    {
    va.push( re[i] );
    i--;
    }
  va.print();
  
  va.undef();
  va += "/this/is/samle/file.tail";
  va += "/file.tail";
  va += "/this/is/./samle/file.tail/";
  va += "/this/..../is/../samle/.file.tail";
  va += "/.file.tail";
  va += "/";
  
  const char* ps;
  
  va.reset();
  while( ps = va.next() )
    {
    printf( "------------------------------------\n" );
    printf( "file is: %s\n", ps );
    printf( "path is: %s\n", (const char*)str_file_path( ps ) );
    printf( "name is: %s\n", (const char*)str_file_name( ps ) );
    printf( "ext  is: %s\n", (const char*)str_file_ext( ps ) );
    printf( "n+ex is: %s\n", (const char*)str_file_name_ext( ps ) );
    printf( "reduced path is: %s\n", (const char*)str_reduce_path( ps ) );
    printf( "dot reduce sample is: %s\n", (const char*)str_dot_reduce( ps, 10 ) );
    }
}

int main( int argc, char* argv[] )
{
  test1();
  test2();
  test3();
  test4();
  test5();
  test6();
  return 0;
}
