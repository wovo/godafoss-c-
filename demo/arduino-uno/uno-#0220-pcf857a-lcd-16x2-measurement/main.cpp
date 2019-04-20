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
using timing  = target::clocking; // <=========

using i2c_bus = gf::i2c_bus_bb_scl_sda< target::a0, target::a1, timing >;
using chip    = gf::pcf8574a< i2c_bus, 0b111 >;
using lcd     = gf::hd44780_rs_e_d_x_y_timing< 
   chip::p0,
   chip::p2,
   gf::port_out<
      chip::p4,
      chip::p5,
      chip::p6,
      chip::p7 >,
   16, 2,
   timing >; 

int main( void ){
   timing::init();     
   gf::direct< chip::p1 >::write( 0 );
   gf::direct< chip::p3 >::write( 1 );
   gf::ostream< gf::formatter< lcd > > cout;
    
   cout << "\f";
   auto t1 = timing::now_ticks();
   cout << 'x';
   auto t2 = timing::now_ticks();
    
   cout 
      << "\fwrite 1 char = \n"
      << (int) ((t2 - t1) / 16 )
      << " us";
}
