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
using uart    = target::uart<>;

template< gf::is_i2c_profile profile >
void show_timing(){

//   gf::ostream< gf::formatter< uart > > cout;

   using i2c_bus = gf::i2c_bus_bb_scl_sda<
      target::scl1,
      target::sda1,
      timing,
      profile >;
   using oled = gf::oled< i2c_bus >;

   gf::use< oled >::initialize();
//   auto t = timing::now_us();
   oled::flush();
//   auto t2 = timing::now_us();

//   cout << "flush() takes " << t2 - t1 << " us\n";

}

int main( void ){
   gf::use< timing >::initialize();
   timing::ms< 2000 >::wait();
   show_timing< gf::i2c_standard >();
   show_timing< gf::i2c_fast >();
}

