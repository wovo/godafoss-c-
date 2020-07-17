// =============================================================================
//
// parameter passing example
//
// =============================================================================
//
// Copyright
//    Wouter van Ooijen (wouter@voti.nl) 2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================

// @quote example
#include "array"
#include "godafoss.hpp"
namespace gf  = godafoss;

void GODAFOSS_NO_INLINE f1(
   gf::by_const< char > p
){ (void) p; }

void GODAFOSS_NO_INLINE f2(
   gf::by_const< std::array< int, 100 > > p
){ (void) p; }

int main(){

   // should probably be passed by value (copy)
   f1( 'x' );

   // should be probably be passed by reference
   f2( std::array< int, 100 >{ 0 } );

};
// @quote end
