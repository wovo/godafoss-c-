// =============================================================================
//
// gf-loop.hpp
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

template< uint64_t N, bool unroll = false, typename body_t >
void loop( body_t body ){
   for( uint_fast8_t i = 0; i < N; ++i ){
      body();
   }
}
