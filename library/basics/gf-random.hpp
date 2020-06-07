// =============================================================================
//
// gf-random.hpp
//
// =============================================================================
//
// This is simple 32-bit LCG random function, for demos and games.
// The random facilities of the standard library are not used because
// they eat up too much RAM.
//
// *** Do NOT use this for crypto work. ***
//
// The LCG used is the Microsoft Visual/Quick C/C++ variant as explained
// on https://en.wikipedia.org/wiki/Linear_congruential_generator, but
// using bits 8..23 rather than 16..30.
//
// =============================================================================
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
// =============================================================================

uint32_t _random_seed = 0;

void random_seed( by_const< uint32_t > x ){
   _random_seed = x;
}

uint32_t random16(){
   _random_seed = _random_seed * 214013 + 2531011;
    return ( _random_seed >> 8 ) & 0xFFFF;
}

uint32_t random32(){
    auto a = random16();
    auto b = random16();
    return ( a << 16 ) | b;
}

template< typename t >
t random_in_range( by_const< t >first, by_const< t > last ){
   static_assert( sizeof( t ) <= 32 );
   return first + ( random32() % ( 1 + last - first  ) );
}
