// =============================================================================
//
// gf-hx711.hpp
//
// =============================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss),
// a C++ library for close-to-the-hardware programming.
//
// Copyright
//    Wouter van Ooijen 2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================
//
// @title hx711
//
// @define godafoss::hx711
//
// This template implements an interface to the
// hx711 24-Bit Analog-to-Digital Converter (ADC).
// This chip is intended to interface to a load cell (force sensor).
//
// @insert text
//
// The chip interface consist of a master-to-slave clock pin (sck),
// and a slave-to-master data pin (dout).
//
// The timing is used for the waits as required by the hx711 datasheet.
//
// The mode offers a choice between the A differential inputs with
// a gain of 128 or 64, and the B inputs with a gain of 32.
// The A inputs are meant to be used with a load cell.
// The datasheet suggest that the B inputs could be used to monitior
// the battery voltage.
// The mode is set at the initialization (the defauylt is a_128),
// and can be changed by the mode_set() function.
//
// The chip can be powered down. When a read is done the chip
// is first (automatically) powered up.
//
// =============================================================================

// @quote text 6
template<
   can_pin_out  _sck,
   can_pin_in   _dout,
   typename            timing
>
struct hx711 {
public:

   // @quote text 5
   enum class mode {
      a_128  = 1,  // A inputs, gain 128
      b_32   = 2,  // B inputs, gain 32
      a_64   = 3   // A inputs, gain 64
   };

private:

   using sck   = direct< pin_out< _sck >>;
   using dout  = direct< pin_out< _dout >>;

   static inline mode current_mode;
   static inline bool powered_down;

   bool read_bit(){
      sck::write( 1 );
      timing:: template ns< 100 >::wait();

      sck::write( 0 );
      timing:: template ns< 100 >::wait();

      return dout::read();
   }

   static int32_t read_value(){
      int32_t result = 0;

      // get the 24 data bits
      for( int i = 0; i < 24; ++i ){
         result = result << 1;
         if( read_bit() ){
            result |= 0x01;
         }
      }

      // extend the sign bit
      if( result & ( static_cast< uint32_t >( 0x01 ) << 23 ) ){
         result |= 0xF000;
      }

      // 1-3 extra clock pulses, which set the mode for the next read
      for( int i = 0; i < (int) current_mode; ++i ){
         (void) read_bit();
      }

      return result;
   }


public:

   // @quote text 1 ... }
   static void init( mode m = mode::a_128 ){
      current_mode = m;
      powered_down = true;

      sck::init();
      dout::init();
      timing::init();

      sck::write( 1 );
   }

   // @quote text 1 ... }
   static int32_t read(){

      // be sure the chip is active
      if( powered_down ){
         sck::write( 0 );
         timing:: template ns< 100 >::wait();
         power_down = false;

         // set the mode
         (void) read_value();
      }

      return read_value();
   }

   // @quote text 1 ... }
   static void power_down(){
      sck::write( 1 );
      powered_down = true;
   }

   // @quote text 1 ... }
   static void mode_set( mode m ){
      current_mode = m;

      // force a dummy read that puts the chip in the new mode
      powered_down = true;
   }

// @quote text 1
};
