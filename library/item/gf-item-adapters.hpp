// =============================================================================
//
// gf-item-adapters.hpp
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
// @title item adapters
//
// These adapter adapts a item to be (only) an input item,
// (only) an output item,
// or an input_output item (if such adaption is possible).
//
// @define godafoss::item_input
// @section item_input
//
// @insert can_input
// @insert item_input
//
// The item_input<> decorator decorates an item to be an input item,
// which is possible if the item satisfies the can_input concept.
//
// @insert can_output
// @insert item_output
//
// The item_output<> decorator decorates an item to be an output item,
// which is possible if the item satisfies the can_output concept.
//
// @insert can_input_output
// @insert item_input_output
//
// The item_input_output<> decorator decorates
// an item to be an input_output item,
// which is possible if the item satisfies the can_input_output concept.
//
// =============================================================================


// =============================================================================
//
// concepts
//
// =============================================================================

// @quote can_input 4
template< typename T >
concept can_input =
      input< T >
   || input_output< T >;

// @quote can_output 4
template< typename T >
concept can_output =
      output< T >
   || input_output< T >;

// @quote can_input_output 4
template< typename T >
concept can_input_output =
     input_output< T >;


// =============================================================================
//
// adapt to be an input item
//
// =============================================================================

// @quote item_input 2 ... ;
template< can_input T >
struct item_input
{};

template< input T >
struct item_input< T > :
   inherit_init< T >,
   inherit_read< T >
{};

template< input_output T >
struct item_input< T > :
   inherit_read< T >
{

   static void GODAFOSS_INLINE init(){
      T::init();
      direct< T >::direction_set_input();
   }

};


// =============================================================================
//
// adapt to an output item
//
// =============================================================================

// @quote item_output 2 ... ;
template< can_output T >
struct item_output
{};

template< output T >
struct item_output< T > :
   inherit_init< T >,
   inherit_write< T >
{};

template< input_output T >
struct item_output< T > :
   inherit_write< T >
{

   static void init(){
      T::init();
      direct< T >::direction_set_output();
   }

};


// =============================================================================
//
// adapt to be an input_output item
//
// =============================================================================

// @quote item_input_output 2 ... ;
template< input_output T >
struct item_input_output
:
   inherit_init< T >,
   inherit_read< T >,
   inherit_write< T >,
   inherit_direction< T >
{};
