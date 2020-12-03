// =============================================================================
//
// gf-atmega.hpp
//
// =============================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss),
// a C++ library for close-to-the-hardware programming.
//
// Copyright
//    Wouter van Ooijen 2019-2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================
//
// HAL for atmega AVR chips
//
// =============================================================================

#include "gf-all.hpp"

// the atmel header files use 'register', which is removed in C++17
#define register 
#include "avr/io.h"
#undef register

// used by the inline small delay
extern "C" void GODAFOSS_WEAK GODAFOSS_NO_INLINE _GODAFOSS_avr_ret(){}

// required because the C++ library doesn't provide it
extern "C" void GODAFOSS_WEAK GODAFOSS_NO_INLINE __cxa_pure_virtual(){
   for(;;){
      // This if for the side-effect only:
      // a never-ending loop without side-effect is UB.
      (void)PORTB;
   }
}
    
namespace godafoss {
    
template< uint64_t clock >	
struct atmega {
	
// =============================================================================
//
// chip initialization
//
// =============================================================================

static void GODAFOSS_INLINE init__(){
   
   // no real initialization required, just check if
   // the clock parameter is reasonable
    
   if constexpr ( clock == 20'000'000 ){
      // 20 MHz crystal	   
	  
   } else if constexpr ( clock == 16'000'000 ){
      // 16 MHz crystal	   
	  
   } else if constexpr ( clock == 8'000'000 ){
      // 16 MHz crystal	   
	  
   } else if constexpr ( clock == 1'000'000 ){
      // 1 MHz crystal	   
	  
   } else {
      static_assert( 
         clock == 0, 
         "Only 1 MHz (internal oscillator), "
         "or 16 or 20 MHz (external crystal) "
         "clock is supported for atmega328.");
   }		 
  
}
   
   
// =============================================================================
//
// GPIO
//
// =============================================================================

// AVR GPIO ports
enum class port { b, c, d };	

// AVR GPIO register addresses
static constexpr struct {
   volatile std::uint8_t * output;
   volatile std::uint8_t * input; 
   volatile std::uint8_t * direction;
} port_register [] = {
   { &PORTB, &PINB, &DDRB },
   { &PORTC, &PINC, &DDRC },
   { &PORTD, &PIND, &DDRD }
};

template< port p, uint32_t pin >
struct gpio : 
   pin_in_out_root 
{
   
   using resources = execute< atmega< clock >::init__ >;   
   
   static void GODAFOSS_INLINE direction_set_output(){
      *port_register[ (int)p ].direction |= ( 0x1U << pin );
   }
   
   static void GODAFOSS_INLINE direction_set_input(){
      *port_register[ (int)p ].direction &= ~ ( 0x1U << pin );
   }
   
   static void GODAFOSS_INLINE write( bool v ){
      if( v ){
         *port_register[ (int)p ].output |= ( 0x1U << pin );
      } else {
         *port_register[ (int)p ].output &= ~ ( 0x1U << pin );
      }
   }

   static void GODAFOSS_INLINE pullup_enable(){
      write( 1 );
   }

   static void GODAFOSS_INLINE pullup_disable(){
      write( 0 );
   }

   static void GODAFOSS_INLINE flush(){}
      
   static bool GODAFOSS_INLINE read(){	   
      return ( *port_register[ (int)p ].input & ( 0x1U << pin ) ) != 0;
   }
      
   static void GODAFOSS_INLINE refresh(){}
   
   static void GODAFOSS_INLINE direction_flush(){}
      
};


// =============================================================================
//
// ADC
//
// =============================================================================

template< uint_fast64_t pin >
struct adc 
// :  be_adc< 10 >
{

   static void init__(){
      
      // reference is AVCC
      ADMUX = 0x01 << REFS0;
      
      // Enable the ADC and prescale
      ADCSRA = 7 | ( 0x01 << ADEN );  
   }
   
   using resources = execute< atmega< clock >::init__, init__ >;    

   static uint_fast16_t read(){
	   
      // select the ADC input pin 
      ADMUX = ( 0x01 << REFS0 ) | pin;

      // start the conversion
      ADCSRA |= 0x01 << ADSC;

      // wait for the conversion to finish
      while ( (ADCSRA & ( 0x01 << ADSC )) != 0 ){}

      return ADCW;
   }
   
   static void GODAFOSS_INLINE refresh(){}
   
};


// =============================================================================
//
// UART
//
// =============================================================================

template< uint64_t baudrate = GODAFOSS_BAUDRATE >
struct uart :
   be_uart< uart< baudrate > >
{
	
   static void init__(){   
	   
      // set baudrate	   
      uint64_t UBRR_VALUE = ((( clock / ( GODAFOSS_BAUDRATE * 16UL ))) - 1 );
      UBRR0H = (uint8_t) ( UBRR_VALUE >> 8 );
      UBRR0L = (uint8_t) UBRR_VALUE;
	  
      // format : 8 data bits, no parity, 1 stop bit
      UCSR0C = 0x06;
	  
      // enable rx and tx
      UCSR0B = (1<<RXEN0)|(1<<TXEN0);
   }	
   
   using resources = execute< atmega< clock >::init__, init__ >;      

   static bool GODAFOSS_INLINE read_blocks(){
      return !( UCSR0A & ( 0x01<<RXC0 ));
   }

   static char GODAFOSS_INLINE read_unchecked(){
      return UDR0; 
   }

   static bool GODAFOSS_INLINE write_blocks(){
      return !( UCSR0A & ( 0x01 << UDRE0 ));
   }

   static void GODAFOSS_INLINE write_unchecked( char c ){
      UDR0 = c;
   }   
   
};


// =============================================================================
//
// inline small delay
//
// =============================================================================

using ticks_type = uint64_t;

// inline delay of a few instruction cycles
template< ticks_type n >
static void GODAFOSS_INLINE inline_small_delay(){
       
   static_assert( n < 16 );
       
   if constexpr ( n > 7 ){
      __asm volatile(                  
         "call _GODAFOSS_avr_ret"     
      );        
   }

   // 8 cycles were handled by the call above,
   // so now handle the remainder (0..7 cycles)
   constexpr ticks_type t = ( n % 8 );   
       
   if constexpr ( t  == 0 ){
      // nothing
         
   } else if constexpr ( t == 1 ){
      __asm volatile(                  
         "   nop     \t\n"  
      );           
              
   } else if constexpr ( t == 2 ){
      __asm volatile(                  
         "   rjmp 1f     \t\n"  
         "1:             \t\n"   
      );           
         
   } else if constexpr ( t == 3 ){
      __asm volatile(                  
         "   rjmp 1f     \t\n"  
         "1: nop         \t\n"   
      );           
         
   } else if constexpr ( t == 4 ){
      __asm volatile(                  
         "   rjmp 1f     \t\n"  
         "1: rjmp 2f     \t\n"   
         "2:             \t\n"   
      );           
         
   } else if constexpr ( t == 5 ){
      __asm volatile(                  
         "   rjmp 1f     \t\n"  
         "1: rjmp 2f     \t\n"   
         "2: nop         \t\n"   
      );           
         
   } else if constexpr ( t == 6 ){
      __asm volatile(                  
         "   rjmp 1f     \t\n"  
         "1: rjmp 2f     \t\n"   
         "2: rjmp 3f     \t\n"   
         "3:             \t\n"   
      );           
         
   } else if constexpr ( t == 7 ){
      __asm volatile(                  
         "   rjmp 1f     \t\n"  
         "1: rjmp 2f     \t\n"   
         "2: rjmp 3f     \t\n"   
         "3: nop         \t\n"   
      );           
         
   }
}


// =============================================================================
//
// busy wait
//
// =============================================================================

// call overhead is LDI, LDI, CALL, CP, CPC, BRGE/t, RET => 15
static constexpr auto call_overhead = 15;
static constexpr auto chunk = 8192;
static constexpr auto one_loop = 7;

// delay function for n instrcution cycles
static void GODAFOSS_NO_INLINE busy_wait_ticks_asm( uint16_t n ){ 
    // the first int parameter is passed in r24/r25
    __asm volatile(                       // clocks
       "1:  cp    r1, r24         \t\n"   // 1
       "    cpc   r1, r25         \t\n"   // 1
       "    brge  2f              \t\n"   // 1
       "    sbiw  r24, 7          \t\n"   // 2 
       "    rjmp  1b              \t\n"   // 2
       "2:                        \t\n"   // 7 total
       : : "r" ( n )                      // uses (reads) n
   ); 
}


// =============================================================================
//
// using timer 1
//
// =============================================================================

static inline uint16_t    last_low = 0;
static inline ticks_type  high = 0;

static ticks_type now_ticks(){

   uint16_t low = TCNT1L;
   low |= ( TCNT1H << 8 );

   if( low < last_low ){
   
       // the timer rolled over, so increment the high part
      high += 0x1ULL << 16;
   }
   last_low = low;

   return ( low | high ); 
} 


// =============================================================================
//
// waiting timing service
//
// =============================================================================

struct waiting : 
   be_timing_wait< waiting > 
{
	
   using chip       = atmega< clock >;   
   using ticks_type = chip::ticks_type;
   using resources  = execute< chip::init__ >;
   
   static constexpr ticks_type ticks_from_ns( uint64_t n ){
      return ( n * clock ) / 1'000'000'000;	   
   }   

   static void GODAFOSS_NO_INLINE wait_ticks_function( ticks_type n ){     

      while( n > chunk ){
         chip::busy_wait_ticks_asm( chunk - call_overhead );
         n -= chunk;  
      }
      if( n > 0 ){
         chip::busy_wait_ticks_asm( n - call_overhead );
      }
	  
   }  
   
   template< ticks_type t >
   static void GODAFOSS_INLINE wait_ticks_template(){
	      
      if constexpr ( t < call_overhead ){    
          chip::inline_small_delay< t >();
          
      } else if constexpr ( t <= chunk ){  

         constexpr ticks_type remaining = t - call_overhead;
          
         chip::inline_small_delay< remaining % one_loop >();
         chip::busy_wait_ticks_asm( remaining - ( remaining % one_loop) );   
         
      } else {
         wait_ticks_function( t ); 
      }         
   };	  
};


// =============================================================================
//
// clocking timing service
//
// =============================================================================

struct clocking : 
   be_timing_wait< clocking > 
{
	
   using chip = atmega< clock >;      
	
   static void init__(){
      chip::init__();

      // set up timer 1 without prescaler (input=CPU clock)
      TCCR1B = 0x01;   
   }	
   
   using resources = execute< init__ >;   
   
   static constexpr ticks_type ticks_from_ns( uint64_t n ){
      return ( n * clock ) / 1'000'000'000;	   
   }   

   static ticks_type GODAFOSS_INLINE now_ticks(){
      return chip::now_ticks();
   }

   static void GODAFOSS_NO_INLINE wait_ticks_function( ticks_type n ){     
      ticks_type t = now_ticks() + n;
      while( now_ticks() < t ){}
   } 
   
   template< ticks_type t >
   static void GODAFOSS_INLINE wait_ticks_template(){
	   
      if constexpr ( t < call_overhead ){   

          // very small delay: generate a few inline instructions 
          
          chip::inline_small_delay< t >();
          
      } else if constexpr ( t <= 2'000 ){  

         // medium delay: call 
         
         constexpr ticks_type remaining = t - call_overhead;
          
         chip::busy_wait_ticks_asm( remaining - ( remaining % one_loop) );   
         chip::inline_small_delay< remaining % one_loop >();
         
      } else {
         
         // long delay: 
         
         wait_ticks_function( t ); 
      }         
   };	  
};

}; // struct atmega

}; // namespace hwcpp
