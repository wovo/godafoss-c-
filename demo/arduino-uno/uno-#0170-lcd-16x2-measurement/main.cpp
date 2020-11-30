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
using timing  = target::clocking;  

using lcd = gf::hd44780_rs_e_d_x_y_timing< 
   target::d8,
   target::d9,
   godafoss::port_out<
      target::d4,
      target::d5,
      target::d6,
      target::d7 >,
   16, 2,
   // timing >; 
   godafoss::slow_down< timing, 1 > >; 
   
using bl = godafoss::direct< godafoss::pin_out< target::d10 > >;   

int main( void ){
    gf::ostream< gf::formatter< lcd > > cout;   
     
    timing::init();     
    bl::init(); 
    bl::write( 1 );
    
    cout << "\f";
    auto t1 = timing::now_ticks();
    cout << 'x';
    // timing::us< 800 >::wait();
    auto t2 = timing::now_ticks();
    
    cout 
       << "\fwrite 1 char = \n"
       << (int) ((t2 - t1) / 16 )
       << " us";


}
