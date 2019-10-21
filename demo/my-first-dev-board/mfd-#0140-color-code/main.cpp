// ==========================================================================
//
// (c) Wouter van Ooijen (wouter@voti.nl) 2019
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

using i2c_bus = gf::i2c_bus_bb_scl_sda< target::d4, target::d5, timing >;
using chip    = gf::pcf8574a< i2c_bus, 0b0111 >;
using lcd     = gf::hd44780_rs_e_d_x_y_timing< 
   chip::p0,
   chip::p2,
   gf::port_out<
      chip::p4,
      chip::p5,
      chip::p6,
      chip::p7 >,
   16, 2,
   timing >; 
   
const char * names[] = { "blauw", "geel", "groen", "rood" };

long int down(){
   timing::ms< 50 >::wait();
   if( target::sw1::read() ) return 0;
   if( target::sw2::read() ) return 1;
   if( target::sw3::read() ) return 2;
   if( target::sw4::read() ) return 3;   
   return -1;
}   

int pressed(){
   while( down() != -1 ){}
   for(;;){
      auto d = down();
      if( d != -1 ) return d;
   }
}

struct opdracht {
   long int code;
   const char *text;
};

opdracht opdrachten[] = {
   { 1324, "electronica"  },
   { 4132, "solderen"     },
   { 1243, "knutselen"    },
   { 3412, "robot"        },
   { 4213, "raspberry Pi" },
   { 3214, "3D printer"   },
};

constexpr const opdracht * show( long int code ){
   for( auto & t : opdrachten ){
      if( t.code == code ) return &t;
   }
   return nullptr;
}

int main( void ){
   target::sw1::init();
   target::sw2::init();
   target::sw3::init();
   target::sw4::init();
   
   target::sw1::pullup_enable();
   target::sw2::pullup_enable();
   target::sw3::pullup_enable();
   target::sw4::pullup_enable();   
    
   gf::direct< chip::p1 >::write( 0 );
   gf::direct< chip::p3 >::write( 1 );   
   gf::ostream< gf::formatter< lcd > > cout;
   for(;;){
      long int code = 0;
      for( int i = 0; i < 4 ; ++i ){
         cout << "\fvoer je code in\n";     
         cout << "kleur " << i + 1 << "? ";
         auto k = pressed();          
         cout << names[ k ];
         while( down() != -1 ){}
         code = 10 * code + ( k + 1 );
      }
      auto p = show( code );
      if( p == nullptr ){
         cout 
            << "\fDat is geen\ngeldige code";
         
      } else {
         cout 
            << "\fje wachtwoord is:\n  "
            << p->text;
      }
      timing::ms< 3'000 >::wait();
   }
}