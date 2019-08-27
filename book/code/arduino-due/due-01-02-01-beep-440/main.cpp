#include "godafoss.hpp"

using gf     = godafoss;
using target = gf::target;
using timing = gf::timing;

using led    = target::d3;
using period = timing::us< 1120 >; 

int main(){
   gf::blink< led, period >();
}