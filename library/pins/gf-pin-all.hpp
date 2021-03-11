// ==========================================================================
//
// gf-pin-all.hpp
//
// ==========================================================================
//
// A write on the the pin all< p1, ... > writes to all pins p1...
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


// fallback, required but never used
template< typename... arg_tail >
struct _pin_out_from_pins {};

// recursion endpoint
template<>
struct _pin_out_from_pins<> :
   pin_out_dummy
{};

// the workhorse
template< typename head, typename... tail >
struct _pin_out_from_pins< head, tail... > {

   using pin = pin_out_from< head > ;

   //static void GODAFOSS_INLINE init(){
   //   pin::init();
   //   _pin_out_from_pins<  tail... >::init();
   //}
   using resources = use_list< pin, _pin_out_from_pins<  tail... > >;

   static void GODAFOSS_INLINE write( bool v ){
      pin::write( v );
      _pin_out_from_pins<  tail... >::write( v );
   }

   static void GODAFOSS_INLINE flush(){
      pin::flush();
      _pin_out_from_pins<  tail... >::flush();
   }

};

template< typename... Ts >
concept can_pin_out_from_list =
   ( can_pin_out_from< Ts > && ... );

// wrapper
template< typename first, typename... tail >
   requires can_pin_out_from< first >
         && can_pin_out_from_list< tail... >
struct all_pins:
   pin_out_root,
   _pin_out_from_pins< first, tail... >
{};
