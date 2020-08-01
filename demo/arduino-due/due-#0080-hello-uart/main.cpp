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

void print( const char * s ){
   while( *s != '\0' ){
      uart::write( *s++ );
   }
}

using resources = gf::use< timing, uart >;

int main( void ){
   resources::initialize();
   timing::ms< 2000 >::wait();
   print( "Hello world\n" );
}

