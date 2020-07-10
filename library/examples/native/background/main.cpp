// =============================================================================
//
// background example
//
// =============================================================================
//
// Copyright
//    Wouter van Ooijen (wouter@voti.nl) 2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================

// @quote text
#include "godafoss.hpp"
namespace gf  = godafoss;

struct background_work: gf::background {

   auto last = gf::target::now_ms();

   void work() override(){
      now = gf::target::now_ms();
      if( now > last + 1'000 ){
         gf::cout << "Another second has passed\n";
      }
   }

};

int main(){

   {
      background_work annnoying;
      for( int i = 0; i < 10; ++i ){
         gf::wait_ms( 2'800 );
         gf::cout << "[" << i << "] 2.8 seconds passed\n"
      }
      // annnoying is destructed here, so it will finally shut up
   }

   for( int i = 0; i < 10; ++i ){
      gf::wait_ms( 2'100 );
      gf::cout << "[" << i << "] 2.1 seconds passed\n"
   }

};
// @quote end
