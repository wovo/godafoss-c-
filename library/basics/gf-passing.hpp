// =============================================================================
//
// gf-passing.hpp
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
//
// @title passing a readonly parameter
//
// @insert by_const
// @define godafoss::by_const
//
// The by_const< type > template is the preferred way to pass a
// const value of the type passed_type.
// This will be either a plain (by copy) const, or a const reference,
// depending (among other things) on the size of the type compared
// to a the size of a reference.
//
// @example native/passing/main.cpp example
//
// =============================================================================


// default: pass by reference
template< typename T >
struct _by_const { using type = const T &; };

#ifdef __x86_64__
constexpr auto _max_by_value = 2;
#endif

#ifdef __i386__
constexpr auto _max_by_value = 1;
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

// @quote by_const 3 ...
// use by_const< T > when passing a T
template< typename T >
using by_const =
   typename _by_const< T >::type;
