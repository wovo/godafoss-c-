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

template< typename T >
concept bool is_pin_out = requires(
   bool v
)  {  
   { T::init() } -> void;
   { T::set( v ) } -> void;
};

class pin_b5 {
public:
   static void init(){
      DDRB |= ( 0b01 << 5 );      
   }
   
   static void write( bool v ){ 
      if( v ){
         PORTB &= ~( 0b01 << 5 );
      } else {
         PORTB |= ( 0b01 << 5 );
      }
   }   
};
  
template< is_pin_out P >  
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
