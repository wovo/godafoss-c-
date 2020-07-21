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
using led     = target::p1_3;

using leds = gf::all_pins<
   target::scl,
   target::sda,
   target::p0_6,
   target::p0_7,

   gf::invert< target::p1_0 >,
   gf::invert< target::p1_1 >,
   gf::invert< target::p1_2 >,
   gf::invert< target::p1_3 >,

   target::p1_4,
   target::p1_5,
   target::p1_8,
   target::p1_9
>;

int main( void ){
   gf::blink< leds, timing::ms< 250 > >();
}

