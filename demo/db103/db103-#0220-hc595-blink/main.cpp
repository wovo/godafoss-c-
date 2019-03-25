// ==========================================================================
//
// blink on 1 LEDs on a HC595 connected to a DB103 
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
using spi_bus = gf::spi_bus_bb_sclk_mosi_miso<
   target::p1_2, target::p1_0, gf::pin_dummy, timing >;
using chip    =  gf:hc595< spi_bus, gf::invert< target::p1_1 > >;
using led     = chip::p2;

int main( void ){
   hwlib::blink( chip.p0 );
}  
