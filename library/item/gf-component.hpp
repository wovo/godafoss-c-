// ==========================================================================
//
// gf-component.hpp
//
// =============================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss),
// a C++ library for close-to-the-hardware programming.
//
// Copyright
//    Wouter van Ooijen 2021
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================


// =============================================================================
//
// @define godafoss::component
// @title component
//
// A component is a building block that provides an interface.
// A component is like an object, but it is 'created' at compile time.
// It is implemented as a C++ struct
// that has only static functions and static attributes.
//
// A component always exists: it is just 'waiting' to be used.
// The features of a component that are not used will be eliminated 
// by the linker, hence the mere presence of an unused component 
// in the source doesn't increase the size of the executable image.
//
// A component, being a type with only static functions and attributes,
// never needs to be instantiated, 
// and in fact a component can't be instatiated.
//
// Instead, each cto provides an init() function.
// Before any of its functions or attributes are used at run-time,
// a cto must be initialized by calling its init() function.
// Failing to do so can cause unpredictable results.
//
// As a cto has only static functions and attributes
// it can be used directly, or the cto can be passed as
// a template parameter.
//
// For cto, and for each more specific cto, a concept exists
// (with the name of the cto), and a root struct from which all
// such cto's are derived (with _root appended to the name of the cto).
//
// The concept checks both for a specific marker, which serves
// no other purpose than to identify the specific cto, and for the
// features that the cto is obliged to offer.
// The concept is used to constrain templates that want to accept
// only a cto that implements a specific set of features.
//
// The root can be a plain struct, but it is often a template.
// For more complex cto's the CRTP pattern is used so the root can
// provide both base properties and enrichment based on the provided
// implementation.
//
// @insert is_cto
//
// A cto has a static init() function that can be called without arguments.
//
// @insert cto_root
//
// The struct cto_root is the root type of all cto's:
// all cto's inherit (in most cases indirectly) from this struct.
//
//
// =============================================================================

// temporary
template< typename T >
concept has_init = requires {
   { T::init() } -> std::same_as< void >;
};

// @quote cto_root 2
struct component_root : not_constructible {
};

// @quote is_cto 4
template< typename T >
concept component = requires {
   std::derived_from< T, component_root >;
} && ! has_init< T >;



