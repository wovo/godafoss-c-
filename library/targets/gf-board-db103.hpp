// ==========================================================================
//
// file : gf-board-db103.hpp
//
// HAL for the arduino due board target
//
// ==========================================================================

#include "targets/gf-chip-lpc1114.hpp"

namespace godafoss {
   
template< uint64_t clock >
struct target_lpc1114 :
   chip_lpc1114< clock >    
{       
    
using chip = chip_lpc1114< clock >;

#define make_pin_in_out( NAME, PORT, PIN)      \
   using NAME  = typename chip:: template      \
      _pin_in_out< PORT, PIN >;                \
   
#define make_pin_adc( NAME, CHANNEL ) \
   using NAME  = typename chip:: template pin_adc< CHANNEL >;   
   
   make_pin_in_out(   scl,  0,  4 );
   make_pin_in_out(   sda,  0,  5 );
   make_pin_in_out(  p0_6,  0,  6 );
   make_pin_in_out(  p0_7,  0,  7 );
   
   make_pin_in_out(  p1_0,  1,  0 );
   make_pin_in_out(  p1_1,  1,  1 );
   make_pin_in_out(  p1_2,  1,  2 );
   make_pin_in_out(  p1_3,  1,  3 );
   
   make_pin_in_out(  p1_4,  1,  4 );
   make_pin_in_out(  p1_5,  1,  5 );
   make_pin_in_out(  p1_8,  1,  8 );
   make_pin_in_out(  p1_9,  1,  9 );
   
   make_pin_adc( adc1_0, 0x0100 );
   make_pin_adc( adc1_1, 0x0101 );
   make_pin_adc( adc1_2, 0x0102 );
   make_pin_adc( adc1_3, 0x0103 );
   make_pin_adc( adc1_4, 0x0104 );   
   
   using waiting  = typename chip::waiting;	  
   // using clocking = typename chip::clocking;	  
   using timing = waiting;  

}; // template<...> 

template< uint64_t clock = 12'000'000 >
using target = target_lpc1114< clock >; 
	
}; // namespace godafoss