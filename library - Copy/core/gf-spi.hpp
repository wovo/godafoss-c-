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
private:   
   
   using sclk    = direct< pin_out< _sclk > >;
   using miso    = direct< pin_in<  _miso > >;
   using mosi    = direct< pin_out< _mosi > >;
      
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
   }
    
   static void write_and_read( 
      const uint8_t * data_out, 
            uint8_t * data_in,
      uint_fast16_t n			
   ){
      uint8_t dummy = 0;	   
      for( uint_fast16_t i = 0; i < n; ++i ){
         write_and_read_single( 
           ( data_out != nullptr ) ? data_out[ i ] : dummy, 
           ( data_in  != nullptr ) ? data_in[ i ]  : dummy
	     );
      }      
   }   
   
public:   

   static void init(){
      sclk::init();
      miso::init();
      mosi::init();	  
      timing::init();    
   }
     
   template< can_pin_out _sel >
   struct transfer {
      using sel = direct< pin_out< _sel > >; 	 
	   
	   transfer(){
         sel::init();
         sel::write( 1 );
         wait_half_period();
      }

      ~transfer(){
         wait_half_period();
         sel::write( 0 );
         wait_half_period();
      }

      template< unsigned int n >
      void write_and_read( 
         const std::array< uint8_t, n > & data_out, 
               std::array< uint8_t, n > & data_in,
	     int_fast16_t amount = n
      ){
         spi_bus_bb_sclk_miso_mosi< _sclk, _miso, _mosi, timing  >
            ::write_and_read( 
			   data_out.data(), 
			   data_in.data(), 
			   std::min( amount, (int_fast16_t) n ) );
      }       	  
	   
      void write_and_read( 
         const uint8_t   data_out, 
               uint8_t & data_in
      ){
         spi_bus_bb_sclk_miso_mosi< _sclk, _miso, _mosi, timing  >
            ::write_and_read( 
               &data_out, 
               &data_in, 
               1 );
      }       	  
	   
      template< unsigned int n >
      void write( 
         const std::array< uint8_t, n > & data_out,
	      int_fast16_t amount = n
      ){
         spi_bus_bb_sclk_miso_mosi< _sclk, _miso, _mosi, timing  >
            ::write_and_read( 
               data_out.data(), 
               nullptr, 
               std::min( amount, (int_fast16_t) n ) );
      }      
 	  
      void write( 
         const uint8_t data_out
      ){
         spi_bus_bb_sclk_miso_mosi< _sclk, _miso, _mosi, timing  >         
            ::write_and_read( 
               &data_out, 
               nullptr, 
               1 );
      }      
 	  
      template< unsigned int n >
      void read( 
         std::array< uint8_t, n > & data_in,
	      int_fast16_t amount = n
      ){
         spi_bus_bb_sclk_miso_mosi< _sclk, _miso, _mosi, timing  >         
            ::write_and_read( 
               nullptr, 
               data_in.data(), 
               std::min( amount, (int_fast16_t) n ) );
      }       	  
	  
      void read( 
         uint8_t & data_in
      ){
         spi_bus_bb_sclk_miso_mosi< _sclk, _miso, _mosi, timing  >
            ::write_and_read( 
               nullptr, 
               &data_in, 
               1 );
      }       	  
	  
      uint8_t read_byte(){
         uint8_t d;         
         spi_bus_bb_sclk_miso_mosi< _sclk, _miso, _mosi, timing  >
            ::write_and_read( 
               nullptr, 
               &d, 
               1 );
         return d;
      }       	  
	  
   };  
   
}; // struct spi_bus_bb_sclk_miso_mosi
