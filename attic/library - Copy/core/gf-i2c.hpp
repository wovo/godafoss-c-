// ==========================================================================
//
// file : gf-i2c.hpp
//
// ==========================================================================   
   

// ==========================================================================
//
// i2c profiles
//
// according to the "I2C-bus specification and user manual, 
// 4 April 2014", UM10204.pdf, Table 10, p 48
//
// ==========================================================================

/*
template< typename T >
concept bool is_uint8 = std::is_same< 
   T, uint8_t >::value; 

template< typename T >
concept bool is_uint8_iterator = std::is_same< 
   std::remove_reference_t< decltype( *std::begin( std::declval< T& >() ) ) >, 
   uint8_t >::value;

template< typename T >
concept bool _provides_uint8 = 
   is_uint8< T >  
   || is_uint8_iterator< T >; 
*/

struct i2c_profile_root {
   static constexpr bool is_i2c_profile = true;
};

/*
template< typename T >
concept is_i2c_profile(){
   return T::is_i2c_profile;
}   
*/

// ========== 100 kHz

using ns_type = uint64_t;

struct i2c_profile_100kHz {
   static constexpr uint_fast64_t  f         = 100'000;
   static constexpr ns_type        t_hd_sta  = 4000;
   static constexpr ns_type        t_low     = 4700;
   static constexpr ns_type        t_high    = 4000;
   static constexpr ns_type        t_su_sta  = 4700;
   //static constexpr ns_type        t_hd_sta  =    0;
   static constexpr ns_type        t_su_dat  =  250;
   static constexpr ns_type        t_su_sto  = 4000;
   static constexpr ns_type        t_buf     = 4700;
};	

// ========== 400 kHz
      
struct i2c_profile_400kHz {
   static constexpr uint_fast64_t  f         = 400'000;
   static constexpr ns_type        t_hd_sta  =  600;
   static constexpr ns_type        t_low     = 1300;
   static constexpr ns_type        t_high    =  600;
   static constexpr ns_type        t_su_sta  =  600;
   //static constexpr ns_type        t_hd_sta  =    0;
   static constexpr ns_type        t_su_dat  =  100;
   static constexpr ns_type        t_su_sto  =  600;
   static constexpr ns_type        t_buf     = 1300;
};	

      
// ==========================================================================
//
// bit-banged implementation
//
// ==========================================================================  

template< 
   can_pin_oc  scl_arg, 
   can_pin_oc  sda_arg, 
   typename    timing, 
   typename    _profile = i2c_profile_100kHz
>
struct i2c_bus_bb_scl_sda {
   using profile  = _profile;
   
   using bus = i2c_bus_bb_scl_sda< 
      scl_arg, sda_arg, timing, _profile >;

   using scl      = direct< pin_oc< scl_arg > >;
   using sda      = direct< pin_oc< sda_arg > >; 
   
   static void write_bit( bool x ){
      scl::write( 0 );
      timing::template ns< profile::t_low - profile::t_su_dat >::wait();
      sda::write( x );
      timing::template ns< profile::t_su_dat >::wait();
      scl::write( 1 ); 
      timing::template ns< profile::t_high >::wait();
   }
   
   static bool read_bit(){         
      scl::write( 0 );
      timing::template ns< profile::t_low - profile::t_su_dat >::wait();         
      sda::write( 1 );
      timing::template ns< profile::t_su_dat >::wait();         
      scl::write( 1 );
      bool result = sda::read();    
      timing::template ns< profile::t_high >::wait();
      return result;
   }       
     
   static void write_start(){
      sda::write( 1 );
      scl::write( 1 );
      timing::template ns< profile::t_su_sta >::wait();
      sda::write( 0 );
      timing::template ns< profile::t_hd_sta >::wait();
      scl::write( 0 );
   }

   static void write_stop(){
      scl::write( 0 );
      timing::template ns< profile::t_low - profile::t_su_dat >::wait();         
      sda::write( 0 );
      timing::template ns< profile::t_su_dat >::wait(); 
      scl::write( 1 );
      timing::template ns< profile::t_su_sto >::wait();   
      sda::write( 1 );
      timing::template ns< profile::t_buf >::wait();
   }
       
   static bool read_ack(){
      return ! read_bit(); 
   } 
   
   static void write_ack(){
      write_bit( 0 );
   }

   static void write_nack(){
      write_bit( 1 );
   }
   
   static void write_byte( uint8_t x ){
      for( int i = 0; i < 8; ++i){
         write_bit( ( x & 0x80 ) != 0 );
         x = x << 1;
      }         
   }

   static uint8_t read_byte(){
      uint8_t result = 0;
      for( int i = 0; i < 8; ++i ){
         result = result << 1;
         if( read_bit() ){
            result |= 0x01;
         } 
      }   
      return result;     
   }        
   
public:     
     
   static void init(){
      timing::init();
      scl::init();
      sda::init();
      
      scl::write( 1 );
      sda::write( 1 );
   }

   struct write_transaction {

      write_transaction( uint8_t address ){
         bus::write_start();
         bus::write_byte( address << 1 );      
      }

      ~write_transaction(){
         bus::read_ack();
         bus::write_stop();
      }

      void write( const uint8_t data[], int n ){
         for( int i = 0; i < n; ++i ){
            bus::read_ack();
            bus::write_byte( data[ i ] );
         }               
      }  

      void write( const uint8_t data ){
         write( &data, 1 );             
      }  

   };

   struct read_transaction {

      bool first;

      read_transaction( uint8_t address ): 
         first( true )
      {
         write_start();
         write_byte( ( address << 1 ) | 0x01 );    
         read_ack();
      }

      ~read_transaction(){
         write_nack();
         write_stop();
      }

      void read( uint8_t data[], int n ){
         if( ! first ){
            write_ack();
         }
         first = false;
         for( int i = 0; i < n; ){
            data[ i ] = read_byte();
         }             
      }      

      void read( uint8_t & data ){
         read( & data, 1 );            
      }      

   };

	  
};// i2c_bus_bb_scl_sda
