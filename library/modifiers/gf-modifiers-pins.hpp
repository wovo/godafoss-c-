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
// @title pin adapters
//
// These adapters adapt a pin to be (only) an input pin,
// (only) an output pin,
// (only) an input_output pin, or (obly) an open collector pin.
// (in each case, if such adaptation is possible).
//
//
//
// These adapters serve, of course, to adapt a given pin to the
// adapted role, but also to ensure that the code that uses the adapted
// pin doesn't use any features beyond the ones of the adapted role.
//
// -----------------------------------------------------------------------------
//
// @define godafoss::pin_in
// @section pin_in
//
// @insert can_pin_in
// @insert pin_in
//
// The pin_in<> decorator decorates a pin to be an input pin,
// which is possible if the pin satisfies the can_input concept,
// which requires the pin to be either a pin_in or a pin_in_out.
//
// -----------------------------------------------------------------------------
//
// @define godafoss::pin_out
// @section pin_out
//
// @insert can_pin_out
// @insert pin_out
//
// The pin_out<> decorator decorates a pin to be an output pin,
// which is possible if the pin satisfies the can_output concept,
// which requires the pin to be either a pin_in, a pin_in_out,
// or a pin_oc.
//
// Note that when a pin_oc is adapted to be used as pin_out,
// a pull-up resistor is required in order for the pin to
// reach a high level.
//
// -----------------------------------------------------------------------------
//
// @define godafoss::pin_in_out
// @section pin_in_out
//
// @insert can_pin_in_out
// @insert pin_in_out
//
// The pin_in_out<> decorator decorates
// a pin to be an input_output pin,
// which is possible if the pin satisfies the can_input_output concept,
// which requires the pin to a pin_in_out, or a pin_oc.
//
// Note that when a pin_oc is adapted to be used as pin_in_out,
// a pull-up resistor is required in order for the pin to
// reach a high level.
//
// -----------------------------------------------------------------------------
//
// @define godafoss::pin_oc
// @section pin_oc
//
// @insert can_pin_oc
// @insert pin_oc
//
// The pin_oc<> decorator decorates
// a pin to be an open collector pin,
// which is possible if the pin satisfies the can_input_output concept,
// which requires the pin to a pin_in_out or a pin_oc.
//
// =============================================================================



// =============================================================================
//
// pin_in
//
// =============================================================================

// @quote can_pin_in 5
template< typename T >
concept can_pin_in =
      is_pin_in< T >
   || is_pin_in_out< T >
   || is_pin_oc< T >;

// @quote pin_in 1 = ...;
template< can_pin_in T >
struct pin_in {
   // The specializations should cover all possibilities.
   static_assert( sizeof( T ) < 0, "library internal error\n" );
};

template< is_pin_in T  >
struct pin_in< T > : pin_in_from_pin_in< T > {};

template< is_pin_in_out T  >
struct pin_in< T > : pin_in_from_pin_in_out< T > {};

template< is_pin_oc T  >
struct pin_in< T > : pin_in_from_pin_oc< T > {};


// =============================================================================
//
// pin_out
//
// =============================================================================

// @quote can_pin_out 5
template< typename T >
concept can_pin_out =
      is_pin_out< T >
   || is_pin_in_out< T >
   || is_pin_oc< T >;

// @quote pin_out 1 = ...;
template< can_pin_out T >
struct pin_out {
   // The specializations should cover all possibilities.
   static_assert( sizeof( T ) < 0, "library internal error\n" );
};

template< is_pin_out T  >
struct pin_out< T > : pin_out_from_pin_out< T > {};

template< is_pin_in_out T  >
struct pin_out< T > : pin_out_from_pin_in_out< T > {};

template< is_pin_oc T  >
struct pin_out< T > : pin_out_from_pin_oc< T > {};


// =============================================================================
//
// pin_in_out
//
// =============================================================================

// @quote can_pin_in_out 4
template< typename T >
concept can_pin_in_out =
      is_pin_in_out< T >
   || is_pin_oc< T >;

// @quote pin_in_out 1 = ...;
template< can_pin_in_out T  >
struct pin_in_out {
   // The specializations should cover all possibilities.
   static_assert( sizeof( T ) < 0, "library internal error\n" );
};

template< is_pin_in_out T  >
struct pin_in_out< T > : pin_in_out_from_pin_in_out< T > {};

template< is_pin_oc T  >
struct pin_in_out< T > : pin_in_out_from_pin_oc< T > {};


// =============================================================================
//
// pin_oc
//
// =============================================================================

// @quote can_pin_oc 4
template< typename T >
concept can_pin_oc =
      is_pin_in_out< T >
   || is_pin_oc< T >;

// @quote pin_oc 1 = ...;
template< can_pin_oc T >
struct pin_oc {
   // The specializations should cover all possibilities.
   static_assert( sizeof( T ) < 0, "library internal error\n" );
};

template< is_pin_in_out T  >
struct pin_oc< T > : pin_oc_from_pin_in_out< T > {};

template< is_pin_oc T  >
struct pin_oc< T > : pin_oc_from_pin_oc< T > {};



