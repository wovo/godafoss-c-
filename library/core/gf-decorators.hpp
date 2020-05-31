// ==========================================================================
//
// gf-decorators.hpp
//
// ==========================================================================
//
// This file defines the decorators can be supported by opting in:
//    - invert   : invert the value that is read or written
//    - all      : write (the same value) to all
//    - direct   : refresh before each read, flush after each write
//    - buffered
//    - combine
//
// To support a decorator, for instance invert, for a type T, define
//    - a specialization support_invert< T > that has a
//         static constexpr bool value = true;
//    - a specialization invert< T > that provides the functionality
//
// ==========================================================================
//
// This file is part of godafoss,
// a C++ library for close-to-the-hardware programming.
//
// Copyright Wouter van Ooijen 2018-2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================


// ==========================================================================
//
// invert
//
// invert the value that is read or written
//
// ==========================================================================

GODAFOSS_SUPPORTED( invert, invert )

/*
template< typename... T >
struct support_invert {};

template< typename... T >
concept can_invert = requires {
   support_invert< T... >::value;
};

template< typename... P >
   requires can_invert< P... >
struct invert{
   static_assert(
      sizeof...( P ) < 0,
      "An invert<> specialization for these parameters "
      "should exits but seems to be missing.\n"
      "(check the 'required from here' line)" );
};
*/

// ==========================================================================
//
// all
//
// write (the same value) to all
//
// ==========================================================================

GODAFOSS_SUPPORTED( all, all )

/*
template< typename... T >
struct support_all {};

template< typename... T >
concept can_all = requires {
   support_all< T... >::value;
};

template< typename... P >
   requires can_all< P... >
struct all{
   static_assert(
      sizeof...( P ) < 0,
      "An all<> specialization for these parameters "
      "should exits but seems to be missing.\n"
      "(check the 'required from here' line)" );
};
*/
/*
// opt into all<>
template<>
struct support_all< void > {
   static constexpr bool value = true;
};
*/

// ==========================================================================
//
// direct
//
// refresh before each read, flush after each write
//
// ==========================================================================

GODAFOSS_SUPPORTED( direct, direct )

/*
template< typename... T >
struct support_direct {};

template< typename... T >
concept can_direct = requires {
   support_direct< T... >::value;
};

template< typename... P >
   requires can_direct< P... >
struct direct{
   static_assert(
      sizeof...( P ) < 0,
      "An direct<> specialization for these parameters "
      "should exits but seems to be missing.\n"
      "(check the 'required from here' line)" );
};
*/
