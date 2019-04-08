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

void pin_set( int p, bool v ) { 
   if( p < 10 ){   
   } else if( p < 20 ){   
      if( v ){
         PORTB &= ~( 0b01 << ( p % 10 ));
      } else {
         PORTB |= ( 0b01 << ( p % 10 ) );
      }
   } else if( p < 30 ){   
      if( v ){
         PORTC &= ~( 0b01 << ( p % 10 ) );
      } else {
         PORTC |= ( 0b01 << ( p % 10 ) );
      }
   }      
}
   
void toggle( int p ){
   for(;;){
      pin_set( p, 1 );
      pin_set( p, 0 );
   }
}

int main( void ){
   DDRB |= ( 0b01 << 5 );
   toggle( 15 );
}
