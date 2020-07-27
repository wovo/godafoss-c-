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

#include "i2c_bus.hpp"
#include "i2c_bus.cpp"

int main( void ){

   timing::init();
   gf::ostream< gf::formatter< uart > > cout;
   cout << __LINE__ << "\n";

   i2c_bus_c bus( i2c_bus_c::interface::interface_0, 100'000 );

   for(;;){
      uint8_t data[ 1 ] = { 0xAA };
      cout << __LINE__ << "\n";
      bus.write( 0x49, data, 1, 0, 0 );
      cout << __LINE__ << "\n";
      timing::ms< 200 >::wait();
   }
}
