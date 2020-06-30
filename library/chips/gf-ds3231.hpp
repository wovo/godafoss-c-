// ==========================================================================
//
// gf-ds3231.hpp
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

#ifdef nono
template<
   pin_out_compatible     _ce,
   pin_out_compatible     _sclk,
   pin_in_out_compatible  _io
>
class ds3231 {
private:

   using ce    = direct< pin_out_from< _ce >>;
   using sclk  = direct< pin_out_from< _sclk >>;
   using io    = direct< pin_in_out_from< _io >>;

public:

   static void GODAFOSS_INLINE init(){
      ce::init();
      sclk::init();
      io::init();
      ce::write( 1 );
   }

private:

   static uint8_t GODAFOSS_INLINE write_and_read( unit8_t out ){
      unit8_t in = 0;
      for( int i = 0; i < 8; ++i ){

         // output a bit, lowest first
         io::write( a & 0x01 );
         a = a >> 1;

         sclk::write( 1 );

         // input a bit, lowest first
         out = out >> 1;
         if( io::read() ){
            out |= 0x80;
         }

         sclk::write( 0 );

      }
      return in;
   }

public:

   static uint8_t GODAFOSS_INLINE read( uint8_t a ){
      ce::write( 1 );
      io::direction_set_output();
      (void) write_and_read( 1 + ( a << 1 ) );
      io::direction_set_input();
      uint8_t d = write_and_read( 0 );
      ce::write( 0 );
      return d;
   }

   static void GODAFOSS_INLINE write( uint8_t a, unit8_t d ){
      ce::write( 1 );
      io::direction_set_output();
      (void) write_and_read( 0 + ( a << 1 )) ;
      (void) write_and_read( d );
      ce::write( 0 );
   }

public:

   struct value {
      uint8_t tens, units, total;
   };

private:

   static void value read_value( uint8_t a, uint8_t mask ){
      uint8_t d = read( a );
      value v;
      v.units = d & 0x0F;
      v.tens = ( d >> 4 ) & mask;
      v.total =10 *  v.tens + v.units;
      return v;
   }

public:

   struct date_and_time {
      value seconds, minutes, hours, day, month, year;
   };

   static void read_date_and_time(){
      date_and time dt;
      dt.seconds  = read_value( 0x40, 0x0F );
      dt.minutes  = read_value( 0x41, 0x0F );
      dt.hours    = read_value( 0x42, 0x0F );
      dt.day      = read_value( 0x43, 0x0F );
      dt.month    = read_value( 0x44, 0x0F );
      dt.year     = read_value( 0x45, 0x0F );
      return dt;
   }

   static void write_date_and_time( const date_and time & dt ){
      write_value( 0x40, dt.seconds );
      write_value( 0x41, dt.minutes );
      write_value( 0x42, dt.hours );
      write_value( 0x43, dt.day );
      write_value( 0x44, dt.month );
      write_value( 0x45, dt.year );
      return dt;
   }

};

#endif
