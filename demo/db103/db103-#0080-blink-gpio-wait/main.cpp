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
using led     = gf::pin_out< gf::direct< target::p1_4 >>;
using timing  = target::timing;

int main( void ){   
   
   led::init();
   timing::init();
   
   for(;;){
      
      led::write( 1 );

      timing::ms< 200 >::wait();

      led::write( 0 );

      timing::ms< 200 >::wait();

   }
}

