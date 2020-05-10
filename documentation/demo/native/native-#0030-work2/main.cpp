#include "godafoss.hpp"
#include <iostream>

namespace gf = godafoss;
using target = gf::target<>;
using timing = target::timing;

using w = target::window< gf::xy( 128, 64 ), 1 >;
using xy = gf::xy;
using line = gf::line< w >;
using rectangle = gf::rectangle< w >;

int main( void ){
   w::init();
   w::clear();
   rectangle( xy( 2,2 ), xy( 10, 10 ), gf::red, gf::blue ).write();
   w::flush(); 
   timing::ms< 2'000 >::wait();
}

