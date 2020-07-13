// =============================================================================
//
// gf-item-direct.hpp
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
// @define godafoss::direct
// @title direct
//
// The direct<> decorator accepts an item and decorates it by
// inserting the appropriate refresh or flush
// before or after each read, write, or direction change operation,
// and replacing the refresh and flush operations by empty functions.
//
// The effect is that such a decorated item can be used without
// refresh or flush calls.
//
// @insert can_direct
// @insert direct
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

   static void refresh(){}

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

   static void flush(){}

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

   static void direction_flush(){}

};


// =============================================================================
//
// opt in to the direct<> decorator and provide it
//
// =============================================================================

// @quote can_direct 4
template< typename T >
concept can_direct = requires {
   item< T >;
};

template< can_direct T >
struct direct_supported< T > {
    static constexpr bool supported = true;
};

// @quote direct 3 ... ;
template< typename T >
   requires can_direct< T >
struct direct< T > :
   _direct_read<
   _direct_write<
   _direct_direction < T >>> {};
