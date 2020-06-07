// =============================================================================
//
// gf-passing.hpp
//
// =============================================================================
//
// by_const< T > : best way (copy or reference)
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


// when no exact match: pass as a 1 byte smaller data type would be passed
template< int N, typename T >
struct _by_const { using type = _by_const< N - 1, T >::type; };

// when up to the size of a pointer: pass by value
template< typename T >
struct _by_const< 1, T > { using type = const T; };

// when larger than a pointer: pass by reference
template< typename T >
struct _by_const< 1 + sizeof( int * ), T > { using type = const T &; };

// interface: use by_const< T > when passing a T
template< typename T >
using by_const = _by_const< sizeof( T ), T >::type;
