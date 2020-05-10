// ==========================================================================
//
// file : gf-chip-stm32f040f4.hpp
//
// HAL for the stm32f040f4 chip
//
// ==========================================================================

#include "gf-all.hpp"

// the STM header files use 'register' in the pre-C++17 sense
#define register
// stm32f103x6 also implies stm32f103x4
#include "stm32f103x6.h"
#undef register

namespace godafoss {
       
template< uint64_t clock >	
struct chip_stm32f103x4 {
	
// ==========================================================================
//
// chip initialization
//
// ==========================================================================	

static void GODAFOSS_INLINE init(){
    
   if constexpr ( clock == 8'000'000 ){
      // 8 MHz internal oscillator  
	  
   } else if constexpr ( clock == 72'000'000 ){
      // 72 MHz PLL internal oscillator

   } else {
      static_assert( 
         clock == 0, 
         "Only 8 MHz (internal oscillator), "
		   "or 72 MHz (external crystal) "
		   "clock is supported.");
   }		 
  
}
   
   
// ==========================================================================
//
// GPIO
//
// ==========================================================================

/*
      case 0  : return *GPIOA;
      case 1  : return *GPIOB;
      case 2  : return *GPIOC;
      case 3  : return *GPIOD;
*/


template< GPIO_TypeDef * p, uint32_t pin >
struct _pin_in_out : 
   be_pin_in_out 
{
   
   static void GODAFOSS_INLINE config( uint32_t conf ){
      volatile auto & config_word = ( pin < 8 ) ? p->CRL : p->CRH;
      const uint32_t config_offset = 4 * ( pin % 8 );
      config_word &= ~( 0xF << config_offset );
      config_word |= conf << config_offset;     
   }
	
   static void GODAFOSS_INLINE init(){
      godafoss::chip_stm32f103x4< clock >::init();
      
      if( p == GPIOA ){ RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; }
      if( p == GPIOB ){ RCC->APB2ENR |= RCC_APB2ENR_IOPBEN; }
      if( p == GPIOC ){ RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; }
      if( p == GPIOD ){ RCC->APB2ENR |= RCC_APB2ENR_IOPDEN; }
   }
   
   static void GODAFOSS_INLINE direction_set_output(){
      config( 0x03 );
   }
   
   static void GODAFOSS_INLINE direction_set_input(){
      config( 0x08 );
   }
   
   static void GODAFOSS_INLINE write( bool v ){
      p->BSRR |= ( 1 << ( v ? pin : pin + 16 ));
   }

   static void GODAFOSS_INLINE flush(){}
      
   static bool GODAFOSS_INLINE read(){	   
      return (( p->IDR & ( 1 << pin ) ) != 0 );   
   }
      
   static void GODAFOSS_INLINE refresh(){}
   
   static void GODAFOSS_INLINE direction_flush(){}
      
};




// ==========================================================================
//
// ADC
//
// ==========================================================================

template< uint_fast64_t pin >
struct _pin_adc :
   be_adc< 10 >
{
	
   static void init(){
      godafoss::chip_stm32f103x4< clock >::init();
      
      // reference is AVCC
      ADMUX = 0x01 << REFS0;
	  
      // Enable the ADC and prescale
      ADCSRA = 7 | ( 0x01 << ADEN );  
   }

   static uint_fast16_t read(){
	   
      // select the ADC input pin 
      ADMUX = ( 0x01 << REFS0 ) | pin;

      // start the conversion.
      ADCSRA |= 0x01 << ADSC;

      // wait for the conversion to finish
      while ( (ADCSRA & ( 0x01 << ADSC )) != 0 ){}

      return ADCW;
   }
   
   static void GODAFOSS_INLINE refresh(){}
   
};


// ==========================================================================
//
// UART
//
// ==========================================================================

template< uint64_t baudrate = GODAFOSS_BAUDRATE >
struct uart :
   be_uart< uart< baudrate > >
{
	
   static void init(){   
	   
      // set baudrate	   
      uint64_t UBRR_VALUE = ((( clock / ( GODAFOSS_BAUDRATE * 16UL ))) - 1 );
      UBRR0H = (uint8_t) ( UBRR_VALUE >> 8 );
      UBRR0L = (uint8_t) UBRR_VALUE;
	  
	  // format : 8 data bits, no parity, 1 stop bit
      UCSR0C = 0x06;
	  
	  // enable rx and tx
	  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
   }	

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


// ==========================================================================
//
// inline small delay
//
// ==========================================================================

using ticks_type = uint64_t;

template< ticks_type n >
static void GODAFOSS_INLINE inline_small_delay(){
       
   static_assert( n < 16 );
       
   if constexpr ( n > 7 ){
      __asm volatile(                  
         "call _GODAFOSS_avr_ret"     
      );        
   }

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


// ==========================================================================
//
// busy wait
//
// ==========================================================================

// call overhead is LDI, LDI, CALL, CP, CPC, BRGE/t, RET => 15
static constexpr auto call_overhead = 15;
static constexpr auto chunk = 8192;
static constexpr auto one_loop = 7;

static void GODAFOSS_NO_INLINE busy_wait_ticks_asm( int n ){ 
    // first int parameter is passd in r24/r25
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


// ==========================================================================
//
// using timer 1
//
// ==========================================================================

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


// ==========================================================================
//
// wait
//
// ==========================================================================

struct waiting : 
   be_timing_wait< waiting > 
{
	
   using chip = chip_atmega328< clock >;   
   using ticks_type = chip::ticks_type;
	
   static void init(){
      chip::init();
   }	
   
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
	   
	  //wait_ticks_function( t ); 
	  //return;
	      
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


// ==========================================================================
//
// clock
//
// ==========================================================================

struct clocking : 
   be_timing_wait< clocking > 
{
	
   using chip = chip_stm32f103x4< clock >;      
	
   static void init(){
      chip::init();

      // set up timer 1 without prescaler (input=CPU clock)
      TCCR1B = 0x01;   
   }	
   
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
          chip::inline_small_delay< t >();
          
      } else if constexpr ( t <= 2'000 ){  

         constexpr ticks_type remaining = t - call_overhead;
          
         chip::inline_small_delay< remaining % one_loop >();
         chip::busy_wait_ticks_asm( remaining - ( remaining % one_loop) );   
         
      } else {
         wait_ticks_function( t ); 
      }         
   };	  
};


// ==========================================================================
//
// default
//
// ==========================================================================
   
using timing   = waiting;  
// using timing   = clocking;  

}; // struct chip_stm32f103x4

}; // namespace godafoss
