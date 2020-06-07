// ==========================================================================
//
// gf-box-invert.hpp
//
// ==========================================================================
//
// The invert<> decorator inverts the value written to or read from a box.
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


// ==========================================================================
//
// invert the read operation (if available)
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
// invert the write operation (if available)
//
// ==========================================================================
   
template< typename T >
struct _invert_write : T {};

template< is_output T >
struct _invert_write< T > : T {
   
   static void write( typename T::value_type v ) {
      T::write( T::invert( v ));
   }

};


// ==========================================================================
//
// invert adapter
//
// ==========================================================================

// invert requires that an invert function is present
template< typename T >
concept can_invert = requires (
   typename T::value_type v   
) {
   is_box< T >;
   { T::invert( v ) } -> std::same_as< typename T::value_type >;
};

// invert both the read and write operations (if available)
template< can_invert T >
struct invert : 
   _invert_read< 
   _invert_write< T >> {};
