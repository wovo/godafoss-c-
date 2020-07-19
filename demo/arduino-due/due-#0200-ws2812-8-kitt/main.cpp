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
using leds    = gf::ws2812< target::d11, timing, 8 >;
using port    = gf::port_from_window< gf::monochrome< leds, gf::black, gf::yellow > >;

int main( void ){
   gf::kitt< port, timing::ms< 50 > >();
}

