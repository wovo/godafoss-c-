// ==========================================================================
//
// Hello world on an Arduino Uno
//
// (c) Wouter van Ooijen (wouter@voti.nl) 2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

#include "godafoss.hpp"

namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;

using back_light  = gf::direct< target::d10 >;
using lcd = gf::hd44780_rs_e_d_s_timing<
    target::d8,
    target::d9,
    gf::port_out_from_pins<
       target::d4,
       target::d5,
       target::d6,
       target::d7 >,
    { 16, 2 },
    timing >;

int main( void ){

    back_light::write( 1 );

//    gf::scrolling_text< lcd >(
//       "We regret to inform you that we are closed for the day."
//    );

}
