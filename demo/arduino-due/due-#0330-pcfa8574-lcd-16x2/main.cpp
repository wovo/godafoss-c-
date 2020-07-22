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

using i2c_bus = gf::i2c_bus_bb_scl_sda<
   target::scl1,
   target::sda1,
   timing >;
using chip    = gf::pcf8574< i2c_bus, 0x07 >;
using lcd     = gf::hd44780_rs_e_d_s_timing<
    chip::p0,
    chip::p2,
    gf::port_out_from_pins<
       chip::p4,
       chip::p5,
       chip::p6,
       chip::p7 >,
    { 16, 2 },
    timing >;
using rw      = gf::direct< chip::p1 >;
using led     = gf::direct< chip::p3 >;

int main( void ){

   rw::init();
   rw::write( 0 );

   led::init();
   led::write( 1 );

   gf::ostream< gf::formatter< lcd > > cout;

   cout << "\fHello world!\n== second line =";

}
