#include "godafoss.hpp"

using gf     = godafoss;
using target = gf::target;
using timing = gf::timing;

int main(){
   gf::blink< 
      target<>::d2, 
      timing::ms< 500 > 
   >();
}