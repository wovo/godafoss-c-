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

class pin {
public:
   virtual void set( bool v ) = 0;
};

class pin_b5 : public pin {
public:
   pin_b5(){
      DDRB |= ( 0b01 << 5 );      
   }
   
   void set( bool v ) override { 
      if( v ){
         PORTB &= ~( 0b01 << 5 );
      } else {
         PORTB |= ( 0b01 << 5 );
      }
   }   
};
  
void toggle( pin & p ){
   for(;;){
      p.set( 1 );
      p.set( 0 );
   }
}

int main( void ){
   pin_b5 p;
   toggle( p );
}
