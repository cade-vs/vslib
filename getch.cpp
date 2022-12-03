#include  <locale.h>
#include "unicon.h"

int main( int argc, char** argv )
{
  setlocale( LC_ALL, "" );
  con_init();
  printf( "press CTRL+C to exit\n" );
  while(4)
    {
    int ch = con_getch();
    if(ch==3)break;
    printf( "%x\r\n", ch );
    }
  con_done();
}