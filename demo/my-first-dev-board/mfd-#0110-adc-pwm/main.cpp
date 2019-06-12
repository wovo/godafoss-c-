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

using adc     = target::adc;
using colors = target::colors;
using led    = target::leds_together;

int main(){
   led::init();
   led::write( 1 );
   colors::init();
   adc::init();
   
   for(;;){
      auto x = adc::read();
      colors::write( 0b0'100 );
      timing::wait_us( x );
      colors::write( 0b0'000 );
      timing::wait_us( adc::adc_max - x );
   }
}   