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
using adc = target::adc;

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
   adc::init();
   
   int r = 0;
   int g = 0;
   int b = 0;
   
   for(;;){
      if( sw_red::read() ){ r = adc::read(); }
      if( sw_green::read() ){ g = adc::read(); }
      if( sw_blue::read() ){ b = adc::read(); }
      
      colors::write( 0b0'001 );
      timing::wait_us( r );
      colors::write( 0b0'000 );
      timing::wait_us( adc::adc_max - r );      

      colors::write( 0b0'010 );
      timing::wait_us( g );
      colors::write( 0b0'000 );
      timing::wait_us( adc::adc_max - g );      

      colors::write( 0b0'100 );
      timing::wait_us( b );
      colors::write( 0b0'000 );
      timing::wait_us( adc::adc_max - b );      

   }   
}   