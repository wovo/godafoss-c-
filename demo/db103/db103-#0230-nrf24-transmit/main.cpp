// TRANSMITTER

#include "godafoss.hpp"

namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;
using led     = gf::direct< gf::pin_out< target::p1_2 >>;
using uart    = target::uart<>;
using adc     = target::adc1_0;

using spi = gf::spi_bus_bb_sclk_miso_mosi<
   target::p1_9, 
   target::p1_8, 
   target::p1_5,
   timing
>;
using nrf = gf::nrf24l01_spi_ce_csn<
   spi,
   target::p1_3,
   target::p1_4,
   timing
>; 

int main(){ 
   timing::init();
   led::init();   
   adc::init();   
   nrf::init();
   
   nrf::extensions_toggle();
   nrf::write( nrf::reg::feature, 0x07 );
   nrf::mode_transmit();

   for(;;){
       
      nrf::interrupts_clear();
      nrf::write( nrf::cmd::flush_tx );
      
      // transmit 'a' ... 'z'
      std::array< uint8_t, 32 > msg;
      auto x= adc::read();
      msg[ 0 ] = x >> 2;
      nrf::transmit_datagram( msg );
    
      timing::ms< 100 >::wait();
	   led::write( 1 );
      timing::ms< 100 >::wait();
	   led::write( 0 );
   }      
}

