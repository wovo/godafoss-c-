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
   template< int bits = 8 >
   static void write_and_read_single( 
      uint8_t   d_out, 
      uint8_t & d_in 
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
   
   static void write_and_read( 
      const uint8_t * data_out, 
            uint8_t * data_in,
      uint_fast16_t n			
   ){
      uint8_t dummy;	   
      for( uint_fast16_t i = 0; i < n; ++i ){
         write_and_read_single( 
		     ( data_out != nullptr ) ? data_out[ i ] : dummy, 
			  ( data_in  != nullptr ) ? data_in[ i ]  : dummy
	     );
      }      
      wait_half_period();
   }   
     
   template< can_pin_out _sel >
   struct transfer {
      using sel = direct< pin_out< _sel > >; 	 
	   
	   transfer(){
         sel::write( 1 );
         wait_half_period();
      }

      ~transfer(){
         wait_half_period();
         sel::write( 0 );
         wait_half_period();
      }

      template< int n >
      void write_and_read( 
         const std::array< uint8_t, n > & data_out, 
               std::array< uint8_t, n > & data_in
      ){
         spi_bus_bb_sclk_miso_mosi< _sclk, _miso, _mosi, timing  >
         ::write_and_read( data_out, data_in, n );
      }       	  
	   
      template< int n >
      void write( 
         const std::array< uint8_t, n > & data_out
      ){
         spi_bus_bb_sclk_miso_mosi< _sclk, _miso, _mosi, timing  >
         ::write_and_read( data_out, nullptr, 1 );
      }      
 	  
      void write( 
         const uint8_t data_out
      ){
         spi_bus_bb_sclk_miso_mosi< _sclk, _miso, _mosi, timing  >         
         ::write_and_read( &data_out, nullptr, 1 );
      }      
 	  
      template< int n >
      void read( 
               std::array< uint8_t, n > & data_in
      ){
         spi_bus_bb_sclk_miso_mosi< _sclk, _miso, _mosi, timing  >         
         ::write_and_read( nullptr, data_in, n  );
      }       	  
	  
      void read( 
               uint8_t data_in
      ){
         spi_bus_bb_sclk_miso_mosi< _sclk, _miso, _mosi, timing  >
         ::write_and_read( nullptr, &data_in, 1  );
      }       	  
	  
   };  
   
}; // struct spi_bus_bb_sclk_miso_mosi
