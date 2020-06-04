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
   port_oc_compatible  _port,
   pin_oc_compatible   _cs1,
   pin_oc_compatible   _cs2,
   pin_oc_compatible   _cd,
   pin_oc_compatible   _e,
   typename             timing
> struct ks0108_data_cs1_cs2_cd_e_timing : 
   godafoss::window_root<
      ks0108_data_cs1_cs2_cd_e_timing< _port, _cs1, _cs2, _cd, _e, timing >,
      xy< int_fast16_t >,
      black_or_white,
      { 128, 64 }
   >
{
private:

   using root = godafoss::window_root<
      ks0108_data_cs1_cs2_cd_e_timing< _port, _cs1, _cs2, _cd, _e, timing >,
      xy< int_fast16_t >,
      black_or_white,
      { 128, 64 }
   >;
	
   using port    = port_out_from< _port >;  
   using cs1     = pin_oc_from< invert< _cs1 > >;
   using cs2     = pin_oc_from< invert< _cs2 > >;
   using cd      = pin_oc_from< _cd >;
   using e       = pin_oc_from< _e >;
   
   static constexpr uint_fast8_t cmd_on  = 0x3E;
   static constexpr uint_fast8_t cmd_off = 0x3F;
   static constexpr uint_fast8_t cmd_x   = 0xB8;
   static constexpr uint_fast8_t cmd_y   = 0x40;
   static constexpr uint_fast8_t cmd_dsl = 0xC0;  
   
   static void write8( bool is_command, uint_fast8_t d ){
   
      direct< cd >::write( is_command );
      direct< port >::write( d );

      direct< e >::write( 1 );
      timing::template us< 1 >::wait();
      direct< e >::write( 0 );      
      timing::template us< 1 >::wait();
   }      
           
   static void chip( bool c ){
      direct< cs1 >::write( ! c );
      direct< cs2 >::write( c );
   }

   static void GODAFOSS_INLINE command( uint_fast8_t d ){
      write8( true, d );
   }

   static void GODAFOSS_INLINE data( uint_fast8_t d ){
      write8( false, d );
   }
   
   static inline uint8_t buffer[ root::size.x * root::size.y / 8 ];

public:   

   static void init(){
      port::init();  
      cs1::init();
      cs2::init();
      cd::init();
      e::init();
   
      chip( 0 );
      command( cmd_dsl );   
      command( cmd_on );   
      chip( 1 );
      command( cmd_dsl );   
      command( cmd_on );   
   }
   
   static void write_implementation( 
      root::location_t  _pos, 
      root::color_t     col 
   ){
	  const auto pos = _pos - root::origin;
      const uint_fast8_t bit_address   = pos.x + pos.y * root::size.x;
      const uint_fast8_t byte_address  = bit_address / 8;
      const uint_fast8_t bit_offset    = bit_address % 8;
      if( col.is_black ){
         buffer[ byte_address ] |= ( 1 << bit_offset );
      } else {
         buffer[ byte_address ] &= ~ ( 1 << bit_offset );
	  }
   }
   
   static void flush(){
      chip( 0 );	   
      for( int x = 0; x < 8; ++x ){		  
         command( cmd_x + x );
         command( cmd_y + 0 );
         for( int y = 0; y < 64; ++y ){
            // data( buffer[ x * 128 + y ] );		
            data( 0xAA );	 
		 }
      }		 
      chip( 1 );	   
      for( int x = 0; x < 8; ++x ){		  
         command( cmd_x + x );
         command( cmd_y + 0 );
         for( int y = 0; y < 64; ++y ){
            //data( buffer[ x * 128 + 64 + y ] );			 
            data( 0x0FF );	 
		 }
      }		   
   }
 
   
}; // ks0108_data_cs1_cs2_cd_e_timing
