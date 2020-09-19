// =============================================================================
//
// gf-restricted.hpp
//
// =============================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss),
// a C++ library for close-to-the-hardware programming.
//
// Copyright
//    Wouter van Ooijen 2018-2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================

#define GODAFOSS_ADT_OPERATOR( bits, op )
   template< typename _other_base >
      requires requires( _base a ){
         { op a }
      }
   inline auto operator | (){
      return base( op value );
   }

#define GODAFOSS_ADT_OPERATOR( bits, op )
   template< typename _other_base >
      requires requires( _base a, _other_base b ){
         { a op b }
      }
   inline auto operator | ( bits< _other_base > x ){
      return base( value op b.value );
   }

template< typename _base >
struct bits {
   using base = _base;
   base value = 0;

   constexpr bits(): value( 0 ){}
   constexpr explicit bits( base v ): value( v ){}
   constexpr bits( bits x ): value( x.value ){}

   inline bits & operator = ( bits x ){
      value = x.value;
      return *this;
   }

   GODAFOSS_BINARY_ADT_OPERATOR( bits, == )
   GODAFOSS_BINARY_ADT_OPERATOR( bits, != )

   GODAFOSS_BINARY_ADT_OPERATOR( bits, & )
   GODAFOSS_BINARY_ADT_OPERATOR( bits, | )
   GODAFOSS_BINARY_ADT_OPERATOR( bits, ^ )
   GODAFOSS_BINARY_ADT_OPERATOR( bits, | )
   GODAFOSS_UNARY_ADT_OPERATOR ( bits, ~ )

};
