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
  va.zap();

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
  v1 = tr;    // same as: v1.zap; v1.merge( &tr );
  v1.print(); // print array data

  VRegexp re( "a([0-9]+)" ); // compiling new regexp

  if( re.m( "tralala85." ) ) // match against regexp
    printf( "sub 1 = %s\n", re[1].data() ); // re[1] returns `85'

  printf( "--------------------\n" );
  v1.zap(); // clear
  v1.split( " +", "tralala  opala and another   one" ); // splits on spaces
  v1.print();

  printf( "joined: %s\n", v1.join( "---" ) ); // join the same data back

  printf( "--------------------\n" );
  v1.zap();
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

int main( int argc, char* argv[] )
{
  test1();
  test2();
  test3();
  
  return 0;
}
