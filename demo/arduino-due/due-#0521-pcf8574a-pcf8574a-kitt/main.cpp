// ==========================================================================
//
// kitt on 8 LEDs on a HC595 connected to a DB103 board (LPC1114)
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

// using i2c_bus_1 = gf::i2c_bus_bb_scl_sda< target::scl1, target::sda1, timing >;

using i2c_bus_1 = target::i2c0<>;
using chip_1    = gf::pcf8574a< i2c_bus_1, 0x01 >;

using i2c_bus_2 = gf::i2c_bus_bb_scl_sda< chip_1::p0, chip_1::p1, timing >;
using chip_2    = gf::pcf8574a< i2c_bus_2, 0x03 >;

int main( void ){
   gf::kitt< gf::invert< chip_2 >, timing::ms< 30 >  >();
}
