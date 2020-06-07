// ==========================================================================
//
// gf-interval.hpp
//
// ==========================================================================
//
// This file is part of godafoss, 
// a C++ library for close-to-the-hardware programming.
//
// Copyright Wouter van Ooijen 2018
// 
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// ==========================================================================
//
// subrange
//
// ==========================================================================


template< long64_t first, long64_t last >
struct subrange {
   
   using value_type = int_from_max< last - first >;
   value_type value;
   
   subrange(){}
   subrange( value_type value ): value{ value }{}

   assign to
   assign from
   

};      