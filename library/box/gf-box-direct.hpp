// =============================================================================
//
// gf-box-direct.hpp
//
// =============================================================================
//
// The direct<> decorator inserts the appropriate refresh or flush
// before or after each read, write, or direction change operation.
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
// read
//
// =============================================================================

template< typename T >
struct _direct_read : T {};

template< input T >
struct _direct_read< T > : T {

   static auto read(){
      T::refresh();
      return T::read();
   }

};


// =============================================================================
//
// write
//
// =============================================================================

template< typename T >
struct _direct_write : T {};

template< output T >
struct _direct_write< T > : T {

   using _vt = typename T::value_type;

   static void write( by_const< _vt > v ) {
      T::write( v );
      T::flush();
   }

};


// =============================================================================
//
// direction
//
// =============================================================================

template< typename T >
struct _direct_direction : T {};

template< simplex T >
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


// =============================================================================
//
// opt in to the direct<> decorator and provide it
//
// =============================================================================

template< typename T >
concept can_box_direct = requires {
   item< T >;
};

template< can_box_direct T >
struct direct_supported< T > {
    static constexpr bool supported = true;
};

template< typename T >
   requires can_box_direct< T >
struct direct< T > :
   _direct_read<
   _direct_write<
   _direct_direction < T >>> {};
