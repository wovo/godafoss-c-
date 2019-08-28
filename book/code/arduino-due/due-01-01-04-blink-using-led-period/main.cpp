#include "godafoss.hpp"

namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;

using led     = target::d2;
using period  = timing::ms< 500 >; 

int main(){
   gf::blink< led, period >();
}