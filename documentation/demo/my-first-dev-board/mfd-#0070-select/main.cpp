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

using colors = target::colors;
using led    = target::leds_together;

using sw_black  = target::sw1;
using sw_red    = target::sw2;
using sw_green  = target::sw3;
using sw_blue   = target::sw4;

int main(){
   led::init();
   led::write( 1 );
   colors::init();
   colors::write( 0 );
   sw_red::init();
   sw_green::init();
   sw_blue::init();
   sw_black::init();
   sw_red::pullup_enable();
   sw_green::pullup_enable();
   sw_blue::pullup_enable();
   sw_black::pullup_enable();
   
   for( uint8_t c = 0;;){
      if( sw_black::read() ){ c =  0; }
      if( sw_red::read() ){ c |= 0b0'001; }
      if( sw_green::read() ){ c |= 0b0'010; }
      if( sw_blue::read() ){ c |= 0b0'100; }
      colors::write( c );
   }
}   