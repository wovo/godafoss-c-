// ==========================================================================
//
// gf-modifiers-ports.hpp
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
// @title port adapters
//
// These adapters adapt a port to be (only) an input port,
// (only) an output port,
// (only) an input_output port, or (only) an open collector port.
// (in each case, if such adaptation is possible).
//
// The created pin has only the properties required for that pin:
// other properties of the source pin are not available via the created pin.
// The exception is pullup and pulldown features: those are
// available via the created pins.
//
// These adapters serve, of course, to adapt a given port to the
// adapted role, but also to ensure that the code that uses the adapted
// port doesn't use any features beyond the ones of the adapted role.
//
// -----------------------------------------------------------------------------
//
// @define godafoss::port_in
// @section port_in
//
// @insert can_port_in
// @insert port_in
//
// The port_in<> adapter creates an input port from a source port,
// which is possible if the source port satisfies the can_port_in concept,
// which requires it to be either a port_in, a port_in_out,
// or a port_oc.
//
// -----------------------------------------------------------------------------
//
// @define godafoss::port_out
// @section port_out
//
// @insert can_port_out
// @insert port_out
//
// The port_out<> adapter creates an output port from a source port,
// which is possible if the source port satisfies the can_port_out concept,
// which requires it to be either a port_in, a port_in_out,
// or a port_oc.
//
// Note that when a port_oc is adapted to be used as port_out,
// pull-up resistors are required in order for the pins to
// reach a high level.
//
// -----------------------------------------------------------------------------
//
// @define godafoss::port_in_out
// @section port_in_out
//
// @insert can_port_in_out
// @insert port_in_out
//
// The port_in_out<> adapter creates an input_output port from a source port,
// which is possible if the source port satisfies the can_port_in_out concept,
// which requires it to a port_in_out, or a port_oc.
//
// Note that when a port_oc is adapted to be used as port_in_out,
// pull-up resistors are required in order for the pins to
// reach a high level.
//
// -----------------------------------------------------------------------------
//
// @define godafoss::port_oc
// @section port_oc
//
// @insert can_port_oc
// @insert port_oc
//
// The port_oc<> adapter creates an open collector port from a source port,
// which is possible if the source port satisfies the can_port_oc concept,
// which requires it to a port_oc.
//
// It is not possible to create a port_oc from an input-output port,
// because that would require control over the direction of the individual
// pins. An input-output provides (only) control over the direection of
// all pins at once.
//
// =============================================================================

// =============================================================================
//
// port_in
//
// =============================================================================

// @quote can_port_in 5
template< typename T >
concept can_port_in =
      is_port_in< T >
   || is_port_in_out< T >
   || is_port_oc< T >;

// @quote port_in 1 = ...;
template< can_port_in T >
struct port_in {
   // The specializations should cover all possibilities.
   static_assert( sizeof( T ) < 0, "library internal error\n" );
};

template< is_port_in T  >
struct port_in< T > : port_in_from_port_in< T > {};

template< is_port_in_out T  >
struct port_in< T > : port_in_from_port_in_out< T > {};

template< is_port_oc T  >
struct port_in< T > : port_in_from_port_oc< T > {};


// =============================================================================
//
// port_out
//
// =============================================================================

// @quote can_port_out 5
template< typename T >
concept can_port_out =
      is_port_out< T >
   || is_port_in_out< T >
   || is_port_oc< T >;

// @quote port_out 1 = ...;
template< can_port_out T >
struct port_out {
   // The specializations should cover all possibilities.
   static_assert( sizeof( T ) < 0, "library internal error\n" );
};

template< is_port_out T  >
struct port_out< T > : port_out_from_port_out< T > {};

template< is_port_in_out T  >
struct port_out< T > : port_out_from_port_in_out< T > {};

template< is_port_oc T  >
struct port_out< T > : port_out_from_port_oc< T > {};


// =============================================================================
//
// port_in_out
//
// =============================================================================

// @quote can_port_in_out 4
template< typename T >
concept can_port_in_out =
      is_port_in_out< T >
   || is_port_oc< T >;

// @quote port_in_out 1 = ...;
template< can_port_in_out T  >
struct port_in_out {
   // The specializations should cover all possibilities.
   static_assert( sizeof( T ) < 0, "library internal error\n" );
};

template< is_port_in_out T  >
struct port_in_out< T > : port_in_out_from_port_in_out< T > {};

template< is_port_oc T  >
struct port_in_out< T > : port_in_out_from_port_oc< T > {};


// =============================================================================
//
// port_oc
//
// =============================================================================

// @quote can_port_oc 3
template< typename T >
concept can_port_oc =
   is_port_oc< T >;

// @quote port_oc 1 = ...;
template< is_port_oc T  >
struct port_oc : port_oc_from_port_oc< T > {};
