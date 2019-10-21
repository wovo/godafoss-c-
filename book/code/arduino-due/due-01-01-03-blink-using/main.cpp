#include "godafoss.hpp"

namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;

int main(){
   gf::blink< 
      target::d2, 
      timing::ms< 500 > 
   >();
}