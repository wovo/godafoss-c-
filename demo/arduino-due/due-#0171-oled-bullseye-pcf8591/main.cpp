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

namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;

using i2c_bus_sw = gf::i2c_bus_bb_scl_sda<
   target::scl1, target::sda1, timing >;
using i2c_bus_hw = target::i2c0<>;

using i2c_bus  = i2c_bus_sw;
using oled     = gf::flip_vertically< gf::oled< i2c_bus > >;
using chip     = gf::pcf8591< i2c_bus >;

int main( void ){
   gf::use< oled, chip::adc0, chip::adc1 >::initialize();
   for(;;){
      chip::adc0::refresh();
      chip::adc1::refresh();
      auto pos = oled::offset_t(
         chip::adc0::read().of( oled::size.x - 1 ),
         chip::adc1::read().of( oled::size.y - 1 ) );

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

