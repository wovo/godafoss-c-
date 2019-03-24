// ==========================================================================
//
// file : target-sam3xa-chip.hpp
//
// HAL for atsam3x
//
// ==========================================================================

#include "gf-all.hpp"

// the LPC header file use 'register' in the pre-C++17 sense
#define register
#include "LPC11xx.h"
#undef register

namespace godafoss {
   
template< uint64_t clock >   
struct chip_lpc1114 {   

   
// ==========================================================================
//
// chip initialization
//
// ==========================================================================	
    
static void init(){
	
   // don't do this over and over	
   GODAFOSS_RUN_ONCE;	
             
   SysTick->CTRL  = 0;         // stop the timer
   SysTick->LOAD  = 0xFFFFFF;  // use its as a 24-bit timer
   SysTick->VAL   = 0;         // clear the timer
   SysTick->CTRL  = 5;         // start the timer, 1:1
   
   if constexpr ( clock == 12'000'000 ){
	   
      // this is the default	   
     
   } else {	   
      static_assert( 
         clock == 12'000'000, 
         "Only 12 MHz (internal 12 MHz oscillator) "
		   "is supported for this chip.");   
   }		 
   
   
}


// ==========================================================================
//
// pin access primitives
//
// ==========================================================================	
    
// return the pointer to an IO port related register
static GODAFOSS_INLINE volatile uint32_t * gpioreg(
   uint32_t port, 
   uint32_t offset 
){
   return (volatile uint32_t *)( 0x50000000 + port * 0x10000 + offset );
}
   
// configure a pin that has no AD capability
static GODAFOSS_INLINE void pin_gp_conf( 
   volatile uint32_t *p, 
   uint32_t value 
){
   *p = value | ( *p & ~0x07 );
}   

// configure a pin with AD capability  
static GODAFOSS_INLINE void pin_ad_conf( 
   volatile uint32_t *p, 
   uint32_t value 
){
   *p = 
      ( value << 0 ) 
      | ( *p & ~ ( ( 0x07 << 0 ) | ( 0x01 << 7 )));
}   
   
// configure a pin as GPIO
static GODAFOSS_INLINE void configure_as_gpio( 
   uint32_t port, 
   uint32_t pin 
){
   LPC_SYSCON->SYSAHBCLKCTRL |= (1<<16);     // enable IOCON
   switch( ( port << 8 ) | pin ){
      case 0x0000: pin_gp_conf( &LPC_IOCON->RESET_PIO0_0,  0x01 ); break;
      case 0x0001: pin_gp_conf( &LPC_IOCON->PIO0_1,        0x00 ); break;
      case 0x0002: pin_gp_conf( &LPC_IOCON->PIO0_2,        0x00 ); break;
      case 0x0003: pin_gp_conf( &LPC_IOCON->PIO0_3,        0x00 ); break;
      case 0x0004: pin_gp_conf( &LPC_IOCON->PIO0_4,        0x00 ); break;
      case 0x0005: pin_gp_conf( &LPC_IOCON->PIO0_5,        0x00 ); break;
      case 0x0006: pin_gp_conf( &LPC_IOCON->PIO0_6,        0x00 ); break;
      case 0x0007: pin_gp_conf( &LPC_IOCON->PIO0_7,        0x00 ); break;
      case 0x0008: pin_gp_conf( &LPC_IOCON->PIO0_8,        0x00 ); break;
      case 0x0009: pin_gp_conf( &LPC_IOCON->PIO0_9,        0x00 ); break;
      case 0x000A: pin_gp_conf( &LPC_IOCON->SWCLK_PIO0_10, 0x01 ); break;
      case 0x000B: pin_ad_conf( &LPC_IOCON->R_PIO0_11,     0x01 ); break;
         
      case 0x0100: pin_ad_conf( &LPC_IOCON->R_PIO1_0,      0x01 ); break;
      case 0x0101: pin_ad_conf( &LPC_IOCON->R_PIO1_1,      0x01 ); break;
      case 0x0102: pin_ad_conf( &LPC_IOCON->R_PIO1_2,      0x01 ); break;
      case 0x0103: pin_ad_conf( &LPC_IOCON->SWDIO_PIO1_3,  0x01 ); break;
      case 0x0104: pin_ad_conf( &LPC_IOCON->PIO1_4,        0x00 ); break;
      case 0x0105: pin_gp_conf( &LPC_IOCON->PIO1_5,        0x00 ); break;
      case 0x0106: pin_gp_conf( &LPC_IOCON->PIO1_6,        0x00 ); break;
      case 0x0107: pin_gp_conf( &LPC_IOCON->PIO1_7,        0x00 ); break;
      case 0x0108: pin_gp_conf( &LPC_IOCON->PIO1_8,        0x00 ); break;
      case 0x0109: pin_gp_conf( &LPC_IOCON->PIO1_9,        0x00 ); break;
      case 0x010A: pin_gp_conf( &LPC_IOCON->PIO1_10,       0x00 ); break; 
      case 0x010B: pin_gp_conf( &LPC_IOCON->PIO1_11,       0x00 ); break; 
         
      case 0x0200: pin_gp_conf( &LPC_IOCON->PIO2_0,        0x00 ); break;
      case 0x0201: pin_gp_conf( &LPC_IOCON->PIO2_1,        0x00 ); break;
      case 0x0203: pin_gp_conf( &LPC_IOCON->PIO2_3,        0x00 ); break;
      case 0x0202: pin_gp_conf( &LPC_IOCON->PIO2_2,        0x00 ); break;
      case 0x0204: pin_gp_conf( &LPC_IOCON->PIO2_4,        0x00 ); break;
      case 0x0205: pin_gp_conf( &LPC_IOCON->PIO2_5,        0x00 ); break;
      case 0x0206: pin_gp_conf( &LPC_IOCON->PIO2_6,        0x00 ); break;
      case 0x0207: pin_gp_conf( &LPC_IOCON->PIO2_7,        0x00 ); break;
      case 0x0208: pin_gp_conf( &LPC_IOCON->PIO2_8,        0x00 ); break;
      case 0x0209: pin_gp_conf( &LPC_IOCON->PIO2_9,        0x00 ); break;
      case 0x020A: pin_gp_conf( &LPC_IOCON->PIO2_10,       0x00 ); break;
      case 0x020B: pin_gp_conf( &LPC_IOCON->PIO2_11,       0x00 ); break;
         
      case 0x0300: pin_gp_conf( &LPC_IOCON->PIO3_0,        0x00 ); break;
      case 0x0301: pin_gp_conf( &LPC_IOCON->PIO3_1,        0x00 ); break;
      case 0x0302: pin_gp_conf( &LPC_IOCON->PIO3_2,        0x00 ); break;
      case 0x0303: pin_gp_conf( &LPC_IOCON->PIO3_3,        0x00 ); break;
      case 0x0304: pin_gp_conf( &LPC_IOCON->PIO3_4,        0x00 ); break;
      case 0x0305: pin_gp_conf( &LPC_IOCON->PIO3_5,        0x00 ); break;
         
      default: break; // HWLIB_PANIC_WITH_LOCATION; 
   }        
} 
   
// ==========================================================================
//
// pin in_out
//
// ==========================================================================	
    
template< uint32_t port, uint32_t pin >
struct _pin_in_out : 
   be_pin_in_out
{
	   
   static const uint32_t mask = 0x1U << pin;
   
public:
  
   static GODAFOSS_INLINE void init(){
      configure_as_gpio( port, pin );
   }
   
   static GODAFOSS_INLINE void direction_set_input(){
      *gpioreg( port, 0x8000 ) &= ~ mask;
   }
   
   static GODAFOSS_INLINE bool read(){
      return *gpioreg( port, 0x3FFC ) & mask;
   }
   
   static GODAFOSS_INLINE void  direction_set_output(){
     *gpioreg( port, 0x8000 ) |= mask ;    
   }
   
   static GODAFOSS_INLINE void write( bool v ) {
      *gpioreg( port, 0x04 << pin ) = v ? -1 : 0;
   }
   
   static GODAFOSS_INLINE void  refresh() {
   }
   
   static GODAFOSS_INLINE void  flush() {
   }   
   
   static GODAFOSS_INLINE void  direction_flush() {
   } 
   
};   
	
	
// ==========================================================================
//
// pin adc
//
// ==========================================================================	
    
// configure a pin as ADC
static uint32_t configure_as_adc( 
   uint32_t port, 
   uint32_t pin 
){ 
 
   // enable IOCON & A/D clock
   LPC_SYSCON->SYSAHBCLKCTRL |= ( 0x01 << 16 ) | ( 0x01 << 13 );
   
      // enable A/D power
   LPC_SYSCON->PDRUNCFG &= ~( 1 << 4 );      

   switch( ( port << 8 ) | pin ){
      case 0x000B: pin_ad_conf( &LPC_IOCON->R_PIO0_11,    0x02 ); return 0;
      
      case 0x0100: pin_ad_conf( &LPC_IOCON->R_PIO1_0,     0x02 ); return 1;
      case 0x0101: pin_ad_conf( &LPC_IOCON->R_PIO1_1,     0x02 ); return 2;
      case 0x0102: pin_ad_conf( &LPC_IOCON->R_PIO1_2,     0x02 ); return 3;
      case 0x0103: pin_ad_conf( &LPC_IOCON->SWDIO_PIO1_3, 0x02 ); return 4;
      case 0x0104: pin_ad_conf( &LPC_IOCON->PIO1_4,       0x01 ); return 5;
        
      //default: HWLIB_PANIC_WITH_LOCATION;
   }          
}    


// ==========================================================================
//
// busy wait
//
// ==========================================================================

static GODAFOSS_INLINE void wait_busy( int_fast32_t n ){
   __asm volatile( 
      "   mov r0, %[reg]   \t\n"
      "   b   1f           \t\n"
      "   .align 4         \t\n"
      "1: sub r0, #4       \t\n" 
      "   bgt 1b           \t\n" 
      :: [reg] "r" (n) : "r0"
   );
} 


// ==========================================================================
//
// timer
//
// ==========================================================================

static GODAFOSS_INLINE uint_fast64_t now_ticks(){

   
   static unsigned int last_low = 0;
   static unsigned long long int high = 0;

   // the timer ticks down, but we want an up counter
   unsigned int low = 0xFFFFFF - ( SysTick->VAL & 0xFFFFFF );
   if( low < last_low ){
   
      // the timer rolled over, so increment the high part
      high += 0x1ULL << 24;
   }
   last_low = low;

   // return the aggregated ticks value
   // the counter runs at 84 MHz 
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
      chip_lpc1114< clock >::init();
   }	
   
   static constexpr ticks_type ticks_from_ns( uint64_t n ){
      return ( n * clock ) / 1'000'000'000;	   
   }
   
   static ticks_type GODAFOSS_INLINE now_ticks(){
      return chip_lpc1114< clock >::now_ticks();
   }      
   
   static void GODAFOSS_NO_INLINE wait_ticks_function( ticks_type n ){     
      ticks_type t = now_ticks() + n;
      while( now_ticks() < t ){}
   }  
   
   template< ticks_type t >
   static void GODAFOSS_INLINE wait_ticks_template(){   
       
      if constexpr ( t < 200 ){
          wait_busy( ((int32_t) t ) - 20 );
          
      } else {
          wait_ticks_function( t );
          
      }   
   }      
   
}; // struct waiting

}; // struct lpc11114

}; // namespace godafoss

