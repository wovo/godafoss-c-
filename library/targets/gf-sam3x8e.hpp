// ==========================================================================
//
// file : target-sam3xa-chip.hpp
//
// HAL for atsam3x
//
// ==========================================================================

#include "gf-all.hpp"

// the atmel header files use 'register', which is removed in C++17
#define register 
#include "sam.h"
#undef register

// the implementation of CPU clock speed switching
#include "targets/sam3xa.inc"

namespace godafoss {
    
template< uint64_t clock >   
struct chip_sam3x8e {
	
	
// ==========================================================================
//
// chip initialization
//
// ==========================================================================	
    
static void init(){
	
   // don't do this over and over	
   GODAFOSS_RUN_ONCE;	
   
   // disable the watchdog     
   WDT->WDT_MR = WDT_MR_WDDIS;     
   
   // enable the clocks to all GPIO blocks
   PMC->PMC_PCER0 = ( 0x3F << 11 );
   
   // start the SysTick timer, counting at the CPU clock
   SysTick->CTRL  = 0;         // stop the timer
   SysTick->LOAD  = 0xFFFFFF;  // use its as a 24-bit timer
   SysTick->VAL   = 0;         // clear the timer
   SysTick->CTRL  = 5;         // start the timer, 1:1   
   
   if constexpr ( clock == 8'000'000 ){
	   
      // this is the default	   
	   
   } else if constexpr ( clock == 84'000'000 ){
	   
      // set flash timing	 
      EFC0->EEFC_FMR = EEFC_FMR_FWS( 5 );
      EFC1->EEFC_FMR = EEFC_FMR_FWS( 5 );     
	  
      // switch to 84 Mhz
      sam3xa::SystemInit();
     
   } else {	   
      static_assert( 
         clock == 84'000'000, 
         "Only 12 MHz (external 12 MHz crystal) "
         "or 84 MHz (external 12 MHz crystal, PLL) "
		   "are supported for this chip.");   
   }		 
}    


// ==========================================================================
//
// GPIO
//
// ==========================================================================
	
enum class _port {
   a = 0x400E0E00U,
   b = 0x400E1000U,
   c = 0x400E1200U,
   d = 0x400E1400U
};

template< _port P, uint32_t pin >
struct _pin_in_out : 
   be_pin_in_out< _pin_in_out< P, pin > >
{
	
   static void GODAFOSS_INLINE init(){
      godafoss::chip_sam3x8e< clock >::init();
   }
   
   static void GODAFOSS_INLINE direction_set_input(){
      ((Pio*)P)->PIO_ODR = ( 0x1U << pin );
   }
   
   static void GODAFOSS_INLINE direction_set_output(){
      ((Pio*)P)->PIO_OER = ( 0x1U << pin );
   }
   
   static void GODAFOSS_INLINE direction_flush(){}
   
   static void GODAFOSS_INLINE write( bool v ){
      ( v 
         ? ((Pio*)P)->PIO_SODR 
         : ((Pio*)P)->PIO_CODR 
      )  = ( 0x1U << pin );	   
   }

   static void GODAFOSS_INLINE flush(){}
   
   static bool GODAFOSS_INLINE read(){
      return ( ((Pio*)P)->PIO_PDSR & ( 0x1U << pin ) ) != 0;	   
   }

   static void GODAFOSS_INLINE refresh(){}

};


// ==========================================================================
//
// inline small delay
//
// ==========================================================================

static constexpr auto inline_delay_max = 6;

template< uint64_t t >
static void GODAFOSS_INLINE wait_inline(){
              
   if constexpr ( t  == 0 ){
      // nothing
         
   } else if constexpr ( t == 1 ){
      __asm volatile(                  
         "   nop     \t\n"  
      );           
              
   } else if constexpr ( t == 2 ){
      __asm volatile(                  
         "   nop     \t\n"  
         "   nop     \t\n"  
      ); 
		 
   } else if constexpr ( t == 3 ){
      __asm volatile(                  
         "   nop     \t\n"  
         "   nop     \t\n"  
         "   nop     \t\n"  
      ); 
		 
   } else if constexpr ( t == 4 ){
      __asm volatile(                  
         "   nop     \t\n"  
         "   nop     \t\n"  
         "   nop     \t\n"  
         "   nop     \t\n"  
      ); 
		 
   } else if constexpr ( t == 5 ){
      __asm volatile(                  
         "   nop     \t\n"  
         "   nop     \t\n"  
         "   nop     \t\n"  
         "   nop     \t\n"  
         "   nop     \t\n"  
      ); 
		 
   } else if constexpr ( t == 6 ){
      __asm volatile(                  
         "   nop     \t\n"  
         "   nop     \t\n"  
         "   nop     \t\n"  
         "   nop     \t\n"  
         "   nop     \t\n"  
         "   nop     \t\n"  
      ); 
		 
   }
}


// ==========================================================================
//
// busy wait
//
// ==========================================================================

static void GODAFOSS_NO_INLINE wait_busy( int32_t n ){
   __asm volatile(                  
      "   .align 4           \t\n"  
      "1: subs.w  r0, #3     \t\n"  
      "   bgt 1b             \t\n"  
      : : "r" ( n )          // uses (reads) n         
   ); 
}


// ==========================================================================
//
// timer
//
// ==========================================================================

static inline uint32_t  last_low = 0;
static inline uint64_t  high = 0;

static uint64_t now_ticks(){
   
   // the timer ticks down, but we want an up counter
   uint32_t low = 0xFFFFFF - ( SysTick->VAL & 0xFFFFFF );
   if( low < last_low ){
   
       // the timer rolled over, so increment the high part
      high += 0x1ULL << 24;
   }
   last_low = low;

   return ( low | high ); 
}


// ==========================================================================
//
// timing fundamentals
//
// ==========================================================================

struct waiting : be_timing_wait< waiting > {
   
   using ticks_type = uint64_t;
   
   static void GODAFOSS_INLINE init(){
      chip_sam3x8e< clock >::init();
   }	
   
   static constexpr ticks_type ticks_from_ns( uint64_t n ){
      return ( n * clock ) / 1'000'000'000;	   
   }
   
   static ticks_type GODAFOSS_INLINE now_ticks(){
      return chip_sam3x8e< clock >::now_ticks();
   }      
   
   static void GODAFOSS_NO_INLINE wait_ticks_function( ticks_type n ){     
      ticks_type t = now_ticks() + n;
      while( now_ticks() < t ){}
   }  
   
   template< ticks_type t >
   static void GODAFOSS_INLINE wait_ticks_template(){   
       
      if constexpr ( t <= inline_delay_max ){
         wait_inline< t >();    
      
      } else if constexpr ( t < 2'000 ){
          wait_busy( ((int32_t) t ) - 20 );
          
      } else {
          wait_ticks_function( t );
          
      }   
   }      
   
}; // struct waiting
  
}; // struct chip_sam3x8e

}; // namespace godafoss





