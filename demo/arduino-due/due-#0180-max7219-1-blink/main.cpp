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
using led     = target::d13;

using spi = gf::spi_bus_bb_sclk_miso_mosi<
   target::d10,
   gf::pin_in_dummy,
   target::d8,
   timing
>;
using window = gf::max7219< spi, target::d9, 8 >;

int main( void ){
   //gf::blink< all, timing::us< 1 > >();
   //timing::init();
   //timing::ms< 1'000 >::wait();
/*if(0){
      chip::init();
   chip::clear( black );
   for( int i = 0; i < 64; ++i ){
      chip::write( chip::origin + chip::offset_t( i, i ), white );
      chip::flush();
      timing::ms< 200 >::wait();
   }
}
* */
   gf::run< gf::demo::random_lines2< window, timing::ms< 200 > > >();
}

