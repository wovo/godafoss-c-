#include "godafoss.hpp"

namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;

using i2c_bus = gf::i2c_bus_bb_scl_sda<
   target::scl,
   target::sda,
   timing >;
using oled = gf::oled< i2c_bus >;

int main( void ){
//   for(;;){ oled::init(); }
//   oled::clear( gf::black );
//   for(;;){ oled::flush(); timing::ms< 5 >::wait(); }
   gf::demo::random_lines2< oled, timing::ms< 2 > > ();
}
