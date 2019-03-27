// ==========================================================================
//
// Hello world on an Arduino Uno
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
using adc     = target::adc0;

using lcd = gf::hd44780_rs_e_d_x_y_timing< 
   target::d8,
   target::d9,
   gf::port_out<
      target::d4,
      target::d5,
      target::d6,
      target::d7 >,
   16, 2,
   timing >; 
   
using bl = gf::direct< gf::pin_out< target::d10 > >;   

int main( void ){  
   gf::ostream< gf::formatter< lcd > > cout;     
   bl::init(); 
   bl::write( 1 );
    
   adc::init();
   timing::init();
   for(;;){
      cout 
         << "\f"
         << "adc = 0x" 
         << gf::hex << gf::setw( 3 ) << gf::hex << gf::setfill( '0' )
         << adc::read()
         << "\n";
      timing::ms< 200 >::wait();
   }

}
