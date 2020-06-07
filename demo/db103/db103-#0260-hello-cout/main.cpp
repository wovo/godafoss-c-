#include "godafoss.hpp"

namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;
using uart    = target::uart<>;
   
int main( void ){
   timing::ms< 2'000 >::wait();
   uart::init();
   gf::ostream< gf::formatter< uart > > cout;
   cout << "Hello world\n";
}  
