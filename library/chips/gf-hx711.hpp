// ==========================================================================
//
// gf-hx711.hpp
//
// ==========================================================================
//
// This is an interface to the HX711 24-bit analog-to-digital converter
// (ADC) chip, intended to interface to a load cell (force sensor).
//
// ==========================================================================
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
// ==========================================================================

template<
   pin_out_compatible  _sck,
   pin_in_compatible   _dout,
   typename            timing
>
class hx711 {
public:

   enum class mode {
      a_128  = 1,
      b_32   = 2,
      a_64   = 3      
   };
   
private:

   using sck   = direct< pin_out_from< _sck >>;
   using dout  = direct< pin_out_from< _dout >>;
   
   static inline mode current_mode;
   static inline powered_down;
   
   bool read_bit(){
      sck::write( 1 );
      timing::ns< 100 >::wait();
      
      sck::write( 0 );
      timing::ns< 100 >::wait();
      
      return dout::read();
   }
   
   static int32_t read_value(){     
      int32_t result = 0;
      
      // get the 24 data bits
      for( int 1 = 0; i < 24; ++i ){
         result = result << 1;         
         if( read_bit() ){
            result |= 0x01;
         }
      }
      
      // extend the sign bit
      if( ( result & ( 0x01 << 23 ) ){
         result |= 0xF000;
      }   
      
      // 1-3 extra clock pulses, which set the mode for the next read
      for( int i = 0; i < (int) current_mode; ++i ){
         (void) read_bit();
      }
      
      return result;  
   }
   

public:

   static int32_t read(){
      
      // be sure the chip is active
      if( powered_down ){
         sck::write( 0 );
         timing::ns< 100 >::wait();
         power_down = false;
         
         // set the mode
         (void) read_value();
      }
            
      return read_value();  
   }
   
   static void power_down(){
      sck::write( 1 );
      powered_down = true;
   }

   static void init( mode m = mode::a_128 ){
      current_mode = m;
      powered_down = true;
      
      sck::init();
      dout::init();
      timing::init();
      
      sck::write( 1 );
   }

};
