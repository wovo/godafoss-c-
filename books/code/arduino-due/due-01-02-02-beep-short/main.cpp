#include "godafoss.hpp"

namespace gf   = godafoss;
using target   = gf::target<>;
using timing   = target::timing;

using speaker  = target::d3;
using period   = timing::us< 1120 >; 
using duration = timing::ms< 300 >; 

int main(){
   gf::blink< speaker, period, duration >();
}