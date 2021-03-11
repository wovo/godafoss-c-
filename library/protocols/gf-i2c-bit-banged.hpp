// =============================================================================
//
// gf-i2c-bit-banged.hpp
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
   is_i2c_profile  _profile
>
struct _i2c_base_bb_scl_sda {

   using timing   = timing_wait< timing_arg >;
   using profile  = typename _profile::intervals< timing >;

   static constexpr auto frequency = _profile::frequency;

   using scl      = direct< pin_oc< scl_arg > >;
   using sda      = direct< pin_oc< sda_arg > >;

   static void init__(){
      scl::write( 1 );
      sda::write( 1 );
   }

   using resources = use_list< timing, scl, sda, execute< init__ > >;

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

   static void write( uint8_t data ){
      loop< 8 >( [&]{
         write_bit( ( data & 0x80 ) != 0 );
         data = data << 1;
      });
      (void) read_ack();
   }

   static inline bool _first_byte;

   static [[nodiscard]] uint8_t read(){
      if( ! _first_byte ){
         write_ack();
      }
      _first_byte = false;

      uint8_t result = 0;
      loop< 8 >( [&]{
         result = result << 1;
         if( read_bit() ){
            result |= 0x01;
         }
      });

      return result;
   }

   static void start_write_transaction( uint_fast8_t address ){
      write_start();
      write( address << 1 );
   }

   static void stop_write_transaction(){
      write_stop();
   }

   static void start_read_transaction( uint_fast8_t address ){
      write_start();
      write( ( address << 1 ) | 0x01 );
      _first_byte = true;
   }

   static void stop_read_transaction(){
      write_nack();
      write_stop();
   }
};

template<
   can_pin_oc      scl_arg,
   can_pin_oc      sda_arg,
   is_timing_wait  timing_arg,
   is_i2c_profile  profile    = i2c_standard
>
struct i2c_bus_bb_scl_sda :
   i2c_bus<
      _i2c_base_bb_scl_sda< scl_arg, sda_arg, timing_arg, profile >,
      profile >{};
