// ==========================================================================
//
// gf-box-invert.hpp
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
struct _invert_read : T {};

template< is_input T >
struct _invert_read< T > : T {
   static auto read(){
      return T::invert( T::read() );
   }
};


// ==========================================================================
//
// write
//
// ==========================================================================
   
template< typename T >
struct _invert_write : T {};

template< is_input T >
struct _invert_write< T > : T {
   static void write( typename T::value_type v ) {
      T::write( T::invert( v ));
   }
};


// ==========================================================================
//
// wrapper
//
// ==========================================================================

template< typename T >
concept bool can_invert = requires {
   is_box< T >;
   { T::invert( T::value_type ) } -> typename T::value_type
};

template< can_invert T >
struct invert : 
   _invert_read< 
      _invert_write< T >> {};
