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
using i2c_bus = gf::i2c_bus_bb_scl_sda<
   target::scl, target::sda, timing >;
using chip    = gf::pcf8574a< i2c_bus >;

int main( void ){ 
   gf::kitt< gf::invert< chip >, timing::ms< 50 > >(); 
}  

