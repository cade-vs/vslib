/*
 *
 * (c) Vladi Belperchinov-Shabanski "Cade" <cade@biscom.net> 1998-1999
 *
 * SEE `README',`LICENSE' OR `COPYING' FILE FOR LICENSE AND OTHER DETAILS!
 *
 * $Id: vslib.cpp,v 1.2 2001/10/28 13:53:02 cade Exp $
 *
 */

#ifdef TEST

#include <vslib.h>

int main( int argc, char* argv[] )
{

  VTrie tr;
  VArray va;

  // eto kak se mushka v hash-a (t.e. trie-to de:))
  tr[ "tralala" ] = "data1";
  tr[ "opala"   ] = "data2";
  tr[ "keynext" ] =  "data3";

  // eto kak se mushka v masiva
  va.push( "this" );
  va.push( "just" );
  va.push( "test" );
  va.push( "simple" );

  // da dobavis stringche kum kraq na string ot masiva
  va[1] += " x2";

  // da oburnesh masiva do hash i da go dobavish kum sushtestvuvasht hash
  tr += va; // same as: tr.merge( &va );

  // chistene na masiva
  va.zap();

  // vzimane na key-ovete ot hash-a tr vuv masiva va, vrushta broikata,
  // t.e. `i' e ravno na va.count()
  int i = tr.keys( &va );

  printf( "keys count = %d\n", i );

  // kak se razpechatva masiva i stoinostite na keyovete ot hasha
  for( i = 0; i < va.count(); i++ )
    {
    printf( "%d -> %s (%s)\n", i, va[i].data(), tr[ va[i] ].data() );
    }

  VArray v1;

  printf( "--------------------\n" );
  v1 = tr; // same as: v1.zap; v1.merge( &tr );
  v1.print();

  printf( "--------------------\n" );
  printf( "adler32 = %X\n", (unsigned int)str_adler32( "This is a test" ));

  printf( "--------------------\n" );

  // kak se kompilira re
  VRegexp re( "a([0-9]+)" );

  if( re.m( "tralala85." ) )
    printf( "sub 1 = %s\n", re[1] ); // re[1] e purviq izraz ot skobite

  printf( "--------------------\n" );
  v1.zap();
  v1.split( " +", "tralala  opala and another   one" );
  v1.print();

  printf( "joined: %s\n", v1.join( "---" ) );

  printf( "--------------------\n" );
  v1.zap();
  v1.split( " +", "tralala  opala and another   one", 3 );
  v1.print();

  printf( "--------------------\n" );
  v1[1] = "hack this one here"; // mushkane na poziciq
  str_sleft( v1[2], 11 ); // rqzane na chast ot stringcheto
  v1[0] = 12345; // mushkane na chislo :))
  v1.print();

  printf( "--------------------\n" );
  VArray aa[3]; // masiv ot masivi

  aa[0].split( " ", "this is just a simple test" );
  aa[1].split( " ", "never ending story" );
  aa[2].split( " ", "star-wars rulez" );

  aa[0][1] = "was"; // purvi masiv, vtora kletka... vmesto `is'
  aa[2][0] = "slackware"; // trti masiv purva kletka... vmesto `star-wars'

  // avtomatichno expandva masiva do 11 elementa, predi tova
  // e bil samo 3...
  aa[1][10] = "king of the hill";

  for( i = 0; i < 3; i++ )
    {
    printf("---\n");
    aa[i].print();
    }

  return 0;
}
#endif

// eof vslib.cpp
