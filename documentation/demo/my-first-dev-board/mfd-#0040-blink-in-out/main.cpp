// ==========================================================================
//
// (c) Wouter van Ooijen (wouter@voti.nl) 2019
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

using color  = target::green;
using leds   = gf::port_out< 
   target::leds_border_together,
   target::led5,
   gf::pin_out_dummy >;

int main(){
   color::init();
   color::write( 1 );
   gf::walk< leds, timing::ms< 500 > >();
}  

