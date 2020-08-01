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
using uart    = target::uart<>;
using adc0     = target::a0;
using adc1     = target::a1;

using resources = gf::use< timing, adc0, adc1 >;

int main( void ){
   resources::initialize();
   gf::ostream< gf::formatter< uart > > cout;
   timing::ms< 2000 >::wait();
   for(;;){
      cout
         << "adc0 = 0x"
         << gf::hex << gf::setw( 3 ) << gf::hex << gf::setfill( '0' )
         << adc0::read().raw_value
         << "     "
         << "adc1 = 0x"
         << gf::hex << gf::setw( 3 ) << gf::hex << gf::setfill( '0' )
         << adc1::read().raw_value
         << "\n";
      timing::ms< 500 >::wait();
   }
}

