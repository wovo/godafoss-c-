#include "godafoss.hpp"

namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;
using i2c_bus = gf::i2c_bus_bb_scl_sda<
   target::scl, target::sda, timing >;
using chip    = gf::pcf8574a< i2c_bus >;
using led     = chip::p2;

int main( void ){
   gf::blink< led, timing::ms< 200 >  >();
}  