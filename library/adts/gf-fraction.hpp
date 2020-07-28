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
// Think of it as a percentage, or a factor in the range 0.0 .. 1.0.
//
// @insert fraction
//
// A fraction has raw_value, which should be interpreted relative to
// its full_scale. Both are of the type T.
//
// @insert constructor
//
// The constructor is explicit, to avoid the mistake of passing a value
// where a raction is expected.
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
   constexpr explicit fraction( T x ):
      raw_value( x )
   {}

   // @quote assign 2 ... }
   template< typename V, V rhs_full_scale >
   constexpr fraction & operator=( const fraction< V, rhs_full_scale > & rhs ){
      raw_value = ( rhs.raw_value * rhs_full_scale ) / full_scale;
   }

   // @quote of 2 ... }
   template< typename V >
   constexpr V of( V x ) const {
      return ( x * raw_value ) / _full_scale;
   }

   // @quote of 2 ... }
   template< typename V, typename W >
   constexpr V of( V x, W y ) const {
      return x + of( y - x );
   }

   // @quote negate 1 ... }
   constexpr fraction operator-() const {
      return fraction( full_scale - raw_value );
   }

   // @quote multiply 2 ... }
   template< typename V >
   constexpr V operator *( V rhs ) const {
      return raw_value * rhs;
   }

   // @quote multiply 2 ... }
   template< typename V >
   friend constexpr V operator *( V lhs, fraction rhs ) {
      return rhs * lhs;
   }

   // @quote multiply 2 ... }
   template< typename V >
   constexpr V operator *( V rhs ) const {
      return raw_value * rhs;
   }

   // @quote divide 2 ... }
   template< typename V >
   constexpr V operator / ( V rhs ) const {
      return raw_value / rhs;
   }

   // @quote compare 2 ... }
   template< typename V >
   constexpr bool operator==( V rhs ) const {
      return raw_value * V::full_scale == rhs.raw_value * full_scale;
   }

   // @quote compare 2 ... }
   template< typename V >
   constexpr bool operator!=( V rhs ) const {
      return ! ( *this == rhs );
   }

};

