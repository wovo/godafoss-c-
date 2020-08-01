#include "godafoss.hpp"

namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;
using uart    = target::uart<>;

using spi_1 = gf::spi_bus_bb_sclk_miso_mosi<
   target::d12,
   target::d13,
   target::d9,
   timing
>;
using nrf1 = gf::nrf24l01_spi_ce_csn<
   spi_1,
   target::d11,
   target::d8,
   timing
>;

using spi_2 = gf::spi_bus_bb_sclk_miso_mosi<
   target::d6,
   target::d7,
   target::d3,
   timing
>;
using nrf2 = gf::nrf24l01_spi_ce_csn<
   spi_2,
   target::d5,
   target::d2,
   timing
>;

#define GODAFOSS_TEST_EQUAL( c, a, b ) \
   gf_equal( __FILE__, __LINE__, c, #a, #b, a, b );

int equal_calls_count = 0;
int equal_failures_count = 0;

template< typename ostream, typename A, typename B >
void gf_equal(
   const char * f, int n,            // file name and line number
   ostream & cout,
   const char * ta, const char *tb,  // arguments, stringified
   const A & a, const B & b          // arguments, as-is
){
   ++equal_calls_count;
   if( a != b ){
	  ++equal_failures_count;
      cout
         << f << ":" << gf::dec << n
         << " check failed \n"
         << "   GODAFOSS_TEST_EQUAL( " << ta << " , " << tb << " )\n"
         << "   left  \"" << ta << "\" = " << a << "\n"
         << "   right \"" << tb << "\" = " << b << "\n\n";
   }
}

template< typename ostream >
void test_end( ostream & cout ){
   if( equal_failures_count == 0 ){
      cout
	     << "\nTest success: "
		 << gf::dec << equal_calls_count
		 << " test were successfull\n";
   } else {
      cout
	     << "\nTEST FAILURE: "
		 << gf::dec << equal_failures_count
		 << " test were NOT successfull\n";
   }
}

template< typename nrf, typename ostream >
void dump( ostream & cout ){
   cout
      << gf::hex << gf::setfill( '0' )
      << "config   = 0x" << gf::setw( 2 )
         << nrf::read( nrf::reg::config ) << "\n"
      << "retr     = 0x" << gf::setw( 2 )
         << nrf::read( nrf::reg::setup_retr ) << "\n"
      << "observe  = 0x" << gf::setw( 2 )
         << nrf::read( nrf::reg::observe_tx ) << "\n"
      << "feature  = 0x" << gf::setw( 2 )
         << nrf::read( nrf::reg::feature ) << "\n"
      << "fifo-st  = 0x" << gf::setw( 2 )
         << nrf::read( nrf::reg::fifo_status ) << "\n"
      << "status   = 0x" << gf::setw( 2 )
         << nrf::status() << "\n\n";
}

template< int n, typename T >
void loop( T body ){
   for( int i =0; i < n; ++i ){
      body();
   }
}

template< typename nrf, typename ostream >
void test( ostream & cout ){

   cout << "\nstart single chip tests\n";
   gf::use< nrf >::initialize();

   // test SPI interface

   // rx_addr_p2 reads back what was written

   nrf::write( nrf::reg::rx_addr_p2, 0xAA );
   GODAFOSS_TEST_EQUAL( cout,  nrf::read( nrf::reg::rx_addr_p2 ), 0xAA );

   nrf::write( nrf::reg::rx_addr_p2, 0x55 );
   GODAFOSS_TEST_EQUAL( cout,  nrf::read( nrf::reg::rx_addr_p2 ), 0x55 );

   // writing to another register doesn't influenece rx_addr_p2

   nrf::write( nrf::reg::rx_addr_p2, 0x55 );
   nrf::write( nrf::reg::rx_addr_p3, 0x00 );
   GODAFOSS_TEST_EQUAL( cout,  nrf::read( nrf::reg::rx_addr_p2 ), 0x55 );

   // cd always reads 0, except for the LSB

   nrf::write( nrf::reg::cd, 0x5F );
   GODAFOSS_TEST_EQUAL( cout,  nrf::read( nrf::reg::cd ) & 0xFE, 0x00 );

   // test tx FIFO logic

   // after clearing the tx fifo it is indeed empty

   nrf::write( nrf::cmd::flush_tx );
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_empty(), true );
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_full(), false );

   // in receive mode, a tx message will end up in the tx fifo

   nrf::mode_receive();
   std::array< uint8_t, 32 > tx_data = { 1, 2, 3, 4, 5, 4, 3, 2, 8, 9 };
   nrf::transmit( tx_data );
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_empty(), false );
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_full(), false );

   // two more tx messages will fill the tx fifo

   nrf::transmit( tx_data );
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_empty(), false );
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_full(), false );

   nrf::transmit( tx_data );
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_empty(), false );
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_full(), true );

   // enabling tx clears the tx buffer

   nrf::mode_transmit();
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_empty(), true );
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_full(), false );

   // in transmit mode, sending a message retries untill the count is reached
   // the message remains in the queue

   nrf::mode_transmit();
   nrf::configure( typename nrf::retransmission_attempts{ 2 } );
   nrf::lost_packets_reset();
   GODAFOSS_TEST_EQUAL( cout,  nrf::lost_packets_count(), 0 );
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_empty(), true );
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_full(), false );
   nrf::transmit( tx_data );
   timing::ms< 10 >::wait();
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_empty(), false );
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_full(), false );
   GODAFOSS_TEST_EQUAL( cout,  nrf::lost_packets_count(), 1 );
   GODAFOSS_TEST_EQUAL( cout,  nrf::retransmitted_packets_count(), 2 );

   // with the extensions enabled, a message can be transmitted once
   // the message is removed from the queue
   // surprisingly, retransmitted_packets_count() is not affected

   nrf::mode_transmit();
   nrf::extensions_toggle();
   nrf::write( nrf::reg::feature, 0b0'0000'0001 );
   nrf::lost_packets_reset();
   GODAFOSS_TEST_EQUAL( cout,  nrf::lost_packets_count(), 0 );
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_empty(), true );
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_full(), false );
   nrf::transmit_once( tx_data );
   timing::ms< 10 >::wait();
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_empty(), true );
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_full(), false );
   GODAFOSS_TEST_EQUAL( cout,  nrf::lost_packets_count(), 0 );
   GODAFOSS_TEST_EQUAL( cout,  nrf::retransmitted_packets_count(), 2 );

   // transmitting messages fills the buffer, which empties after some time

   nrf::transmit_once( tx_data );
   nrf::transmit_once( tx_data );
   nrf::transmit_once( tx_data );
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_empty(), false );
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_full(), true );
   timing::ms< 10 >::wait();
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_empty(), true );
   GODAFOSS_TEST_EQUAL( cout,  nrf::transmit_fifo_full(), false );

   loop< 20 >( [&]{
      timing::ms< 10 >::wait();
      nrf::interrupts_clear();
   });

   // dump< nrf >( cout );

   cout << "single chip tests done\n";
}

