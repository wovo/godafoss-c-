#include "godafoss.hpp"
int main(){
   godafoss::blink< 
      godafoss::target<>::led, 
      godafoss::target<>::timing::ms< 500 > 
   >();
}
