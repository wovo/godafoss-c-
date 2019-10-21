#include "godafoss.hpp"

namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;
using led     = target::pa0;

int main( void ){
   gf::blink< led, timing::ms< 200 >  >();
}  