#include "godafoss.hpp"

using gf     = godafoss;
using target = gf::target;
using timing = gf::timing;

using led    = target::d3;
using period = timing::us< 1120 >; 

using morse_short = timing::ms< 200 >; 
using morse_long  = timing::us< 600 >; 

int main(){
   gf::blink< led, period, duration, morse_short >();
   morse_short::wait();
   gf::blink< led, period, duration, morse_short >();
   morse_short::wait();
   gf::blink< led, period, duration, morse_short >();
   morse_short::wait();
   
   gf::blink< led, period, duration, morse_long >();
   morse_short::wait();
   gf::blink< led, period, duration, morse_long >();
   morse_short::wait();
   gf::blink< led, period, duration, morse_long >();
   morse_short::wait();
   
   gf::blink< led, period, duration, morse_short >();
   morse_short::wait();
   gf::blink< led, period, duration, morse_short >();
   morse_short::wait();
   gf::blink< led, period, duration, morse_short >();
   morse_short::wait();
}