// ==========================================================================
//
// gf-pin-dummies.hpp
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


struct pin_in_out_dummy : 
   be_pin_in_out 
{   
   
   static void GODAFOSS_INLINE init(){}   

   static void GODAFOSS_INLINE write( bool v ){ 
      (void) v; 
   }      
	  
   static bool GODAFOSS_INLINE read(){ 
      return 0; 
   }      
	  
   static void GODAFOSS_INLINE flush(){
   }

   static void GODAFOSS_INLINE refresh(){
   }
   
   static void GODAFOSS_INLINE direction_set_input(){
   }

   static void GODAFOSS_INLINE direction_set_output(){
   }
};


// ==========================================================================

using pin_out_dummy = pin_out< pin_in_out_dummy >;  

using pin_in_dummy  = pin_in< pin_in_out_dummy >;      

using pin_oc_dummy  = pin_oc< pin_in_out_dummy >;      


