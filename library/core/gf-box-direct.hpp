// ==========================================================================
//
// gf-box-direct.hpp
//
// ==========================================================================
//
// This file is part of godafoss, 
// a C++ library for close-to-the-hardware programming.
//
// Copyright Wouter van Ooijen 2019
// 
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================


// ==========================================================================
//
// read
//
// ==========================================================================

template< typename T >
struct _direct_read : T {};

template< is_input T >
struct _direct_read< T > : T {
	
   static auto read(){
      T::refresh();
      return T::read();
   }
   
};


// ==========================================================================
//
// write
//
// ==========================================================================
   
template< typename T >
struct _direct_write : T {};

template< is_input T >
struct _direct_write< T > : T {
	
   static void write( typename T::value_type v ) {
      T::write( v );
      T::flush();
   }
   
};


// ==========================================================================
//
// direction
//
// ==========================================================================
   
template< typename T >
struct _direct_direction : T {};

template< is_simplex T >
struct _direct_direction< T > : T {
	
   static void direction_set_input() {
      T::direction_set_input();
      T::direction_flush();
   }
   
   static void direction_set_output() {
      T::direction_set_output();
      T::direction_flush();
   }
   
};


// ==========================================================================
//
// wrapper
//
// ==========================================================================

template< typename T >
concept bool can_direct = requires {
   is_box< T >;
};

template< can_direct T >
struct direct : 
   _direct_read< 
      _direct_write< 
	     _direct_direction < T >>> {};
