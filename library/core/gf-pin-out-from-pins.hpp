// ==========================================================================
//
// gf-pin-out-from-pins.hpp
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


// fallback, required but never used
template< typename... arg_tail >
struct _pin_out_from_pins {};

// recursion endpoint
template<> 
struct _pin_out_from_pins<> : 
   pin_out_dummy
{};

// workhorse
template< typename head, typename... tail >
struct _pin_out_from_pins< head, tail... > {
	
   using pin = pin_out< head >	;
	
   static void GODAFOSS_INLINE init(){
      pin::init();
      _pin_out_from_pins<  tail... >::init();	  
   }	
   
   static void GODAFOSS_INLINE write( bool v ){
      pin::write( v );
      _pin_out_from_pins<  tail... >::write( v );	  
   }	   
   
   static void GODAFOSS_INLINE flush(){
      pin::flush();
      _pin_out_from_pins<  tail... >::flush();	  
   }	
   
};

// wrapper
template< can_pin_out_list... tail >
struct pin_out_from_pins :
   be_pin_out,
   _pin_out_from_pins< tail... >
{};
