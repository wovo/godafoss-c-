// =============================================================================
//
// gf-item-inherit.hpp
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
// @title inherit_*
//
// Adapters for selectively inheriting only the init,
// read, write, or direction functions of a item.
// This is used or instance in the item_input<> adapter, to
// 'pass' only the input functionality.
//
// @define godafoss::inherit_init
// @section inherit_init
// @insert inherit_init
//
// The inherit_init decorator inherits only the init() function of
// the decorated item.
//
// @define godafoss::inherit_read
// @section inherit_read
// @insert inherit_read
//
// The inherit_read decorator inherits only the refresh() and read()
// functions of the decorated item.
//
// @define godafoss::inherit_write
// @section inherit_write
// @insert inherit_write
//
// The inherit_read decorator inherits only the write() and flush()
// functions of the decorated item.
//
// @define godafoss::inherit_direction
// @section inherit_direction
// @insert inherit_direction
//
// The inherit_read decorator inherits only the
// direction_set_input(), direction_set_output() and
// direction_flush()
// functions of the decorated item.
//
// =============================================================================


// =============================================================================
//
// init
//
// =============================================================================

// @quote inherit_init 2 = ... ;
template< typename T >
struct inherit_init
{};

template< is_item T >
struct inherit_init< T > {

   //static void GODAFOSS_INLINE init(){
   //   T::init();
   //}

   // must rename this!

   using resources = T::resources;

};


// =============================================================================
//
// read
//
// =============================================================================

// @quote inherit_read 2 = ... ;
template< typename T >
struct inherit_read
{};

template< is_input T >
struct inherit_read< T > {

   using _value_type = typename T::value_type;

   static _value_type GODAFOSS_INLINE read(){
      return T::read();
   }

   static void GODAFOSS_INLINE refresh(){
      T::refresh();
   }

};


// =============================================================================
//
// write
//
// =============================================================================

// @quote inherit_write 2 = ... ;
template< typename T >
struct inherit_write
{};

template< is_output T >
struct inherit_write< T > {

   using _value_type = typename T::value_type;

   static void GODAFOSS_INLINE write( by_const< _value_type > v ){
      T::write( v );
   }

   static void GODAFOSS_INLINE flush(){
      T::flush();
   }

};


// =============================================================================
//
// direction
//
// =============================================================================

// @quote inherit_direction 2 = ... ;
template< typename T >
struct inherit_direction
{};

template< is_simplex T >
struct inherit_direction< T > {

   static void GODAFOSS_INLINE direction_set_input(){
      T::direction_set_input();
   }

   static void GODAFOSS_INLINE direction_set_output(){
      T::direction_set_output;
   }

   static void GODAFOSS_INLINE direction_flush(){
      T::direction_flush();
   }

};
