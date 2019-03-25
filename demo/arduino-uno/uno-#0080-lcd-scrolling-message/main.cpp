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

using lcd = godafoss::hd44780_rs_e_d_x_y_timing< 
   target::d8,
   target::d9,
   godafoss::port_out<
      target::d4,
      target::d5,
      target::d6,
      target::d7 >,
   16, 2,
   timing >; 
   
using bl = godafoss::direct< godafoss::pin_out< target::d10 > >;   

int main( void ){  
    bl::init(); 
    bl::write( 1 );

    gf::scrolling_text<
       lcd,
       timing::ms< 1'000 >,
       timing::ms<   300 >,
       timing::ms< 1'000 >
    >(  "We regret to inform you that we are closed for the day." );  
    
}
