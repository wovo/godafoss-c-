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

using led    = target::leds_together;
using colors = gf::pin_out_from_pins< 
   target::blue,
   gf::invert< target::green >>;

int main(){
   led::init();
   led::write( 1 );
   gf::blink< colors, timing::ms< 500 > >();
}   

