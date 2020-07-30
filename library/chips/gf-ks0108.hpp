// ==========================================================================
//
// file : gf-ks0108.hpp
//
// ==========================================================================
//
// This file is part of HwCpp,
// a C++ library for close-to-the-hardware programming.
//
// Copyright Wouter van Ooijen 2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

template<
   can_port_oc  _port,
   can_pin_oc   _cs1,
   can_pin_oc   _cs2,
   can_pin_oc   _cd,
   can_pin_oc   _e,
   typename             timing
> struct ks0108_data_cs1_cs2_cd_e_timing :

   godafoss::window_root<
      ks0108_data_cs1_cs2_cd_e_timing< _port, _cs1, _cs2, _cd, _e, timing >,
      xy< int_fast16_t >,
      color_bw,
      { 128, 64 }
   >
{
private:

   using root = godafoss::window_root<
      ks0108_data_cs1_cs2_cd_e_timing< _port, _cs1, _cs2, _cd, _e, timing >,
      xy< int_fast16_t >,
      color_bw,
      { 128, 64 }
   >;

   // cs1, cs2, e are (internally) active high
   //(the electrical pins are active low)
   using port    = port_out< _port >;
   using cs1     = pin_out< invert< _cs1 > >;
   using cs2     = pin_out< invert< _cs2 > >;
   using cd      = pin_out< _cd >;
   using e       = pin_out< invert< _e > >;

   // KS0108 commands
   static constexpr uint_fast8_t cmd_off = 0x3E;
   static constexpr uint_fast8_t cmd_on  = 0x3F;
   static constexpr uint_fast8_t cmd_x   = 0xB8;
   static constexpr uint_fast8_t cmd_y   = 0x40;
   static constexpr uint_fast8_t cmd_dsl = 0xC0;

   // pixel buffer
   static inline uint8_t buffer[ root::size.x * root::size.y / 8 ] = {};

   // write on byte, command or data
   static void write8( bool is_data, uint_fast8_t d ){

      timing::template us< 10 >::wait();

      // select command or data
      direct< cd >::write( is_data );

      timing::template us< 10 >::wait();

      // write the data byte
      direct< port >::write( d );

      timing::template us< 10 >::wait();

      // e pulse
      direct< e >::write( 0 );
      timing::template us< 10 >::wait();
      direct< e >::write( 1 );
      timing::template us< 10 >::wait();
   }

   // select chip 0 or chip 1, 2 selects both chips
   static void select_chip( int c ){
      direct< cs1 >::write( c != 1 );
      direct< cs2 >::write( c != 0 );
   }

   // write one command byte
   static void GODAFOSS_INLINE write_command( uint_fast8_t d ){
      write8( false, d );
   }

   // write one data byte
   static void GODAFOSS_INLINE write_data( uint_fast8_t d ){
      write8( true, d );
   }

public:

   static void init(){
      port::init();
      cs1::init();
      cs2::init();
      cd::init();
      e::init();

      timing::template us< 100'000 >::wait();

      select_chip( 0 );
      write_command( cmd_dsl );
      write_command( cmd_on );

      select_chip( 1 );
      write_command( cmd_dsl );
      write_command( cmd_on );

      timing::template us< 1000 >::wait();

      select_chip( 0 );
      write_command( cmd_dsl );
      write_command( cmd_on );

      select_chip( 1 );
      write_command( cmd_dsl );
      write_command( cmd_on );
   }

   static void write_implementation(
      root::offset_t   pos,
      root::color_t    col
   ){
      const uint_fast8_t bit_address   = pos.x + pos.y * root::size.x;
      const uint_fast8_t byte_address  = bit_address / 8;
      const uint_fast8_t bit_offset    = bit_address % 8;
      if( col == black ){
         buffer[ byte_address ] |= ( 1 << bit_offset );
      } else {
         buffer[ byte_address ] &= ~ ( 1 << bit_offset );
	  }
   }

   static void flush(){
      select_chip( 0 );
      for( int x = 0; x < 8; ++x ){
         write_command( cmd_x + x );
         write_command( cmd_y + 0 );
         for( int y = 0; y < 64; ++y ){
            // write_data( buffer[ x * 128 + y ] );
            write_data( y );
         }
      }
      select_chip( 1 );
      for( int x = 0; x < 8; ++x ){
         write_command( cmd_x + x );
         write_command( cmd_y + 0 );
         for( int y = 0; y < 64; ++y ){
            //write_data( buffer[ x * 128 + 64 + y ] );
            write_data( 0x0F );
         }
      }
   }

};
