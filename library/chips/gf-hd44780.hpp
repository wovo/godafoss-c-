// =============================================================================
//
// file : hwcpp-hd44780.hpp
//
// =============================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss),
// a C++ library for close-to-the-hardware programming.
//
// Copyright
//    Wouter van Ooijen 2017-2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================


template<
   can_pin_out   _rs,
   can_pin_out   _e,
   can_port_out  _port,
   xy<>          _size,
   typename      timing
> struct _hd44780_rs_e_d_s_timing {
private:

   using rs      = pin_out< _rs >;
   using e       = pin_out< _e >;
   using port    = port_out< _port >;

   using xy_t    = uint_fast8_t;

public:

   static constexpr auto size = _size;
   static inline xy cursor;

private:

   static void write4( uint_fast8_t d ){

      port::write( d );

      // minimum tDBW
      timing::template ns< 200 >::wait();

      e::write( 1 );

      port::flush();
      e::flush();

      // minimum PW-EH
      timing::template ns< 500 >::wait();

      direct< e >::write( 0 );

      // minumum TcycE = 1000 ns - PW-EH
      // snark: must be enlarged to get things working??
      timing::template ns< 100L * 500 >::wait();
   }

   static void write8( bool is_data, uint_fast8_t d ){
      direct< rs >::write( is_data );
      write4( d >> 4 );
      write4( d );

      // enough for most instructions
      // if an instruction needs more, that is his responsibilitty
      timing::template us< 100 >::wait();
   }

public:

   static void GODAFOSS_INLINE command( uint_fast8_t cmd ){
      write8( 0, cmd );
   }

   static void GODAFOSS_INLINE data( uint_fast8_t chr ){
      write8( 1, chr );
   }

   static void clear(){
      command( 0x01 );
      timing::template us< 5'000 >::wait();
      goto_xy( xy( 0, 0 ));
   }

   static void goto_xy( xy<> target ){

      if( size.y == 1 ){

         // 1-line LCDs have a split after the 8'th char
         if( target.x < 8 ){
            command( 0x80 + target.x );
         } else {
            command( 0x80 + 0x40 + ( target.x - 8 ));
         }

      } else {
         if( size.y == 2 ){
            command(
               0x80
               + (( target.y > 0 )
                  ? 0x40
                  : 0x00 )
               + ( target.x )
            );
         } else {
            command(
               0x80
               + (( target.y & 0x01 )
                  ? 0x40
                  : 0x00 )
               + (( target.y & 0x02 )
                  ? 0x14
                  : 0x00 )
               + ( target.x )
             );
         }
      }

      cursor = target;
   }

   static bool GODAFOSS_INLINE write_blocks(){
      return false;
   }

   static void GODAFOSS_INLINE flush(){}

   static void write( char chr ){

      if( size.y == 1 ){
         if( cursor.x == 8 ){
            goto_xy( cursor );
         }
      }

      data( chr );
      ++cursor.x;
   }

   static void init(){

      // init the dependencies
      rs::init();
      e:: init();
      port::init();
      timing::init();

      // give LCD time to wake up
      direct< rs >::write( 0 );
      direct< e >::write( 0 );
      timing::template ms< 40 >::wait();

      // interface initialization: make sure the LCD is in 4 bit mode
      // (magical sequence, taken from the HD44780 data-sheet)
      write4( 0x03 );
      timing::template ms< 5 >::wait();
      write4( 0x03 );
      timing::template us< 100 >::wait();
      write4( 0x03 );
      write4( 0x02 );          // now we are in 4 bit mode

      // functional initialization
      command( 0x28 );         // 4 bit mode, 2 lines, 5x8 font
      command( 0x0C );         // display on, no cursor, no blink
      clear();                 // clear display, 'cursor' home
      goto_xy( xy( 0, 0 ) );   // 'cursor' home
   }

   // using resources = use< rs, e, port, timing, init >;

}; // class _hd44780_rs_e_d_x_y_timing_foundation


// =============================================================================
//
// @define godafoss::hd44780_rs_e_d_s_timing
// @title hd44780
//
// The hd44780 template implements a charcater terminal
// on an hd44780 character lcd.
//
// @insert text
//
// The rs, e and port must connect to the corresponding pins of the lcd.
// The lcd is used in 4-bit mode, so the port must connect to the
// d0..d3 of the lcd, the d4..d7 can be left unconnected.
// Only writes to the lcd are used.
// The _r/w pin must be connected to ground.
//
// The size of the lcd must be specified
// in characters in the x and y direction.
// Common sizes are 16x1, 16x2, 20x2 and 20x4.
//
// The timing is used for the waits as required by the hd44780 datasheet.
//
// @section example
//
// bla blas
//
// =============================================================================

// @quote text 7 { ... };
template<
   can_pin_out   rs,
   can_pin_out   e,
   can_port_out  port,
   xy<>          size,
   typename      timing
> using hd44780_rs_e_d_s_timing =
    terminal<
    _hd44780_rs_e_d_s_timing<
       rs, e, port, size, timing > >;
