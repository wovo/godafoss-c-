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

template< typename t >
struct runner {
   static void init(){
      t::init();
      runner< t::next >::init();
   }
};

template<>
struct runner< void > {
   static void init(){}
};

template< typename application >
struct run {

   static void GODAFOSS_NO_RETURN run(){
      runner< application >::init();
      application::run();
   }
};


// =============================================================================


template< can_pin_out _pin, can_static_duration _pause >
struct blink {

   using pin = direct< pin_out_from< _pin >>;
   using pause = static_duration< _pause >;

   using resources = resources< pin, pause >;

   static void GODAFOSS_NO_RETURN blink(){
      for(;;){
         pin::write( 1 );
         pause::wait();
         pin::write( 0 );
         pause::wait();
      }
   }
};



int main(){
   <hoe vind je de naam van wat er gerunt moet worden?
}

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
