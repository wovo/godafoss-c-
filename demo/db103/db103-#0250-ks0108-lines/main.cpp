#include "godafoss.hpp"

namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;
using uart    = target::uart<>;

using data = gf::port_oc_from_pins<
   target::scl,
   target::sda,
   target::p0_6,
   target::p0_7,
   target::p1_0,
   target::p1_1,
   target::p1_2,
   target::p1_3 >;
using cs1  = target::p1_4;
using cs2  = target::p1_5;
using cd   = target::p1_8;
using e    = target::p1_9;
using lcd  = gf::ks0108_data_cs1_cs2_cd_e_timing<
   data, cs1, cs2, cd, e, timing >;

int main( void ){
   gf::demo::random_lines2< lcd, timing::ms< 200 > > ();
   timing::init();
   gf::ostream< gf::formatter< uart > > cout;
   lcd::init();
   lcd::clear( gf::white );
   //gf::line< lcd >( lcd::location_t{ 5,5 }, lcd::location_t{ 10, 10 }, gf::black_or_white( 1 ) );
   int n = 0;
   for(;;){
	   lcd::flush();
	   timing::ms< 200 >::wait();
	   cout << ++n << "\n";
   }
}
