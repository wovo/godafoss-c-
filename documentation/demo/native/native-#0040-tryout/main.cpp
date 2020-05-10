#include "godafoss.hpp"
#include <iostream>

namespace gf = godafoss;
using target = gf::target<>;
using timing = target::timing;

//using w = target::window< gf::xy( 128, 64 ) >;

int main( void ){
   for( int i = 0; i < 20; ++i ){
      std::cout << timing::now_ticks() << "\n";
   }
   //char c;
   //std::cin >> c;
   //(void) c;
   timing::wait_ticks_function( 2'000'000 );
   timing::ms< 1'000 >::wait();
      std::cout << timing::now_ticks() << "\n";
   
}