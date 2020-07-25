#include "godafoss.hpp"

namespace gf = godafoss;
using target = gf::target<>;
using timing = target::timing;
using i2c_bus = gf::i2c_bus_bb_scl_sda<
   target::scl1,
   target::sda1,
   timing,
   gf::i2c_standard >;
using oled = gf::oled< i2c_bus >;

int main( void ){
   gf::demo::bouncing_square< oled, timing::ms< 0 >, 8 > ();
}


