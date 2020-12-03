// ==========================================================================
//
// blink the LED on an Arduino Uno
//
// (c) Wouter van Ooijen (wouter@voti.nl) 2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt) 
//
// ==========================================================================

#include "godafoss.hpp"

int main( void ){
   DDRC |= ( 0b01 << 7 );
   for(;;){
      PORTC &= ~( 0b01 << 7 );
      for( volatile uint32_t i = 0; i < 50'000; ++i );
      PORTC |= ( 0b01 << 7 );
      for( volatile uint32_t i = 0; i < 50'000; ++i );
   }      
}
