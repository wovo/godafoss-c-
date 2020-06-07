#include "godafoss.hpp"

namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;
using uart    = target::uart<>;



struct str {
   char x;
//   int y[ 10 ];
};

using s = str;

GODAFOSS_NO_INLINE void f_value( s p ){
   volatile auto dummy = p.x;
   (void) dummy;
}

GODAFOSS_NO_INLINE void f_reference( const s & p ){
   volatile auto dummy = p.x;
   (void) dummy;
}

GODAFOSS_NO_INLINE void f_by_by( gf::by_const< s > p ){
   //gf::ostream< gf::formatter< uart > > cout;
   volatile auto dummy = p.x;
   (void) dummy;
   //cout << "Hello!\n";
   //cout << sizeof( p ) << "\n";
}

int main( void ){
   f_value( s() );
   f_reference( s() );
   f_by_by( s() );
}
