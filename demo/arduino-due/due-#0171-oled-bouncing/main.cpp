#include "godafoss.hpp"

namespace gf = godafoss;
using target = gf::target<>;
using timing = target::timing;

using i2c_bus_sw = gf::i2c_bus_bb_scl_sda<
   target::scl1, target::sda1, timing, gf::i2c_fast >;
using i2c_bus_hw = target::i2c0< gf::i2c_fast >;

using oled = gf::oled< i2c_bus_sw >;

int main( void ){
   gf::demo::bouncing_square< oled, timing::ms< 0 >, 8 > ();
}


