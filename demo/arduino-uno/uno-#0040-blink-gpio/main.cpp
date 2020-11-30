// ==========================================================================
//
// blink the LED on an Arduino Uno
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
using led     = gf::pin_out< gf::direct< target::d13 >>;
using timing  = target::timing;

int main( void ){   
   
   led::resources::initialize();
   timing::resources::initialize();
   
   for(;;){
      
      led::write( 1 );

      timing::ms< 100 >::wait();

      led::write( 0 );

      timing::ms< 100 >::wait();

   }
}
