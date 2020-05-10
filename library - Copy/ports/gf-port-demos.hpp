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


template< can_port_out _port, typename interval >
GODAFOSS_NORETURN void walk(){
   using port = direct< port_out< _port >>;	
   port::init();
   interval::init();
   for(;;){
      for( uint_fast8_t  i = 0; i < port::n_pins; ++i ){
         port::write( (typename port::value_type) ( 0x01 << i ) );      
         interval::wait(); 
      }
   }      
}

template< can_port_out _port, typename interval >
GODAFOSS_NORETURN void kitt(){
   using port = direct< port_out< _port >>;	
   port::init();
   interval::init();
   for(;;){
      for( uint_fast8_t  i = 0; i < port::n_pins; ++i ){
         port::write( (typename port::value_type) ( 0x01 << i ) );   
         interval::wait(); 
      }
      for( uint_fast8_t  i = port::n_pins - 2; i > 0; --i ){
         port::write( (typename port::value_type) ( 0x01 << i ) );      
         interval::wait(); 
      }
   }      
}