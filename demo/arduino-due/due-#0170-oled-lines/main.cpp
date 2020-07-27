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
//using i2c_bus = gf::i2c_bus_bb_scl_sda< target::scl1, target::sda1, timing >;
using i2c_bus = target::i2c0< gf::i2c_fast >;
using oled = gf::oled< i2c_bus >;

int main( void ){
   gf::demo::random_lines2< oled, timing::ms< 50 > >();
}

