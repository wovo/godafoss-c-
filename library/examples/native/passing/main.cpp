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

// @quote text
#include "array"
#include "godafoss.hpp"
namespace gf  = godafoss;

template< typename T >
void function( gf::by_const< T > p ){ (void) p; }

int main(){

   // should probably be passed by value (copy)
   function( 'x' );

   // should be probably be passed by reference
   function( std::array< int, 100 >{ 0 } );

};
// @quote end
