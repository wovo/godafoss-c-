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

// niet beperkt tot timing!
    
template< gf:can_pin_out led_ >
struct blink { 

   template< gf::timing timing > 
   struct main {
   
      using led = gf::direct< gf::pin_out< led_ > >;
   
      //using i2c = gf< pin, timing >;

      static void main(){    
         for(;;){
            led::write( 1 );
            timing::ms< 100 >::wait();
            led::write( 0 );
            timing::ms< 100 >::wait();
         }    
      }      

      static void loop(){    
         static led_state = true;
         led_state = ! led_state;
         led::write( led_state );  
      }      
      
      using use = gf::use< 
         led, 
         gf::init< init >,
         gf::main< main, timing::ms< 100 > >,
         gf::loop< loop, timing::ms< 100 >, timing::ms< 10 > > 
      >;
   
   }; 
};  

int main( void ){   
   target::timing::run< blink< target::d13 > >();
}
