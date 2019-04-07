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

#define PORT PORTB
#define DDR  DDRB
#define PIN  5
  
// in some library file   
void toggle(){
   for(;;){
      PORT &= ~( 0b01 << PIN );
      PORT |= ( 0b01 << PIN );
   }
}

int main( void ){
   DDR |= ( 0b01 << PIN );
   toggle();
}
