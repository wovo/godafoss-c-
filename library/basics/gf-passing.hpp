// =============================================================================
//
// gf-passing.hpp
//
// =============================================================================
//
// by_const< T > : best way (copy or reference,
// epending on the sizeof T compared to a reference)
// to pass a const value of type T
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


// default: pass by reference
template< typename T >
struct _by_const { using type = const T &; };

#ifdef __x86_64__
constexpr auto _max_by_value = 2;
#endif

#ifdef __thumb__
constexpr auto _max_by_value = 1;
#endif

#ifdef __AVR_ARCH__
constexpr auto _max_by_value = 1;
#endif

// when trivially copyable,
// and either a fundamental type, or small: pass by value
template< typename T >
   requires (
//      std::is_trivially_copy_constructible< T >::value &&
      (
//         std::is_fundamental_v< T > ||
         ( sizeof( T ) <= _max_by_value )
      ))
struct _by_const< T > { using type = const T; };


// =============================================================================

// interface: use by_const< T > when passing a T
template< typename T >
using by_const = _by_const< T >::type;
