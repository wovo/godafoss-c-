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
using led     = target::d13;
using timing  = target::timing;

int main( void ){
   //gf::run< gf::blink< led, 200 * gf::ms > >();
   gf::run< gf::blink< led, timing::ms< 200 > > >();
}
