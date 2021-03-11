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
   port_in_out_root< n >
{

   using _vt = typename port_in_out_root< n >::value_type;

   using resources = use_list<>;

   static void GODAFOSS_INLINE write( _vt v ){
      (void) v;
   }

   static _vt GODAFOSS_INLINE read(){
      return 0;
   }

   static void GODAFOSS_INLINE flush(){}

   static void GODAFOSS_INLINE refresh(){}

   static void GODAFOSS_INLINE direction_set_input(){}

   static void GODAFOSS_INLINE direction_set_output(){}

   static void GODAFOSS_INLINE direction_flush(){}

};


// ==========================================================================

template< int n >
using port_out_dummy = port_out_from_port_in_out< port_in_out_dummy< n > >;


// ==========================================================================

template< int n >
using port_in_dummy  = port_in_from_port_in_out< port_in_out_dummy< n > >;


// ==========================================================================

// there is no port_oc< port_in_out > because port pins
// don't have individual direction control

template< int n >
struct port_oc_dummy :
   port_oc_root< n >
{

   using _vt = typename port_oc_root< n >::value_type;

   using resources = use_list<>;

   static void GODAFOSS_INLINE write( _vt v ){
      (void) v;
   }

   static void GODAFOSS_INLINE flush(){}

   static void GODAFOSS_INLINE refresh(){}

   static _vt GODAFOSS_INLINE read(){
      return 0;
   }
};
