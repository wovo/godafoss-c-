// =============================================================================
//
// gf-demos.hpp
//
// =============================================================================
//
// This file is part the https://www.github.com/godafoss
// free C++ library for close-to-the-hardware programming.
//
// Copyright Wouter van Ooijen 2019
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================


template< can_pin_out _pin, can_static_duration _pause >
struct blink : cto_root { 
   //using pin = direct< pin_out_from< _pin >>;
   using pin = pin_out_from< _pin >;
   using pause = static_duration< _pause >;

   using resources = use_list< pin, pause >;
   //using use = list< pin, pause >;

   static void GODAFOSS_NO_RETURN run(){
      for(;;){
         pin::write( 1 );
         pause::wait();
         pin::write( 0 );
         pause::wait();
      }
   }
};

/*
template< can_pin_out _pin, duration pause >
struct blink : cto_root { struct< context gf > {

   using pin = direct< pin_out_from< _pin > >;

   static void GODAFOSS_NO_RETURN main(){
      for(;;){
         pin::write( 1 );
         pause::wait();
         pin::write( 0 );
         pause::wait();
      }
   }
   
   using composition = list< pin, pause, run< main > >;

};
*/


/*
template< can_pin_out _p, typename d, typename t >
void blink(){
   using p = direct< pin_out< _p >>;
   p::init();
   d::init();
   t::init();
   auto end = d::timing::now() + t;
   while( now() < end ){
      p::write( 1 );
      d::wait();
      p::write( 0 );
      d::wait();
   }
}
*/
