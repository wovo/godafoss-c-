// ==========================================================================
//
// gf-passing.hpp
//
// ==========================================================================
//
// by_const< T > : best way (copy or reference) to pass a const value
//
// ==========================================================================
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
// ==========================================================================

/*
template< typename T >
using by_const = by_const_determined_by_size< sizeof( T ) >;

// when no exact match: pass as a 1 byte smaller data type would be passed
template< size_t N >
using by_const_determined_by_size = by_const_determined_by_size< N - 1 >;

// when larger than a pointer: pass by reference
template<>
using by_const_determined_by_size< 1 + sizeof( int * ) > = const & T;

// when same size as a pointer or smaller: pass by value
template<>
using by_const_determined_by_size< 0 > = const T;
*/




   
   