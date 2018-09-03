// ==========================================================================
//
// file : target-sam3xa-chip.hpp
//
// HAL for atsam3x
//
// ==========================================================================

#include "hwcpp-all.hpp"

// the atmel header files use 'register', which is removed in C++17
#define register 
#include "sam.h"
#undef register

// the implementation of CPU clock speed switching
#include "targets/sam3xa.inc"

namespace godafoss {
    
struct sam3x8e {
	
	
// ==========================================================================
//
// PUBLIC
//
// chip initialization
//
// ==========================================================================	
    
static void init(){
	
   // don't do this over and over	
   _HWCPP_RUN_ONCE;	
   
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
		 "are supported for sam3xa.");   
   }		 
}    


// ==========================================================================
//
// LIBRARY-INTERNAL
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
struct _pin_in_out_foundation : 
   _pin_in_out_root 
{
	
   static void HWCPP_INLINE init(){
      hwcpp::chip_sam3x8e< clock >::init();
   }
   
   static void HWCPP_INLINE direction_set_direct( pin_direction d ){
      ( ( d == pin_direction::input )
         ? ((Pio*)P)->PIO_ODR 
         : ((Pio*)P)->PIO_OER 
      )  = ( 0x1U << pin );
   }
   
   static void HWCPP_INLINE set_direct( bool v ){
      ( v 
         ? ((Pio*)P)->PIO_SODR 
         : ((Pio*)P)->PIO_CODR 
      )  = ( 0x1U << pin );	   
   }

   static bool HWCPP_INLINE get_direct(){
      return ( ((Pio*)P)->PIO_PDSR & ( 0x1U << pin ) ) != 0;	   
   }
};

// ========= GPIO constructor used in the actual targets

template< _port P, uint32_t pin >
using _pin_in_out = _box_builder< _pin_in_out_foundation< P, pin > >;




// ========= inline small delay

static constexpr auto inline_delay_max = 6;
template< ticks_type t >
static void HWCPP_INLINE inline_delay(){
              
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

// ========= busy loop wait

static void HWCPP_NO_INLINE busy_delay( int32_t n ){
   __asm volatile(                  
      "   .align 4           \t\n"  
      "1: subs.w  r0, #3     \t\n"  
      "   bgt 1b             \t\n"  
      : : "r" ( n )          // uses (reads) n         
   ); 
}

// ========= foundation

struct _timing_foundation :
   _timing_clocking_foundation< std::ratio< clock, 1 > >
{
   static void init(){
      chip_sam3x8e< clock >::init();
   }	
   
   static ticks_type now_ticks(){
      return chip_sam3x8e< clock >::now_ticks();
   }      
   
   static void HWCPP_NO_INLINE  wait_ticks_function( ticks_type n ){     
      ticks_type t = now_ticks() + n;
      while( now_ticks() < t ){}
   }  
   
   template< ticks_type t >
   static void HWCPP_INLINE wait_ticks_template(){   
       
      if constexpr ( t <= inline_delay_max ){
         inline_delay< t >();    
      
      } else if constexpr ( t < 2'000 ){
          busy_delay( ((int32_t) t ) - 20 );
          
      } else {
          wait_ticks_function( t );
          
      }   
   }      
};

// ========= services

using waiting  = _timing_waiting_builder< _timing_foundation >;
using clocking = _timing_clocking_builder< _timing_foundation >;

}; // struct chip_sam3x8e

}; // namespace godafoss





