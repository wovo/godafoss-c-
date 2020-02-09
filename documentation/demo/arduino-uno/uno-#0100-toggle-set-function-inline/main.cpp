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

__attribute__((always_inline)) void pin_set( bool x ) { 
   if( x ){
      PORTB &= ~( 0b01 << 5 );
   } else {
      PORTB |= ( 0b01 << 5 );
   }
}
   
void toggle(){
   for(;;){
      pin_set( 1 );
      pin_set( 0 );
   }
}

int main( void ){
   DDRB |= ( 0b01 << 5 );
   toggle();
}
