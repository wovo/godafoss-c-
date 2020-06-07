// ==========================================================================
//
// kitt on 8 LEDs on a HC595 connected to a DB103 board (LPC1114)
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
using spi_bus = gf::spi_bus_bb_sclk_miso_mosi<
   target::p1_2, gf::pin_in_dummy, target::p1_0, timing >;
using ss      = gf::invert< target::p1_1 >;
using chip    = gf::hc595< spi_bus, ss >;

int main( void ){   
   timing::init();    
   chip::init();
   gf::ostream< gf::formatter< uart > > cout;
   timing::ms< 2'000>::wait();
   
   auto t1 = timing::now_ticks();
   timing::us< 230 >::wait();
   auto t2 = timing::now_ticks();
   cout << "should be ~ 230 : " << ( t2 - t1 ) / 12 << " us\n";
   
   t1 = timing::now_ticks();
   chip::p0::write( 1 );
   t2 = timing::now_ticks();
   cout << "write pin buffered " << ( t2 - t1 ) / 12 << " us\n";

   t1 = timing::now_ticks();
   gf::direct< chip::p0 >::write( 1 );
   t2 = timing::now_ticks();
   cout << "write pin direct " << ( t2 - t1 ) / 12 << " us\n";

   t1 = timing::now_ticks();
   chip::p0::write( 1 );
   chip::p1::write( 0 );
   chip::p2::write( 1 );
   chip::p3::write( 1 );
   chip::p4::write( 1 );
   chip::p5::write( 0 );
   chip::p6::write( 0 ); 
   chip::p7::write( 1 );   
   chip::p0::flush();
   t2 = timing::now_ticks();
   cout << "write 8 pins buffered + flush (one) " << ( t2 - t1 ) / 12 << " us\n";
   
   t1 = timing::now_ticks();
   chip::p0::write( 1 );
   chip::p1::write( 0 );
   chip::p2::write( 1 );
   chip::p3::write( 1 );
   chip::p4::write( 1 );
   chip::p5::write( 0 );
   chip::p6::write( 0 ); 
   chip::p7::write( 1 );   
   chip::p0::flush();
   chip::p1::flush();
   chip::p2::flush();
   chip::p3::flush();
   chip::p4::flush();
   chip::p5::flush();
   chip::p6::flush();
   chip::p7::flush();
   t2 = timing::now_ticks();
   cout << "write 8 pins buffered + flush (each) " << ( t2 - t1 ) / 12 << " us\n";
   
   t1 = timing::now_ticks();
   chip::p0::write( 1 );
   chip::p0::flush();
   chip::p1::write( 0 );
   chip::p1::flush();
   chip::p2::write( 1 );
   chip::p2::flush();
   chip::p3::write( 1 );
   chip::p3::flush();
   chip::p4::write( 1 );
   chip::p4::flush();
   chip::p5::write( 0 );
   chip::p5::flush();
   chip::p6::write( 0 ); 
   chip::p6::flush();
   chip::p7::write( 1 );   
   chip::p7::flush();
   t2 = timing::now_ticks();
   cout << "write + flush 8 pins buffered " << ( t2 - t1 ) / 12 << " us\n";
   
}

