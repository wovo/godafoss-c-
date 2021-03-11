// ==========================================================================
//
// gf-demos.hpp
//
// ==========================================================================
//
// This file is part the https://www.github.com/godafoss
// free C++ library for close-to-the-hardware programming.
//
// Copyright Wouter van Ooijen 2019
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

template< typename T >
constexpr T demo_ones( int n ){
   return ( n > 0 )
      ? (T) 0x01 | ( demo_ones< T>( n - 1 ) << 1 )
      : 0;
}

template< typename T, int size, int len >
constexpr T demo_pattern( int pos ){
   return
      ( demo_ones< T >( len ) << pos )
      | ( demo_ones< T >( pos + len - size ) );
}

template< can_port_out _port, typename interval, uint64_t n = 1, uint64_t step = 1 >
struct walk {

   using port = direct< port_out< _port >>;

   using resources = use_list< port, interval >;

   static GODAFOSS_NO_RETURN void run(){
      for(;;){
//         constexpr typename port::value_type pattern = ( 0x01LL << n ) - 1;
         for( uint_fast8_t  i = 0; i < port::n_pins; i += step ){
            port::write(
               demo_pattern< typename port::value_type, port::n_pins, n >( i ) );
            interval::wait();
         }
      }
   }
};

template< can_port_out _port, typename interval, uint64_t n = 1, uint64_t step = 1 >
struct kitt {

   using port = direct< port_out< _port > >;

   using resources = use_list< port, interval >;

   static GODAFOSS_NO_RETURN void run(){
      for(;;){
         for( uint_fast8_t  i = 0; i < port::n_pins - ( n - 1 ); i += step ){
            port::write(
               demo_pattern< typename port::value_type, port::n_pins, n>( i ) );
            interval::wait();
         }
         for( uint_fast8_t  i = port::n_pins - n; i > 0; i -= step ){
            port::write(
               demo_pattern< typename port::value_type, port::n_pins, n>( i ) );
            interval::wait();
         }
      }
   }
};
