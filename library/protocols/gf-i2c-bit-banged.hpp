// =============================================================================
//
// gf-i2c.hpp
//
// =============================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss),
// a C++ library for close-to-the-hardware programming.
//
// Copyright
//    Wouter van Ooijen 2018-2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================





// =============================================================================
//
// bit-banged implementation
//
// =============================================================================

template<
   can_pin_oc      scl_arg,
   can_pin_oc      sda_arg,
   is_timing_wait  timing_arg,
   template< is_timing_wait > typename _profile = i2c_standard
>
struct i2c_bus_bb_scl_sda {

   using timing = timing_wait< timing_arg >;
   using profile  = _profile< timing >;

   using bus = i2c_bus_bb_scl_sda<
      scl_arg, sda_arg, timing, _profile >;

   using scl      = direct< pin_oc< scl_arg > >;
   using sda      = direct< pin_oc< sda_arg > >;

   static void write_bit( bool x ){

      scl::write( 0 );
      profile::t_su_dat::wait();

      sda::write( x );
      profile::t_low::wait();

      scl::write( 1 );
      profile::t_high::wait();
   }

   static [[nodiscard]] bool read_bit(){

      scl::write( 0 );
      sda::write( 1 );
      profile::t_low::wait();

      scl::write( 1 );
      bool result = sda::read();
      profile::t_high::wait();

      return result;
   }

   static void write_start(){

      sda::write( 0 );
      profile::t_hd_sta::wait();

      scl::write( 0 );
   }

   static void write_stop(){

      scl::write( 0 );
      sda::write( 0 );
      profile::t_low::wait();

      scl::write( 1 );
      profile::t_su_sto::wait();

      sda::write( 1 );
      profile::t_buf::wait();
   }

   static [[nodiscard]] bool read_ack(){
      return ! read_bit();
   }

   static void write_ack(){
      write_bit( 0 );
   }

   static void write_nack(){
      write_bit( 1 );
   }

   static void write_byte( uint8_t x ){
      loop< 8 >( [&]{
         write_bit( ( x & 0x80 ) != 0 );
         x = x << 1;
      });
   }

   static [[nodiscard]] uint8_t read_byte(){
      uint8_t result = 0;
      loop< 8 >( [&]{
         result = result << 1;
         if( read_bit() ){
            result |= 0x01;
         }
      });
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

   struct _write_transaction {

      _write_transaction( uint8_t address ){
         bus::write_start();
         bus::write_byte( address << 1 );
      }

      ~_write_transaction(){
         bus::read_ack();
         bus::write_stop();
      }

      void write( const uint8_t data[], int n ){
         for( int i = 0; i < n; ++i ){
            bus::read_ack();
            bus::write_byte( data[ i ] );
         };
      }

      void write( const uint8_t data ){
         write( &data, 1 );
      }

   };

   struct _read_transaction {

      bool first;

      _read_transaction( uint8_t address ):
         first( true )
      {
         write_start();
         write_byte( ( address << 1 ) | 0x01 );
         read_ack();
      }

      ~_read_transaction(){
         write_nack();
         write_stop();
      }

      void read( uint8_t data[], int n ){
         for( int i = 0; i < n; ++i ){
            if( ! first ){
               write_ack();
            }
            first = false;
            data[ i ] = read_byte();
         }
      }

      void read( uint8_t & data ){
         read( & data, 1 );
      }

   };

   template< int address >
   struct channel {

      struct read_transaction {

         bool first;

         read_transaction():
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
            for( int i = 0; i < n; ++i ){
               if( ! first ){
                  write_ack();
               }
               first = false;
               data[ i ] = read_byte();
            }
         }

         void read( uint8_t & data ){
            read( & data, 1 );
         }

      };

      struct write_transaction {

         write_transaction(){
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
            };
         }

         void write( const uint8_t data ){
            write( &data, 1 );
         }

      };

   };

};
