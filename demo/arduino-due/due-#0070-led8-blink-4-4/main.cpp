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

// 1 ledje doet het niet??

namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;
using leds    = gf::all_pins<
   gf::all_pins< target::d2, target::d3, target::d4, target::d5 >,
   gf::invert< gf::all_pins<target::d6, target::d7, target::d8, target::d9 > > >;

int main( void ){
   gf::run< gf::blink< leds, timing::ms< 200 > > >();
}
