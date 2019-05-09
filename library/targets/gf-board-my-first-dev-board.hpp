// ==========================================================================
//
// file : gf-board-my-first-devboard.hpp
//
// HAL for the 'my first devboard' target
//
// ==========================================================================

#include "targets/gf-chip-atmega328.hpp"

namespace godafoss {
    
template< uint64_t clock >
struct target_my_first_devboard :
   chip_atmega328< clock >    
{       
	
   using chip = chip_atmega328< clock >;	

   #define make_pin_out( NAME, PORT, PIN )                     \
      using NAME = pin_out< typename chip::                    \
	     template _pin_in_out< chip::_port::PORT, PIN > >;      \
   
   #define make_pin_in( NAME, PORT, PIN )                      \
      using NAME = pin_in< typename chip::                     \
	     template _pin_in_out< chip::_port::PORT, PIN > >;      \
   
   #define make_pin_adc( NAME, PIN )                           \
      using NAME = typename chip:: template _pin_adc< PIN >;   \
      
   make_pin_adc(     pot,  0 );
   
   make_pin_in(    sw1,  c,  1 );
   make_pin_in(    sw2,  c,  2 );
   make_pin_in(    sw3,  c,  3 );
   make_pin_in(    sw4,  c,  4 );
    
   make_pin_out(  buzzer,  d,  3 );
   
   make_pin_out(     red,  b,  3 );
   make_pin_out(   green,  b,  4 );
   make_pin_out(    blue,  b,  5 ); 

   make_pin_out(    led1,  d,  2 );
   make_pin_out(    led2,  d,  7 );
   make_pin_out(    led3,  d,  4 );
   make_pin_out(    led4,  d,  6 );
   make_pin_out(    led5,  d,  5 );
   make_pin_out(    led6,  b,  1 );
   make_pin_out(    led7,  d,  6 );
   make_pin_out(    led8,  b,  2 );
   make_pin_out(    led9,  c,  5 ); 
   
   #undef make_pin_in_out   
   #undef make_pin_adc 
   
   using colors = port_out< red, green, blue >;
   using colors_together = pin_out_from_pins< red, green, blue >;
   
   using leds = port_out< 
      led1, led2, led3, led4, led5, led6, led7, led8, led9 >;

   using leds_border = port_out< 
      led1, led2, led3, led4, led5, led6, led7, led8, led9 >;

   using leds_together = pin_out_from_pins< 
      led1, led2, led3, led4, led5, led6, led7, led8, led9 >;

   
}; // template<...> struct target_my_first_devboard

template< uint64_t clock = 16'000'000 >
using target = target_my_first_devboard< clock >; 
	
}; // namespace godafoss





