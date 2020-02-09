// ==========================================================================
//
// empty project
//
// (c) Wouter van Ooijen (wouter@voti.nl) 2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt) 
//
// ==========================================================================

#include "godafoss.hpp"

template< typename T >
struct absolute : T {
    
   typename( P... )    
   absolute( P... ): T( P... ) {}
}

template< typename T >
struct relative : T {
   // block     
    
    
}



int main( void ){}