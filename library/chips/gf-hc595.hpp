// ==========================================================================
//
// gf-595.hpp
//
// ==========================================================================
//
// This is the interface to the SPI-like 74HC(T)595 chip(s).
//
// It provides access to the individual pins as pin_out's, and
// to all pins together as a port_out.
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
   typename            bus,
   can_pin_out  _cs,
   uint32_t            n_chips = 1
>
struct hc595 :
   port_out_root< 8 * n_chips >
{
   using chip = hc595< bus, _cs >;
   using root = port_out_root< 8 * n_chips >;

   using cs = direct< pin_out< _cs >>;

   static inline root::value_type buffer;
   static inline bool dirty;

   static void GODAFOSS_INLINE init(){
      dirty = false;
      bus::init();
      cs::init();
   }

   static void GODAFOSS_INLINE write( root::value_type d ){
      buffer = d;
      dirty = true;
   }

   static void GODAFOSS_INLINE flush(){
      if( dirty ){
         typename bus:: template transfer< cs >().write( buffer );
         dirty = false;
      }
   }

   template< int n > struct pin :
      pin_out_root
   {

      static GODAFOSS_INLINE void init(){
         chip::init();
      }

      static GODAFOSS_INLINE void write( bool v ) {
         if( v ){
            chip::buffer |= ( 1 << n );
         } else {
            chip::buffer &= ~( 1 << n );
         }
         chip::dirty = true;
      }

      static GODAFOSS_INLINE void flush() {
         chip::flush();
      }

   };

   using p0 = pin< 0 >;
   using p1 = pin< 1 >;
   using p2 = pin< 2 >;
   using p3 = pin< 3 >;
   using p4 = pin< 4 >;
   using p5 = pin< 5 >;
   using p6 = pin< 6 >;
   using p7 = pin< 7 >;

};
