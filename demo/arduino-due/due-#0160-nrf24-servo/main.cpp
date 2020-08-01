// RECIEVER

#include "godafoss.hpp"

namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;
using uart    = target::uart<>;
using servo   = gf::servo< target::d2, timing >;

using spi = gf::spi_bus_bb_sclk_miso_mosi<
   target::d12,
   target::d13,
   target::d9,
   timing
>;
using nrf = gf::nrf24l01_spi_ce_csn<
   spi,
   target::d11,
   target::d8,
   timing
>;

int main(){
   gf::use< timing >::initialize();
   gf::use< servo >::initialize();

   timing::ms< 1000>::wait();
   gf::ostream< gf::formatter< uart > > cout;
   cout << "NRF24L01 test - receive\n";

   gf::use< nrf >::initialize();
   nrf::extensions_toggle();
   nrf::write( nrf::reg::feature, 0x07 );
   nrf::write( nrf::reg::rx_pw_p0, 32 );

   nrf::mode_receive();
   int v = 0;
   for(;;){
      std::array< uint8_t, 32 > msg;
      uint_fast8_t p;
      int_fast8_t n;
      if( nrf::receive( p, msg, n )){
         v = (int) msg[ 0 ];
         if(0) cout << "adc="<< v <<"\n";
         nrf::write( nrf::cmd::flush_rx );
         nrf::interrupts_clear();
      }

      servo::write( ( v * 100 ) / 256 );
   }
}
