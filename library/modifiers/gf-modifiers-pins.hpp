// ==========================================================================
//
// gf-modifiers-pins.hpp
//
// ==========================================================================
//
// Part of   : C++ hwlib library for close-to-the-hardware OO programming
// Copyright : wouter@voti.nl 2017-2019
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================


// =============================================================================
//
// pin_in
//
// =============================================================================

template< typename T >
concept can_pin_in =
      is_pin_in< T >
   || is_pin_in_out< T >
   || is_pin_oc< T >;

template< can_pin_in T >
struct pin_in {};

template< is_pin_in T  >
struct pin_in< T > : pin_in_from_is_pin_in< T > {};

template< is_pin_in_out T  >
struct pin_in< T > : pin_in_from_is_pin_in_out< T > {};

template< is_pin_oc T  >
struct pin_in< T > : pin_in_from_is_pin_oc< T > {};


// =============================================================================
//
// pin_out
//
// =============================================================================

template< typename T >
concept can_pin_out =
      is_pin_out< T >
   || is_pin_in_out< T >;

template< can_pin_out T >
struct pin_out {};

template< is_pin_out T  >
struct pin_out< T > : pin_out_from_is_pin_out< T > {};

template< is_pin_in_out T  >
struct pin_out< T > : pin_out_from_is_pin_in_out< T > {};


// =============================================================================
//
// pin_in_out
//
// =============================================================================

template< typename T >
concept can_pin_in_out =
      is_pin_in_out< T >;

template< is_pin_in_out T  >
struct pin_in_out : pin_in_out_from_is_pin_in_out< T > {};


// =============================================================================
//
// pin_oc
//
// =============================================================================

template< typename T >
concept can_pin_oc =
      is_pin_in_out< T >
   || is_pin_oc< T >;

template< can_pin_oc T >
struct pin_oc {};

template< is_pin_in_out T  >
struct pin_oc< T > : pin_oc_from_is_pin_in_out< T > {};

template< is_pin_oc T  >
struct pin_oc< T > : pin_oc_from_is_pin_oc< T > {};



