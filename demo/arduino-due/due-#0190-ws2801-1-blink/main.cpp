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
   target::d12,
   target::d13,
   timing >;
using chip = gf::ws2801< bus, timing, 9 >;
//using chip = gf::recolor< _chip, gf::color, []( const gf::color & c ){ return - c; } >;

//using port = gf::port_from_window< gf::invert< gf::monochrome< chip, gf::black, gf::red > > >;
//using mono = gf::monochrome< chip, gf::black, gf::red >;
using port = gf::port_from_window< gf::monochrome< chip, gf::black, gf::red > >;


int main( void ){
   gf::run< gf::kitt< port, timing::ms< 20 > > >();
   /*
   chip::init();
   chip::clear( gf::green );
   chip::write( chip::origin + chip::offset_t( 0, 0 ), gf::red );
   chip::write( chip::origin + chip::offset_t( 1, 0 ), gf::blue );
   chip::write( chip::origin + chip::offset_t( 2, 0 ), gf::green );
   chip::flush();
   * */
}

