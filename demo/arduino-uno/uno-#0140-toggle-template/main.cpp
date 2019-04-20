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

class pin_b5 {
public:
   static void init(){
      DDRB |= ( 0b01 << 5 );      
   }
   
   static void set( bool v ){ 
      if( v ){
         PORTB &= ~( 0b01 << 5 );
      } else {
         PORTB |= ( 0b01 << 5 );
      }
   }   
};
  
template< typename P >  
void toggle(){
   P::init();
   for(;;){
      P::set( 1 );
      P::set( 0 );
   }
}

int main( void ){
   toggle< pin_b5 >();
}
