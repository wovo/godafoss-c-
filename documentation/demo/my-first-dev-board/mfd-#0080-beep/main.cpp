// ==========================================================================
//
// (c) Wouter van Ooijen (wouter@voti.nl) 2019
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt) 
//
// ==========================================================================

#include "godafoss.hpp"

namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;

using lsp = target::buzzer;

int main(){
   lsp::init();
   timing::init();
   for( int i = 0; i < 500; ++i ){
      lsp::write( 1 );
      timing::us< 500 >::wait();
      lsp::write( 0 );
      timing::us< 500 >::wait();
   }
}   