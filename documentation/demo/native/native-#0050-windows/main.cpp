#include <ostream>
#include "godafoss.hpp"

namespace gf = godafoss;
using target = gf::target<>;
using timing = target::timing;
using w1 = target::window< { 128, 64 }, 1 >;
using w2 = target::window< { 128, 64 }, 2 >;
using w3 = target::window< { 50, 50 } >;
using w4 = target::window< { 80, 80 } >;
// gf::invert< w1 >,
// using w = gf::all< gf::flip_horizontally< w2 >, w3, w4 >;

using w = gf::all< w1, w4 >;

int main( void ){
   gf::demo::random_lines< w, timing::ms< 200 > > ();
}

