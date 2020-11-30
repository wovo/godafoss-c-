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

using lcd = gf::hd44780_rs_e_d_s_timing< 
   target::d8,
   target::d9,
   godafoss::port_out_from_pins<
      target::d4,
      target::d5,
      target::d6,
      target::d7 >,
   { 16, 2 },
   // timing >; 
   gf::slow_down< timing, 1 > >; 
   
using bl = godafoss::direct< godafoss::pin_out< target::d10 > >;   

int main( void ){
    gf::ostream< gf::formatter< lcd > > cout;   
     
    bl::init(); 
    bl::write( 1 );
    
    cout 
       << "\fHello world!        | this is only for a"
       << "\nnice to meet you    | 40 col. display." 
       << "\t0002line 3 ============X"
       << "\nline 4 ============X"
       ;// << hwlib::flush;

}
