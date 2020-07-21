// ==========================================================================
//
// Test ADC input
//
// (c) Wouter van Ooijen (wouter@voti.nl) 2017-2020
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
using adc     = target::adc1_0;

int main( void ){
   gf::ostream< gf::formatter< uart > > cout;
   timing::init();
   adc::init();
   timing::ms< 100 >::wait();
   for(;;){
      cout
         << "adc = 0x"
         << gf::hex << gf::setw( 3 ) << gf::hex << gf::setfill( '0' )
         << adc::read()
         << "\n";
      timing::ms< 1000 >::wait();
   }
}
