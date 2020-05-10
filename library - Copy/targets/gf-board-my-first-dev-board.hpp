// ==========================================================================
//
// file : gf-board-my-first-devboard.hpp
//
// HAL for the 'my first devboard' target
//
// ==========================================================================

#include "targets/gf-chip-atmega328.hpp"

namespace godafoss {
   
template< typename T >   
struct pin_in_from_adc : be_pin_in {
   static void init(){
      T::init();
   }
   static void refresh(){
      T::refresh();
   }
   static bool read(){
      return T::read() > ( T::adc_max / 2 );
   }
};
    
template< uint64_t clock >
struct target_my_first_devboard :
   chip_atmega328< clock >    
{       
	
   using chip = chip_atmega328< clock >;	

   #define make_pin_out( NAME, PORT, PIN )                      \
      using NAME = pin_out< typename chip::                     \
	     template _pin_in_out< chip::_port::PORT, PIN > >;      \
   
   #define make_pin_in( NAME, PORT, PIN )                       \
      using NAME = pin_in< typename chip::                      \
	     template _pin_in_out< chip::_port::PORT, PIN > >;      \
		 
   #define make_pin_in_out( NAME, PORT, PIN )                   \
      using NAME = typename chip::                              \
	     template _pin_in_out< chip::_port::PORT, PIN >;        \
   
   #define make_pin_adc( NAME, PIN )                            \
      using NAME = typename chip:: template _pin_adc< PIN >;    \
	  
   // my-first0-devboard pin functions	  
      
   make_pin_adc(      adc,  0 );
   make_pin_adc( adc6_pin,  6 );
   make_pin_adc( adc7_pin,  7 );
   
   make_pin_in(  sw1_pin,  c,  1 );
   make_pin_in(  sw2_pin,  c,  2 );
   make_pin_in(  sw3_pin,  c,  3 );
   make_pin_in(  sw4_pin,  c,  4 );

//   make_pin_in(      sw5,  c,  6 );
//   make_pin_in(      sw6,  c,  7 );
   
   using sw1 = invert< sw1_pin >;
   using sw2 = invert< sw2_pin >;
   using sw3 = invert< sw3_pin >;
   using sw4 = invert< sw4_pin >;
    
   //make_pin_in(  sw6,      c,  5 );
   using sw6 = pin_in_from_adc< adc7_pin >;
   using sw5 = pin_in_from_adc< adc6_pin >;

   make_pin_out(  buzzer,  d,  3 );
   
   make_pin_out(     red,  b,  3 );
   make_pin_out(   green,  b,  5 );
   make_pin_out(    blue,  b,  4 ); 

   make_pin_out(    led1,  d,  2 );
   make_pin_out(    led2,  d,  7 );
   make_pin_out(    led3,  d,  4 );
   make_pin_out(    led4,  b,  0 );
   make_pin_out(    led5,  d,  5 );
   make_pin_out(    led6,  b,  1 );
   make_pin_out(    led7,  d,  6 );
   make_pin_out(    led8,  b,  2 );
   make_pin_out(    led9,  c,  5 ); 
   
   // arduino pins
   
   make_pin_in_out(    d0,  d,  0 );
   make_pin_in_out(    d1,  d,  1 );
   make_pin_in_out(    d2,  d,  2 );
   make_pin_in_out(    d3,  d,  3 );
   make_pin_in_out(    d4,  d,  4 );
   make_pin_in_out(    d5,  d,  5 );
   make_pin_in_out(    d6,  d,  6 );
   make_pin_in_out(    d7,  d,  7 );
   make_pin_in_out(    d8,  b,  0 );
   make_pin_in_out(    d9,  b,  1 );
   make_pin_in_out(   d10,  b,  2 );
   make_pin_in_out(   d11,  b,  3 );
   make_pin_in_out(   d12,  b,  4 );
   make_pin_in_out(   d13,  b,  5 );
   
   make_pin_in_out(   d14,  c,  0 );
   make_pin_in_out(   d15,  c,  1 );
   make_pin_in_out(   d16,  c,  2 );
   make_pin_in_out(   d17,  c,  3 );
   make_pin_in_out(   d18,  c,  4 );
   make_pin_in_out(   d19,  c,  5 );
   
   make_pin_in_out(    a0,  c,  0 );
   make_pin_in_out(    a1,  c,  1 );
   make_pin_in_out(    a2,  c,  2 );
   make_pin_in_out(    a3,  c,  3 );
   make_pin_in_out(    a4,  c,  4 );
   make_pin_in_out(    a5,  c,  5 ); 
   
   make_pin_adc(    adc0,  0 );
   make_pin_adc(    adc1,  1 );
   make_pin_adc(    adc2,  2 );
   make_pin_adc(    adc3,  3 );
   make_pin_adc(    adc4,  4 );
   make_pin_adc(    adc5,  5 );   
   
   #undef make_pin_in   
   #undef make_pin_out   
   #undef make_pin_in_out   
   #undef make_pin_adc 
   
   using colors = port_out< red, green, blue >;
   using colors_together = pin_out_from_pins< red, green, blue >;
   
   using leds = port_out< 
      led1, led2, led3, led4, led5, led6, led7, led8, led9 >;
   using leds_together = pin_out_from_pins< 
      led1, led2, led3, led4, led5, led6, led7, led8, led9 >;

   using leds_border = port_out< 
      led1, led2, led3, led6, led9, led8, led7, led4 >;
   using leds_border_together = pin_out_from_pins< 
      led1, led2, led3, led6, led9, led8, led7, led4 >;
   
}; // template<...> struct target_my_first_devboard

template< uint64_t clock = 16'000'000 >
using target = target_my_first_devboard< clock >; 
	
}; // namespace godafoss





