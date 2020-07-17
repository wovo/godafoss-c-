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
// These adapters adapt an item to be (only) an input item,
// (only) an output item,
// or (only) an input_output item
// (in each case, if such adaption is possible).
//
// These adapters serve, of course, to adapt a given item to the
// adapted role, but also to ensure that the code that uses the adapted
// item, doesn't use any features beyond the ones of the adapted role.
//
// -----------------------------------------------------------------------------
//
// @define godafoss::item_input
// @section item_input
//
// @insert can_input
// @insert item_input
//
// The item_input<> decorator decorates an item to be an input item,
// which is possible if the item satisfies the can_input concept,
// which requires the item to be either an input or an input_output.
//
// -----------------------------------------------------------------------------
//
// @define godafoss::item_output
// @section item_output
//
// @insert can_output
// @insert item_output
//
// The item_output<> decorator decorates an item to be an output item,
// which is possible if the item satisfies the can_output concept,
// which requires the item to be either an input or an input_output.
//
// -----------------------------------------------------------------------------
//
// @define godafoss::item_input_output
// @section item_input_output
//
// @insert can_input_output
// @insert item_input_output
//
// The item_input_output<> decorator decorates
// an item to be an input_output item,
// which is possible if the item satisfies the can_input_output concept,
// which requires the item to an input_output.
//
// =============================================================================


// =============================================================================
//
// adapt to be an input item
//
// =============================================================================

// @quote can_input 4
template< typename T >
concept can_input =
   is_input< T >
   || is_input_output< T >;

// @quote item_input 2 ... ;
template< can_input T >
struct item_input
{};

template< is_input T >
struct item_input< T > :
   inherit_init< T >,
   inherit_read< T >
{};

template< is_input_output T >
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

// @quote can_output 4
template< typename T >
concept can_output =
   is_output< T >
   || is_input_output< T >;

// @quote item_output 2 ... ;
template< can_output T >
struct item_output
{};

template< is_output T >
struct item_output< T > :
   inherit_init< T >,
   inherit_write< T >
{};

template< is_input_output T >
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

// @quote can_input_output 3
template< typename T >
concept can_input_output =
   is_input_output< T >;

// @quote item_input_output 2 ... ;
template< can_input_output T >
struct item_input_output
:
   inherit_init< T >,
   inherit_read< T >,
   inherit_write< T >,
   inherit_direction< T >
{};
