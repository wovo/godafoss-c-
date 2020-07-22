// ==========================================================================
//
// Hello world on an Arduino Uno
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

using i2c_bus = gf::i2c_bus_bb_scl_sda<
   target::scl1,
   target::sda1,
   timing >;
using chip    = gf::pcf8574< i2c_bus, 0x07 >;

int main( void ){
   gf::blink< chip::p3, timing::ms< 200 > >();
}
