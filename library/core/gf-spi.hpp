// ==========================================================================
//
// file : gf-spi.hpp
//
// ==========================================================================

template<
   can_pin_out  _sclk,
   can_pin_in   _miso,
   can_pin_out  _mosi,
   typename     timing   
>
struct spi_bus_bb_sclk_miso_mosi {

   using sclk    = direct< pin_out< _sclk > >;
   using miso    = direct< pin_in<  _miso > >;
   using mosi    = direct< pin_out< _mosi > >;
   
   static void init(){
      sclk::init();
      miso::init();
      mosi::init();	  
	   timing::init();
   }
   
   //for now: 1 MHz
   static void wait_half_period(){
      timing::template ns< 500 >::wait();
   }
   
   // must implement other SPI modes
   template< typename data_type, int bits = 8 >
   static void write_and_read_single( 
      data_type d_out, 
      data_type & d_in 
   ){
      d_in = 0;
      for( uint_fast8_t i = 0; i < bits; ++i ){
         mosi::write( ( d_out & ( 0x01 << ( bits - 1 ) ) ) != 0 );		  
         wait_half_period();
         sclk::write( 1 );
         wait_half_period();
         d_out = d_out << 1;
         d_in = d_in << 1;
         d_in |= ( miso::read() ? 0x01 : 0x00 );
         sclk::write( 0 );
      } 
      mosi::write( 0 );
   }
   
   template< size_t n > 
   static void write_and_read( 
      const std::array< uint8_t, n > & data_out, 
            std::array< uint8_t, n > & data_in
   ){
      for( uint_fast8_t i = 0; i < n; ++i ){
         write_and_read_single( data_out[ i ], data_in[ i ] );
      }      
      wait_half_period();
   }   
   
   // flyweight handling of chip select
   template< typename sel, size_t n > 
   static void write_and_read( 
      const std::array< uint8_t, n > & data_out, 
            std::array< uint8_t, n > & data_in
   ){
      sel::write( 0 );
      write_and_read( data_out, data_in );
      sel::write( 1 );
      wait_half_period();
   }   
   
}; // struct spi_bus_bb_sclk_miso_mosi