template< typename nrf1, typename nrf2, typename ostream >
void test( ostream & cout ){

   cout << "\nstart transmit-receive tests\n";
   gf::use< nrf1 >::initialize();
   gf::use< nrf2 >::initialize();


   //std::array< uint8_t, 5 > air_address_1 = { 0xE7, 0xE7, 0xE7, 0xE7, 0xE7 };
   std::array< uint8_t, 5 > air_address_2 = { 0xE0, 0xE7, 0xE7, 0xE7, 0xE7 };

   // transmit two datagrams, no ack

   nrf1::extensions_toggle();
   nrf1::write( nrf1::reg::feature, 0x07 );
   nrf1::write( nrf1::reg::tx_addr, air_address_2 );
   nrf1::write( nrf1::reg::rx_addr_p0, air_address_2 );
   nrf1::write( nrf1::reg::rx_pw_p0, 0 );
   nrf1::configure( nrf1::power::p_18dbm );
   nrf1::mode_transmit();

   nrf1::extensions_toggle();
   nrf1::write( nrf1::reg::feature, 0x07 );
   // nrf2::write( nrf2::reg::tx_addr, air_address_1 );
   nrf2::write( nrf2::reg::rx_addr_p0, air_address_2 );
   nrf2::write( nrf2::reg::rx_pw_p0, 10 );
   nrf2::configure( nrf2::power::p_18dbm );
   nrf2::mode_receive();

   GODAFOSS_TEST_EQUAL( cout,  nrf1::transmit_fifo_empty(), true );
   GODAFOSS_TEST_EQUAL( cout,  nrf2::receive_fifo_empty(), true );

   std::array< uint8_t, 10 > tx_data = { 1, 2, 3, 4, 5, 4, 3, 2, 8, 9 };
   nrf1::transmit_datagram( tx_data );

   GODAFOSS_TEST_EQUAL( cout,  nrf1::transmit_fifo_empty(), false );
   GODAFOSS_TEST_EQUAL( cout,  nrf2::receive_fifo_empty(), true );

   timing::ms< 10 >::wait();

   GODAFOSS_TEST_EQUAL( cout,  nrf1::transmit_fifo_empty(), true );
   GODAFOSS_TEST_EQUAL( cout,  nrf2::receive_fifo_empty(), false );
   GODAFOSS_TEST_EQUAL( cout,  nrf2::receive_next_length(), 10 );
   GODAFOSS_TEST_EQUAL( cout,  nrf2::receive_next_pipe(), 0u );

   nrf1::transmit_datagram( tx_data );
   timing::ms< 10 >::wait();

   std::array< uint8_t, 32 > rx_data_32;
   uint_fast8_t pipe;
   int_fast8_t length;
   bool success = nrf2::receive( pipe, rx_data_32, length );
   GODAFOSS_TEST_EQUAL( cout,  success, 1 );
   GODAFOSS_TEST_EQUAL( cout,  pipe, 0u );
   GODAFOSS_TEST_EQUAL( cout,  length, 10 );
   GODAFOSS_TEST_EQUAL( cout,  tx_data[ 5 ], rx_data_32[ 5 ] );

   GODAFOSS_TEST_EQUAL( cout,  nrf1::transmit_fifo_empty(), true );
   GODAFOSS_TEST_EQUAL( cout,  nrf2::receive_fifo_empty(), false );
   GODAFOSS_TEST_EQUAL( cout,  nrf2::receive_next_length(), 10 );
   GODAFOSS_TEST_EQUAL( cout,  nrf2::receive_next_pipe(), 0u );

   std::array< uint8_t, 32 > rx_data_5;
   success = nrf2::receive( pipe, rx_data_5, length );
   GODAFOSS_TEST_EQUAL( cout,  success, 1 );
   GODAFOSS_TEST_EQUAL( cout,  pipe, 0u );
   GODAFOSS_TEST_EQUAL( cout,  length, 10 );
   GODAFOSS_TEST_EQUAL( cout,  tx_data[ 3 ], rx_data_5[ 3 ] );
   GODAFOSS_TEST_EQUAL( cout,  nrf2::receive_fifo_empty(), true );

   // transmit message, with ack

   GODAFOSS_TEST_EQUAL( cout,  nrf1::transmit_fifo_empty(), true );
   GODAFOSS_TEST_EQUAL( cout,  nrf2::receive_fifo_empty(), true );

   nrf1::transmit( tx_data );
   timing::ms< 10 >::wait();

   GODAFOSS_TEST_EQUAL( cout,  nrf1::transmit_fifo_empty(), true );
   GODAFOSS_TEST_EQUAL( cout,  nrf2::receive_fifo_empty(), false );
   success = nrf2::receive( pipe, rx_data_5, length );
   GODAFOSS_TEST_EQUAL( cout,  nrf2::receive_fifo_empty(), true );

return;

      typename nrf1::air_configuration air_conf1 = {
      typename nrf1::channel{ 2 },
      nrf1::rate::r1Mb,
      nrf1::crc::two_bytes,
      nrf1::power::p_18dbm,
      nrf1::lna::high,
      nrf1::address_length::five_bytes
   };
   typename nrf2::air_configuration air_conf2 = {
      typename nrf2::channel{ 2 },
      nrf2::rate::r1Mb,
      nrf2::crc::two_bytes,
      nrf2::power::p_18dbm,
      nrf2::lna::high,
      nrf2::address_length::five_bytes
   };
   (void)air_conf1;
   (void)air_conf2;



   nrf1::interrupts_clear();
   dump< nrf1 >( cout ); dump< nrf2 >( cout );


   // dump< nrf1 >( cout ); dump< nrf2 >( cout );

/*
   uint8_t p = nrf2::receive_next_pipe();
   GODAFOSS_TEST_EQUAL( cout,  p, 0 );
   uint8_t n = nrf2::read( nrf2::reg::rx_pw_p0 );
   GODAFOSS_TEST_EQUAL( cout,  n, 10 );
   std::array< uint8_t, 10 > rx_data = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
   bool s = nrf2::receive( p, rx_data, len );
   GODAFOSS_TEST_EQUAL( cout,  s, true );
   GODAFOSS_TEST_EQUAL( cout,  p, 0 );
   GODAFOSS_TEST_EQUAL( cout,  n, 10 );
   GODAFOSS_TEST_EQUAL( cout,  rx_data[ 5 ], tx_data[ 5 ] );
*/
   dump< nrf1 >( cout ); dump< nrf2 >( cout );

   cout << "transmit-receive tests tests done\n";
}


int main( void ){
   gf::ostream< gf::formatter< uart > > cout;
   timing::ms< 2'000 >::wait();
   cout << "NRF24L01 test\n";

   //test< nrf1 >( cout );
   //test< nrf2 >( cout );
   test< nrf1, nrf2 >( cout );

   test_end( cout );
}
