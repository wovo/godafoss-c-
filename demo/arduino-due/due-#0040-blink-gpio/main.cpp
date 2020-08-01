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
using led     = target::d13;

int main( void ){

   led::resources::initialize();
   led::direction_set_output();

   for(;;){

      led::write( 1 );led::flush();

      for( uint32_t i = 0; i < 200'000; i++ ){ asm( "nop" ); }

      led::write( 0 );led::flush();

      for( uint32_t i = 0; i < 200'000; i++ ){ asm( "nop" ); }

   }
}

