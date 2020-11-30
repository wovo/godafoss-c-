#include "godafoss.hpp"

namespace gf = godafoss;
using target = gf::target<>;
using timing = target::timing;

using spi_bus = gf::spi_bus_bb_sclk_mosi< target::d6, target::d5, timing, true >;
using oled = gf::ssd1306_spi_ss_dc< spi_bus, target::d2, target::d3 >;

int main(){
   gf::run< gf::demo::bouncing_square< oled, timing::ms< 0 > > >();
};

/*
int main( void ){
   gf::demo::bouncing_square< oled, timing::ms< 0 >, 8 > ();
}
*/

/*
 *


template< application >
void run(){

}

run draait gewoon zijn init, maar ook zijn backgrounds en taken

up-down aspecten:
* backround
* threads

timing moet dus via de run gaan


* */

