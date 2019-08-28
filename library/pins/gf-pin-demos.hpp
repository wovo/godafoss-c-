// ==========================================================================
//
// gf-demos.hpp
//
// ==========================================================================
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
// ==========================================================================


template< can_pin_out _p, typename d >
GODAFOSS_NORETURN void blink(){
   using p = direct< pin_out< _p >>;	
   p::init();
   d::init();
   for(;;){
      p::write( 1 );
      d::wait();	  
      p::write( 0 );
      d::wait();	  
   }	   
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