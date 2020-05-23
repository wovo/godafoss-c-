// ==========================================================================
//
// gf-random.hpp
//
// ==========================================================================
//
// simple random function
//
// ==========================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss), 
// a C++ library for close-to-the-hardware programming.
//
// Copyright 
//    Wouter van Ooijen 2018-2020
// 
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================


template< typename t >
t random_in_range( t first, t last ){
   static std::default_random_engine generator;
   std::uniform_int_distribution< t > distribution( first, last );
   return distribution( generator );   
}   