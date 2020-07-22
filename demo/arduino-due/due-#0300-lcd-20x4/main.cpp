// ==========================================================================
//
// blink the LED on an Arduino Due
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
    { 20, 4 },
    timing >;

int main(){

    gf::ostream< gf::formatter< lcd > > cout;

    back_light::write( 0 );
    timing::ms< 500 >::wait();
    back_light::write( 1 );

    cout
       << "\fHello world!        | this is only for a"
       << "\nnice to meet you    | 40 col. display."
       << "\t0002line 3 ============X"
       << "\nline 4 ============X"
       ;; //<< gf::flush;
}
