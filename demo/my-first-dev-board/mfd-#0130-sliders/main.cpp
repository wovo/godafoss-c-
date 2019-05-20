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

using color = target::blue;

using sw5    = target::sw5;
using leds_5 = gf::pin_out_from_pins< 
   gf::invert< target::led1 >, target::led4 >;
   
using sw6    = target::sw6;
using leds_6 = gf::pin_out_from_pins< 
   gf::invert< target::led2 >, target::led5 >;

int main(){
   leds_5::init();
   leds_6::init();
   color::init();
   color::write( 1 );
   sw5::init();
   sw6::init();
   
   for(;;){
      leds_5::write( sw5::read() );
      leds_6::write( sw6::read() );
   }
}   