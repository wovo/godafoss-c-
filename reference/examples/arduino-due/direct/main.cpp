// =============================================================================
//
// pins example
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
#include "godafoss.hpp"
namespace gf      = godafoss;
using target      = gf::target<>;
using pin         = gf::pin_out_from< target::d13 >;
using direct_pin  = gf::direct< pin >;

int main(){

   pin::init();

   // write, followed by an explicit flush
   pin::write( 1 );
   pin::flush();

   // write to a direct<> pin implies an implicit flush(),
   // so no explicit fluish() is needed
   direct_pin::write( 1 );

};
// @quote end
