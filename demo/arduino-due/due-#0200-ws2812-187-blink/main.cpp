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

namespace gf = godafoss;
using target = gf::target<>;
using timing = target::timing;
using chip = gf::ws2812< target::d11, timing, 187 >;
//using leds = gf::pin_out_from< gf::port_from_window< gf::monochrome< chip, gf::black, gf::blue > > >;

int main( void ){
//   gf::blink< leds, timing::ms< 10 > >();
}

