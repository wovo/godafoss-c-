#include <iostream>
#include "godafoss.hpp"

namespace gf = godafoss;
using target = gf::target<>;
using timing = target::timing;

using w = target::window< { 128, 64 }, 1 >;

int main( void ){
   gf::demo::bouncing_square< w, timing::ms< 50 >, 10 > ();
}

