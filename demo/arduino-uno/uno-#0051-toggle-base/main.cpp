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

void toggle(){
   for(;;){
      PORTB &= ~( 0b01 << 5 );
      PORTB |= ( 0b01 << 5 );
   }
}

int main( void ){
   DDRB |= ( 0b01 << 5 );
   toggle();
}
