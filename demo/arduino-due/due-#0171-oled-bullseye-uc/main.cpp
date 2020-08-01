// ==========================================================================
//
// blink the LED on an Arduino Due
//
// (c) Wouter van Ooijen (wouter@voti.nl) 2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

#include "godafoss.hpp"

namespace gf = godafoss;
using target = gf::target<>;
using timing = target::timing;

using adx     = target::a0;
using ady     = target::a1;

using i2c_bus_sw = gf::i2c_bus_bb_scl_sda<
   target::scl1,
   target::sda1,
   timing,
   gf::i2c_fast >;
using i2c_bus_hw = target::i2c0< gf::i2c_fast >;
using oled = gf::flip_vertically< gf::oled< i2c_bus_sw > >;

int main( void ){
   gf::use< oled, adx, ady >::initialize();
   for(;;){
      adx::refresh();
      ady::refresh();
      auto pos = oled::offset_t(
         adx::read().of( oled::size.x - 1 ),
         ady::read().of( oled::size.y - 1 ) );

      oled::clear( gf::white );
      gf::circle< oled >( oled::origin + pos, 6, gf::black ).write();
      gf::circle< oled >( oled::origin + pos, 8, gf::black ).write();

      auto midsize  = oled::size / 2;
      gf::line< oled >(
         oled::origin + oled::offset_t( oled::size.x / 2 - 1, 0 ),
         oled::offset_t( 0, oled::size.y ),
         gf::black ).write();
      gf::line< oled >(
         oled::origin + oled::offset_t( 0, oled::size.y / 2 ),
         oled::offset_t( oled::size.x, 0 ),
         gf::black ).write();

      oled::flush();
   }
}
