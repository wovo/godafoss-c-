// ==========================================================================
//
// gf-port-dummies.hpp
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


template< int n >
struct port_in_out_dummy : 
   be_port_in_out< n > 
{   

   using _vt = typename be_port_in_out< n >::value_type;
   
   static void GODAFOSS_INLINE init(){}   

   static void GODAFOSS_INLINE write( _vt v ){ 
      (void) v; 
   }      
	  
   static _vt GODAFOSS_INLINE read(){ 
      return 0; 
   }      
	  
   static void GODAFOSS_INLINE direction_set_input(){
   }

   static void GODAFOSS_INLINE direction_set_output(){
   }
};


// ==========================================================================

template< int n >
using port_out_dummy = port_out< port_in_out_dummy< n > >;  


// ==========================================================================

template< int n >
using port_in_dummy  = port_in< port_in_out_dummy< n > >;      


// ==========================================================================

template< int n >
struct port_oc_dummy : 
   be_port_oc< n > 
{   
    
   using _vt = typename be_port_oc< n >::value_type;
   
   static void GODAFOSS_INLINE init(){}   

   static void GODAFOSS_INLINE write( _vt v ){ 
      (void) v; 
   }      
	  
   static _vt GODAFOSS_INLINE read(){ 
      return 0; 
   }      
};

