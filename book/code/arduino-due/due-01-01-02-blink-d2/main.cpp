#include "godafoss.hpp"

int main(){
   godafoss::blink< 
      godafoss::target<>::d2, 
      godafoss::target<>::timing::ms< 500 > 
   >();
}