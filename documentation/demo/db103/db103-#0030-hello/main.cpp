// ==========================================================================
//
// Test uart cout
//
// (c) Wouter van Ooijen (wouter@voti.nl) 2017-2020
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
using uart    = target::uart<>;

int main(){
   timing::init();
   gf::ostream< gf::formatter< uart > > cout;
   timing::ms< 2'000>::wait();
   
   cout << "Hello world\n";
}   

