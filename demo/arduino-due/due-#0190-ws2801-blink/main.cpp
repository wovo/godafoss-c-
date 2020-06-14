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
using bus = gf::spi_bus_bb_sclk_mosi<
   target::d2,
   target::d3,
   timing >;
using chip = gf::ws2801< bus, timing, 3 >;

int main( void ){
   chip::init();
   chip::clear( gf::green );
   chip::write( chip::origin + chip::location_t( 0, 0 ), gf::red );
   chip::write( chip::location_t( 1, 0 ), gf::blue );
   chip::write( chip::location_t( 2, 0 ), gf::red );
   chip::flush();
}

