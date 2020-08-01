// TRANSMITTER

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
   gf::ostream< gf::formatter< uart > > cout;
   timing::ms< 1000>::wait();
   cout << "NRF24L01 test - transmitter\n";

   gf::use< nrf >::initialize();
   nrf::extensions_toggle();
   nrf::write( nrf::reg::feature, 0x07 );
   nrf::mode_transmit();

   uint8_t n = 0;
   for(;;){

      nrf::interrupts_clear();
      nrf::write( nrf::cmd::flush_tx );
      cout
         << "@ status = "
         << gf::hex << nrf::read( nrf::reg::status )
         << "\n"
         << "@ fifo_status = "
         << gf::hex << nrf::read( nrf::reg::fifo_status )
         << "\n";

      // transmit 'a' ... 'z'
      std::array< uint8_t, 32 > msg;
      n = ( n + 1 ) % 26;
      msg[ 0 ] = 'a' + n;
      n = ( n + 1 ) % 26;
      msg[ 1 ] = 'a' + n;
      n = ( n + 1 ) % 26;
      msg[ 2 ] = 'a' + n;
      nrf::transmit_datagram( msg );

      // log some info
      cout
         << "config = "
         << gf::hex << nrf::read( nrf::reg::config )
         << "\n"
         << "status = "
         << gf::hex << nrf::read( nrf::reg::status )
         << "\n"
         << "fifo_status = "
         << gf::hex << nrf::read( nrf::reg::fifo_status )
         << "\n\n";

      timing::ms< 100 >::wait();
   }
}

