// =============================================================================
//
// gf-pin-dummies.hpp
//
// =============================================================================
//
// Dummy gpio 'objects' of the various kinds (in, out, in-out, oc).
//
// These dummies are usefull as placeholders for unused parameters,
// and as endpoints for recusive exapansions.
//
// =============================================================================
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


struct pin_in_out_dummy :
   pin_in_out_root
{

   using resources = use_list<>;

   static void GODAFOSS_INLINE refresh(){}

   static bool GODAFOSS_INLINE read(){
      return 0;
   }

   static void GODAFOSS_INLINE write( by_const< bool > v ){
      (void) v;
   }

   static void GODAFOSS_INLINE flush(){}

   static void GODAFOSS_INLINE direction_set_input(){}

   static void GODAFOSS_INLINE direction_set_output(){}

   static void GODAFOSS_INLINE direction_flush(){}

};


// =============================================================================

using pin_in_dummy =
   pin_in_from_pin_in_out< pin_in_out_dummy >;

using pin_out_dummy =
   pin_out_from_pin_in_out< pin_in_out_dummy >;

using pin_oc_dummy =
   pin_oc_from_pin_in_out< pin_in_out_dummy >;


