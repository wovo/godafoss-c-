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
using timing  = target::timing;
using leds    = gf::port_out_from_pins<
   gf::all_pins< target::d2, target::d3 >,
   gf::all_pins< target::d4, target::d5 >,
   gf::all_pins< target::d6, target::d7 >,
   gf::all_pins< target::d8, target::d9 > >;

int main( void ){
   gf::kitt< leds, timing::ms< 100 > >();
}
