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
using uart    = target::uart<>;

using adc     = target::adc;

int main(){
   adc::init();
   timing::init();
   uart::init();
   timing::ms< 2000 >::wait();
   
   for(;;){
      auto v = adc::read() / 16;       
      uart::write( '|' );
      while( v-- ){ uart::write( '=' ); }
      uart::write( '\n' );
      timing::ms< 200 >::wait();
   }
}   