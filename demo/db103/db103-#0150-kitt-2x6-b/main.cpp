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

using leds_a = gf::port_out_from_pins<
   target::scl,
   target::sda,
   target::p0_6,
   target::p0_7,
   target::p1_0,
   target::p1_1
>;

using leds_b = gf::port_out_from_pins<
   target::p1_2,
   target::p1_3,
   target::p1_4,
   target::p1_5,
   target::p1_8,
   target::p1_9
>;

/*
using leds = gf::port_out_from_pins<
   leds_a,
   leds_b
>;
*/

int main( void ){
   // doesn't work yet: port_out from 2 port_out's
   // gf::kitt< leds, timing::ms< 50 > >();
}

