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
using uart    = target::uart<>;

//using i2c_bus = gf::i2c_bus_bb_scl_sda< target::scl1, target::sda1, timing >;
using i2c_bus = target::i2c0<>;

using chip    = gf::pcf8591< i2c_bus >;

int main( void ){

   timing::init();
   chip::init();
   gf::ostream< gf::formatter< uart > > cout;
   timing::ms< 2000 >::wait();

   for(;;){
      cout
         << "adc0: " << gf::setw( 4 ) << chip::adc0::read().raw_value << "   "
         << "adc1: " << gf::setw( 4 ) << chip::adc1::read().raw_value << "   "
         << "adc2: " << gf::setw( 4 ) << chip::adc2::read().raw_value << "   "
         << "adc3: " << gf::setw( 4 ) << chip::adc3::read().raw_value << "\n";
      timing::ms< 250 >::wait();
   }
}
