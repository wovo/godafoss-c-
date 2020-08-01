// =============================================================================
//
// gf-item-no-inline.hpp
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
// @define godafoss::no_inline
// @title no_inline
//
// The no_inline<> item decorator creates an item for which all functions
// are not inline.
//
// This can be used as the outermost decorator around an item constructed
// from a chain of inheritances, in which the chain of function calls
// is all marked GODAFOSS_INLINE.
//
// @insert no_inline
//
// =============================================================================


// =============================================================================
//
// write
//
// =============================================================================

template< typename T >
struct _no_inline_item_write : T {};

template< is_output T >
struct _no_inline_item_write< T > : T {

   using _vt = typename T::value_type;

   static void write( by_const< _vt > v ){
      T::write( v );
   }

   static void flush(){
      T::flush();
   }

};


// =============================================================================
//
// read
//
// =============================================================================

template< typename T >
struct _no_inline_item_read : T {};

template< is_input T >
struct _no_inline_item_read< T > : T {

   using _vt = typename T::value_type;

   static _vt read(){
      return T::read();
   }

   static void refresh(){
      T::refresh();
   }

};


// =============================================================================
//
// direction
//
// =============================================================================

template< typename T >
struct _no_inline_item_direction : T {};

template< is_simplex T >
struct _no_inline_item_direction< T > : T {

   static void direction_set_output(){
      T::direction_set_output();
   }

   static void direction_set_input(){
      T::direction_set_input();;
   }

   static void direction_flush(){
      T::direction_flush();
   }

};


// =============================================================================
//
// no_inline
//
// =============================================================================

// @quote no_inline 2 ... ;
template< is_item T >
using no_inline =
   _no_inline_item_write<
   _no_inline_item_read<
   _no_inline_item_direction< T > > >;
