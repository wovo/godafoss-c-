#include "godafoss.hpp"

using gf     = godafoss;
using target = gf::target;
using timing = gf::timing;

using led    = target::d2;
using period = timing::ms< 500 >; 

int main(){
   gf::blink< led, period >();
}