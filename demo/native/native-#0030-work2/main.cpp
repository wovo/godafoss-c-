#include "godafoss.hpp"
#include <iostream>

namespace gf = godafoss;
using target = gf::target<>;
using timing = target::timing;

using w = target::window< { 128, 64 }, 1 >;
using line = gf::line< w >;
using rectangle = gf::rectangle< w >;

int main( void ){
   /*
   w::init();
   w::clear( gf::green );
   w::flush();
std::cout << __LINE__ << "\n";timing::ms< 2'000 >::wait();
// => paints everything white??
   rectangle(
      w::location_t( 2, 2 ),
      w::location_t( 10, 10 ),
      gf::red, gf::blue ).write();
   w::flush();
std::cout << __LINE__ << "\n";timing::ms< 2'000 >::wait();
   timing::ms< 2'000 >::wait();
   * */
}

