// =============================================================================
//
// gf-shared-names.hpp
//
// =============================================================================
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
// Instantiating the macro GODAFOSS_SUPPORTED( NAME_1, NAME_2 ) creates:
//
// a concept NAME_compatible< T > that is satisfied only when
//    a NAME_supported< T > or < T, Ts > specialization exists that
//    has a static constexpr bool = true;
//
// a concept NAME_compatible_list< T... > that is satisfied when
//    all its parameters T... satisfy NAME_compatible< T >
//
// a base class template NAME_2< typename T > that (when instantiated)
//    creates an error message.
//
// Usage 1:
//
//   GODAFOSS_SOPPORTED( pin_in, pin_in_from )
//      create a specialization of pin_in_compatible
//      now a template can constrain
//
// =============================================================================

#define GODAFOSS_SUPPORTED( NAME_1, NAME_2 )                    \
                                                                \
template< typename T, typename... P >                           \
struct NAME_1 ## _supported { };                                \
                                                                \
template< typename T >                                          \
struct NAME_1 ## _supported< T > { };                           \
                                                                \
template< typename... Ts >                                      \
concept NAME_1 ## _compatible = requires {                      \
   NAME_1 ## _supported< Ts... >::supported;                    \
};                                                              \
                                                                \
template< typename... Ts >                                      \
concept NAME_1 ## _compatible_list =                            \
   ( NAME_1 ## _compatible< Ts > && ... );                      \
                                                                \
template< typename... P >                                       \
   requires NAME_1 ## _compatible< P... >                       \
struct NAME_2 {                                                 \
   static_assert(                                               \
      sizeof...( P ) < 0,                                       \
         "library internal error\n"                             \
         "   " #NAME_2 "<> claims support for this type(s)"     \
         " of parameter(s),\n"                                  \
         "   but no actual implementation is available.\n"      \
         "   (check the 'required from here' line)" );          \
};


// =============================================================================
//
// shared names that other parts of the library support
//
// =============================================================================

GODAFOSS_SUPPORTED( invert, invert )

GODAFOSS_SUPPORTED( all, all )

GODAFOSS_SUPPORTED( direct, direct )
