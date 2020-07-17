// =============================================================================
//
// gf-pin-pullup-pulldown.hpp
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


// =============================================================================
//
// pullup
//
// =============================================================================

template< typename T >
concept pullup_capable = requires {
   T::pullup_enable();
   T::pullup_disable();
};


template< typename T >
struct pullup_filter {};

template< pullup_capable T >
struct pullup_filter< T > {

   static void GODAFOSS_INLINE pullup_enable(){
      T::pullup_enable();
   }

   static void GODAFOSS_INLINE pullup_disable(){
      T::pullup_disable();
   }
};


// =============================================================================
//
// pulldown
//
// =============================================================================

template< typename T >
concept pulldown_capable = requires {
   T::pulldown_enable();
   T::pulldown_disable();
};

template< typename T >
struct pulldown_filter {};

template< pulldown_capable T >
struct pulldown_filter< T > {

   static void GODAFOSS_INLINE pulldown_enable(){
      T::pulldown_enable();
   }

   static void GODAFOSS_INLINE pulldown_disable(){
      T::pulldown_disable();
   }
};


// =============================================================================
//
// filter
//
// =============================================================================

template< typename T >
struct pullup_pulldown_filter :
   pullup_filter< T >,
   pulldown_filter< T >
{};
