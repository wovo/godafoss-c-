// =============================================================================
//
// gf-item-invert.hpp
//
// =============================================================================
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
// =============================================================================

// =============================================================================
//
// @define godafoss::invert
// @title invert
//
// The invert<> decorator inverts the value written to or read from an item.
//
// @insert can_invert
// @insert invert
//
// =============================================================================

// =============================================================================
//
// invert the read operation (if available)
//
// =============================================================================

template< typename T >
struct _invert_read : T {};

template< is_input T >
struct _invert_read< T > : T {

   static auto read(){
      return T::invert( T::read() );
   }

};


// =============================================================================
//
// invert the write operation (if available)
//
// =============================================================================

template< typename T >
struct _invert_write : T {};

template< is_output T >
struct _invert_write< T > : T {

   using _vt = typename T::value_type;

   static void write( by_const< _vt > v ) {
      T::write( T::invert( v ));
   }

};


// =============================================================================
//
// opt into the invert decorator
//
// =============================================================================

// @quote can_invert 7
// invert is supported for an item that has an invert function
template< typename T >
concept can_invert = requires (
      typename T::value_type v
   ){
      { T::invert( v ) } -> std::same_as< typename T::value_type >;
   }
   && is_item< T >;

template< can_invert T >
struct invert_supported< T > {
   static constexpr bool supported = true;
};

// @quote invert 2 ... ;
template< can_invert T >
struct invert< T >
// invert both the read and write operations (if present)
:
   _invert_read<
   _invert_write< T >> {};
