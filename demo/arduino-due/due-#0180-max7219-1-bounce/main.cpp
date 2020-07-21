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

static constexpr auto white = gf::black_or_white( false );
static constexpr auto black = gf::black_or_white( true );

using spi = gf::spi_bus_bb_sclk_miso_mosi<
   target::d10,
   gf::pin_in_dummy,
   target::d8,
   timing
>;
using window = gf::max7219< spi, target::d9, 1 >;

int main( void ){
   gf::demo::bouncing_square< window, timing::ms< 100 >, 2, { 1, 2 } > ();
}
