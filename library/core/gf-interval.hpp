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
// interval
//
// ==========================================================================


template< typename T >
struct interval {
   
   T low;
   T high;

   interval( const & low, cont & high ):
      low( low ), high( high )
      {}

};      