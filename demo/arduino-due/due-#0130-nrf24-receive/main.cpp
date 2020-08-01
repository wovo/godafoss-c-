// RECIEVER

#include "godafoss.hpp"

namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;
using uart    = target::uart<>;

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
   timing::ms< 1000>::wait();
   gf::ostream< gf::formatter< uart > > cout;
   cout << "NRF24L01 test - receive\n";

   gf::use< nrf >::initialize();
   nrf::extensions_toggle();
   nrf::write( nrf::reg::feature, 0x07 );
   nrf::write( nrf::reg::rx_pw_p0, 32 );

   nrf::mode_receive();
   for(;;){
      std::array< uint8_t, 32 > msg;
      uint_fast8_t p;
      int_fast8_t n;
      if( nrf::receive( p, msg, n )){
         cout << "\n********** rx=" << (char) msg[ 0 ] << (char) msg[ 1 ] << (char) msg[ 2 ] <<"\n\n";
      }

      nrf::interrupts_clear();
      timing::ms< 1'000 >::wait();

      // log some info
      if(0)cout
         << "config (F) = "
         << gf::hex << nrf::read( nrf::reg::config )
         << "\n"
         << "status (E) = "
         << gf::hex << nrf::read( nrf::reg::status )
         << "\n"
         << "fifo_status (11) = "
         << gf::hex << nrf::read( nrf::reg::fifo_status )
         << "\n\n";

   }
}

