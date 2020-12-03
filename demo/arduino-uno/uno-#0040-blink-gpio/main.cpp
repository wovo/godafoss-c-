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
using led     = gf::direct< gf::pin_out<  target::d13 > >;
using timing  = target::waiting;
    
    
struct blink {
    
   using resources = gf::use< led, timing >;
    
   static void run(){    
      for(;;){
         led::write( 1 );
         timing::ms< 100 >::wait();
         led::write( 0 );
         timing::ms< 100 >::wait();
      }    
   }      

};   

int main( void ){   
   gf::run< blink >();
}
