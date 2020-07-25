// ==========================================================================
//
// gf-fraction.hpp
//
// ==========================================================================
//
// This file is part of godafoss,
// a C++ library for close-to-the-hardware programming.
//
// Copyright Wouter van Ooijen 2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// ==========================================================================
//
// @define fraction
// @title fraction
//
// A fraction is a value relative to some (static) maximum.
// Think of it as a percentage, or a value in the range 0.0 .. 1.0.
//
// @insert fraction
//
// A fraction has value, which should be interpreted relative to
// its full_scale. Both are of the type T.
//
// @insert assign
//
// A fraction can be assigned from another fraction, which can
// have a different full_scale. The assignement rescales the value.
//
// @ insert rescale
//
// The rescale function retruns the value, rescaled to the
// full_scale argument.
//
// ==========================================================================

// @quote fraction 8 \n   ...\n};
template< typename T, T _full_scale >
struct fraction {

   using data_type = T;
   static constexpr data_type full_scale = _full_scale;

   data_type raw_value;

   // @quote constructor 2 ...
   explicit fraction( T x ):
      raw_value( x )
   {}

   // @quote assign 2 ... }
   template< typename V, V rhs_full_scale >
   fraction & operator=( const fraction< V, rhs_full_scale > & rhs ){
      raw_value = ( rhs.raw_value * rhs_full_scale ) / full_scale;
   }

   // @quote multiply 2 ... }
   template< typename V >
   V operator*( V rhs ){
      return ( raw_value * rhs ) / full_scale;
   }

   // @quote rescale 2 ... }
//   template< typename V, V other_full_scale >
//   V rescale( V other_full_scale ){
//      return ( value * full_scale ) / other_full_scale;
//   }
};

