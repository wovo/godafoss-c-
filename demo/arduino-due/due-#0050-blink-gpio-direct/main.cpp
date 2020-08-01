// ==========================================================================
//
// blink the LED on an Arduino Due
//
// (c) Wouter van Ooijen (wouter@voti.nl) 2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

#include "godafoss.hpp"

namespace gf  = godafoss;
using target  = gf::target<>;
using led     = gf::pin_out_from< gf::direct< target::d13 >>;

int main( void ){

   led::resources::initialize();

   for(;;){

      led::write( 1 );

      for( uint32_t i = 0; i < 1'000'000; i++ ){ asm( "nop" ); }

      led::write( 0 );

      for( uint32_t i = 0; i < 1'000'000; i++ ){ asm( "nop" ); }

   }
}

