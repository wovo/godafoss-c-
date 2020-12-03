// ============================================================================
//
// gf-board-arduino-leonardo.hpp
//
// ============================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss),
// a C++ library for close-to-the-hardware programming.
//
// Copyright
//    Wouter van Ooijen 2019-2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================
//
// This file defines the arduino leonardo (atmega32u4) board target.
//
// =============================================================================

#include "target-hals/gf-atmega.hpp"

namespace godafoss {
    
template< uint64_t clock = 16'000'000 >
struct target : 
   atmega< clock > 
{       
   
   using chip = atmega< clock >;	
   
private:   
   
	// aliases
   
   using port = chip::port;
   template< port p, int n > struct gpio : chip::template gpio< p, n > {};
   template< int n > struct adc : chip::template adc< 0 >{};
   
public:  

   // general data pins      
      
   using  d0  = gpio< port::d, 2 >;   
   using  d1  = gpio< port::d, 3 >;   
   using  d2  = gpio< port::d, 1 >;   
   using  d3  = gpio< port::d, 0 >;   
   using  d4  = gpio< port::d, 4 >;   
   using  d5  = gpio< port::c, 6 >;   
   using  d6  = gpio< port::d, 7 >;   
   using  d7  = gpio< port::e, 6 >;   
   
   using  d8  = gpio< port::b, 4 >;   
   using  d9  = gpio< port::b, 5 >;   
   using d10  = gpio< port::b, 6 >;   
   using d11  = gpio< port::b, 7 >;   
   using d12  = gpio< port::d, 6 >;   
   using d13  = gpio< port::c, 7 >;   
   
   using d14  = gpio< port::c, 0 >;   
   using d15  = gpio< port::c, 1 >;   
   using d16  = gpio< port::c, 2 >;   
   using d17  = gpio< port::c, 3 >;   
   using d18  = gpio< port::c, 4 >;   
   using d19  = gpio< port::c, 5 >;   
   
   // analog input

   using  a0  = gpio< port::c, 7 >;   
   using  a1  = gpio< port::c, 6 >;   
   using  a2  = gpio< port::c, 5 >;   
   using  a3  = gpio< port::c, 4 >;   
   using  a4  = gpio< port::c, 1 >;   
   using  a5  = gpio< port::c, 0 >;   
   
   // designated pins
    
   using tx   = pin_out<  d0 >;
   using rx   = pin_in<   d1 >;
   using scl  = pin_oc<  d18 >;
   using sda  = pin_oc<  d19 >;
   using led  = pin_out< d13 >;

   using sck  = pin_out<  d7 >;
   using miso = pin_in <  d8 >;
   using mosi = pin_out<  d9 >;
   using ss   = pin_out< d10 >;  
   
   using adc0 = adc< 0 >;
   using adc1 = adc< 1 >;
   using adc2 = adc< 2 >;
   using adc3 = adc< 3 >;
   using adc4 = adc< 4 >;
   using adc5 = adc< 5 >;
   
   // timing options
   
   using waiting = typename chip::waiting;
   using clocking = typename chip::clocking;
   
   // default timing
   
   using timing = waiting;	  	  
   
}; // struct target

}; // namespace godafoss
