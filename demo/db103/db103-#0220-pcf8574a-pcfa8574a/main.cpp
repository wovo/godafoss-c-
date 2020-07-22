// ==========================================================================
//
// kitt on 8 LEDs on PCF8574A connected to a DB013 board (LPC1114)
//
// (c) Wouter van Ooijen (wouter@voti.nl) 2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

#include "godafoss.hpp"

namespace gf     = godafoss;
using target     = gf::target<>;
using timing     = target::timing;

using i2c_bus_1  = gf::i2c_bus_bb_scl_sda< target::scl, target::sda, timing >;
using chip_1     = gf::pcf8574a< i2c_bus_1 >;

using i2c_bus_2  = gf::i2c_bus_bb_scl_sda< chip_1::p6, chip_1::p7, timing >;
using chip_2     = gf::pcf8574a< i2c_bus_2 >;

int main( void ){
   gf::blink< chip_2::p4, timing::ms< 200 > >();
}
