// ==========================================================================
//
// (c) Wouter van Ooijen (wouter@voti.nl) 2019
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt) 
//
// ==========================================================================

#include "godafoss.hpp"

namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;

using color   = target::blue;

using led_a = gf::pin_out_from_pins<
   target::led1, gf::invert< target::led4 > >;
using led_b = gf::pin_out_from_pins<
   target::led3, gf::invert< target::led6 > >;

using sw_a = target::sw5;
using sw_b = target::sw6;


int main(){;
   color::init();
   color::write( 1 );
   
   led_a::init();
   led_b::init();
   
   sw_a::init();
   sw_b::init();

   for(;;){
      led_a::write( sw_a::read() );
      led_b::write( sw_b::read() );
   }
}   