// ==========================================================================
//
// gf-ds1302.hpp
//
// ==========================================================================
//
// This is an interface to thge DS1302 time-keeping
// (real-time clock) chip with trickle-charge feature.
//a
// Interface:
//    read(), write()
//       access to register and RAM bytes
//    read_date_and_time, write_date_and_time
//       read
//
// ==========================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss),
// a C++ library for close-to-the-hardware programming.
//
// Copyright
//    Wouter van Ooijen 2019-2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

template<
   can_pin_out     _ce,
   can_pin_out     _sclk,
   can_pin_in_out  _io
>
class ds1302 {
private:

   using ce    = direct< pin_out< _ce >>;
   using sclk  = direct< pin_out< _sclk >>;
   using io    = direct< pin_in_out< _io >>;

public:

   static void init(){
      ce::init();
      sclk::init();
      io::init();
      ce::write( 1 );
   }

private:

   static std::uint8_t write_and_read( std::uint8_t out ){
      std::uint8_t in = 0;
      for( int i = 0; i < 8; ++i ){

         // output a bit, lowest first
         io::write( out & 0x01 );
         out = out >> 1;

         sclk::write( 1 );

         // input a bit, lowest first
         in = in >> 1;
         if( io::read() ){
            in |= 0x80;
         }

         sclk::write( 0 );

      }
      return in;
   }

public:

   static std::uint8_t single_value_read( std::uint8_t a ){
      ce::write( 1 );
      io::direction_set_output();
      (void) write_and_read( 1 + ( a << 1 ) );
      io::direction_set_input();
      uint8_t d = write_and_read( 0 );
      ce::write( 0 );
      return d;
   }

   static void single_value_write( std::uint8_t a, std::uint8_t d ){
      ce::write( 1 );
      io::direction_set_output();
      (void) write_and_read( 0 + ( a << 1 )) ;
      (void) write_and_read( d );
      ce::write( 0 );
   }

private:

   static uint8_t two_bcd_read( uint8_t a, uint8_t mask = 0x0F ){
      uint8_t d = single_value_read( a );
      return 10 * ( ( d >> 4 ) & mask ) + ( d & 0x0F );
   }

public:

   static date_and_time read(){
      date_and_time dt;
      dt.seconds  = two_bcd_read( 0x40, 0x0F );
      dt.minutes  = two_bcd_read( 0x41, 0x0F );
      dt.hours    = two_bcd_read( 0x42, 0x0F );
      dt.day      = two_bcd_read( 0x43, 0x0F );
      dt.month    = two_bcd_read( 0x44, 0x0F );
      dt.year     = two_bcd_read( 0x45, 0x0F );
      dt.weekday  = two_bcd_read( 0x45, 0x0F );
      return dt;
   }

   static void write( const date_and_time & dt ){
      single_value_write( 0x40, dt.seconds );
      single_value_write( 0x41, dt.minutes );
      single_value_write( 0x42, dt.hours );
      single_value_write( 0x43, dt.day );
      single_value_write( 0x44, dt.month );
      single_value_write( 0x45, dt.year );
   }

};
