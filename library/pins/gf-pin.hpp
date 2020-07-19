// =============================================================================
//
// gf-pin.hpp
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
//
// @title pins
//
// A pin is a box< bool > cto that is used to asbstract a
// GPIO (general-purpose input-output interface) pin on a
// micro-controller (or peripheral chip),
// or the more limited input-only, output-only,
// or open-collector version.
//
// When a pin represents a physical pin, 0 (or false) means a low voltage
// level (almost ground), and 1 (or true) means a high voltage level.
//
// When a pin represents a functionality, for instance 'enable',
// true (or 1) means that the function is enabled, and
// false (or 0) means that the function is not enabled (disabled).
//
// When the phyiscal pin is active-low, an invert<> decrorator
// is used to create the internal active-high representation of the pin.
//
// -----------------------------------------------------------------------------
//
// @define is_pin_in
// @section pin_in
// @insert is_pin_in
//
// A pin_in is a box< bool > cto that abstracts a single-pin read-only
// interface to the world outside the target chip.
// A typical use of a pin_in is to read a switch or pushbutton.
//
// @insert pin_in_root
//
// All pin_in cto's inherit from pin_in_root.
//
// -----------------------------------------------------------------------------
//
// @define pin_out
// @section pin_out
// @insert is_pin_out
//
// A pin_in is a box< bool > cto that abstracts a single-pin write-only
// interface to the world outside the target chip.
// A typical use of a pin_in is to drive an LED.
//
// @insert pin_out_root
//
// All pin_out cto's inherit from pin_out_root.
//
// -----------------------------------------------------------------------------
//
// @define is_pin_in_out
// @section pin_in_out
// @insert is_pin_in_out
//
// A pin_in_out is a box< bool > cto that abstracts a single-pin simplex
// read-write interface to the world outside the target chip.
// A pin_in_out is the most versatile of the pin types,
// because it can be used in any of the roles.
// In most cases a pin_in_out is used as either pin, a a pin_out,
// or a pin_oc, but some communication protocols
// require a pin to be switched between input and output.
//
// @insert pin_in_out_root
//
// All pin_in cto's inherit from pin_in_out_root.
//
//
// -----------------------------------------------------------------------------
//
// @define is_pin_oc
// @section pin_oc
// @insert is_pin_oc
//
// A pin_oc is a box< bool > cto that abstracts a single-pin duplex
// read-write interface to the world outside the target chip.
// The term oc means open-collector, refrerring to the
// (now somewhat outdated) way this type of pin can be implemented:
// the output stage has a transistor that can pull the pin low,
// but unlike a normal output pin it has no transistor to pull
// the pin high.
//
// Open-collector pins are used in various protocols like i2c and one-wire,
// where open-collector pins of more than one chip are connected
// to the same wire.
// Any chip can pull the write low. When no chip does so, a common
// pull-up resistor pulls the line low.
// This arrangement prevents electrical problems which would be caused
// when one chip drives the line low, and another drives it high.
//
// @insert pin_oc_root
//
// All pin_in cto's inherit from pin_oc_root.
//
//
// =============================================================================


// =============================================================================
//
// pin_in
//
// =============================================================================

// @quote is_pin_in 7
template< typename T >
concept is_pin_in = requires {
      T::_pin_in_marker;
   }
   && is_box< T, bool >
   && is_input< T >;

// @quote pin_in_root 6
struct pin_in_root :
   box_root< bool >,
   input_root< bool >
{
   static constexpr bool _pin_in_marker = true;
};


// =============================================================================
//
// pin_out
//
// =============================================================================

// @quote is_pin_out 6
template< typename T >
concept is_pin_out = requires {
      T::_pin_out_marker;
   }
   && is_box< T, bool >
   && is_output< T >;

// @quote pin_out_root 6
struct pin_out_root :
   box_root< bool >,
   output_root< bool >
{
   static constexpr bool _pin_out_marker = true;
};


// =============================================================================
//
// pin_in_out
//
// =============================================================================

// @quote is_pin_in_out 7
template< typename T >
concept is_pin_in_out =
 requires {
      T::_pin_in_out_marker;
   }
   && is_box< T, bool >
   && is_simplex< T >;

// @quote pin_in_out_root 6
struct pin_in_out_root :
   box_root< bool >,
   simplex_root< bool >
{
   static constexpr bool _pin_in_out_marker = true;
};


// =============================================================================
//
// pin_oc
//
// =============================================================================

// @quote is_pin_oc 6
template< typename T >
concept is_pin_oc = requires {
      T::_pin_oc_marker;
   }
   && is_box< T, bool >
   && is_duplex< T >;

// @quote pin_oc_root 6
struct pin_oc_root :
   box_root< bool >,
   duplex_root< bool >
{
   static constexpr bool _pin_oc_marker = true;
};
