// =============================================================================
//
// gf-pin-adapters.hpp
//
// =============================================================================
//
//
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
// concepts
//
// =============================================================================



// =============================================================================
//
// pull-ups
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
// in_out
//
// =============================================================================

GODAFOSS_SUPPORTED( pin_in_out, pin_in_out_from )

template< pin_in_out T >
struct pin_in_out_supported< T > {
   static constexpr bool supported = true;;
};

template< pin_in_out T >
struct pin_in_out_from< T > :
   pin_out_root,
   box_inherit_init< T >,
   box_inherit_read< T >,
   box_inherit_write< T >,
   pullup_filter< T >
{};


// =============================================================================
//
// out
//
// =============================================================================

GODAFOSS_SUPPORTED( pin_out, pin_out_from )

template< pin_out T >
struct pin_out_supported< T > {
   static constexpr bool supported = true;;
};

template< pin_out T >
struct pin_out_from< T >  :
   pin_out_root,
   box_inherit_init< T >,
   box_inherit_write< T >
{};

template< pin_in_out T >
struct pin_out_supported< T > {
   static constexpr bool supported = true;;
};

template< pin_in_out T >
struct pin_out_from< T > :
   pin_out_root,
   box_inherit_write< T >
{

   static void GODAFOSS_INLINE init(){
      T::init();
      direct< T >::direction_set_output();
   }

};


// =============================================================================
//
// in
//
// =============================================================================

GODAFOSS_SUPPORTED( pin_in, pin_in_from )

template< pin_in_out T >
struct pin_in_supported< T > {
   static constexpr bool supported = true;;
};

template< pin_in_out T >
struct pin_in_from< T > :
   pin_in_root,
   box_inherit_read< T >,
   pullup_filter< T >
{

   static void GODAFOSS_INLINE init(){
      T::init();
      direct< T >::direction_set_input();
   }

};

template< pin_in T >
struct pin_in_supported< T > {
   static constexpr bool supported = true;;
};

template< pin_in T >
struct pin_in_from< T > :
   pin_in_root,
   box_inherit_init< T >,
   box_inherit_read< T >,
   pullup_filter< T >
{};

template< pin_oc T >
struct pin_in_supported< T > {
   static constexpr bool supported = true;;
};

template< pin_oc T >
struct pin_in_from< T > :
   pin_in_root,
   box_inherit_read< T >,
   pullup_filter< T >
{

   static void GODAFOSS_INLINE init(){
      T::init();
      direct< T >::write( 1 );
   }

};


// =============================================================================
//
// oc
//
// =============================================================================

GODAFOSS_SUPPORTED( pin_oc, pin_oc_from )

template< pin_oc T >
struct pin_oc_supported< T > {
   static constexpr bool supported = true;;
};

template< pin_oc T >
struct pin_oc_from< T > :
   pin_oc_root,
   box_inherit_init< T >,
   box_inherit_write< T >,
   box_inherit_read< T >,
   pullup_filter< T >
{};

template< pin_in_out T >
struct pin_oc_supported< T > {
   static constexpr bool supported = true;
};

template< pin_in_out T >
struct pin_oc_from< T > :
   pin_oc_root,
   box_inherit_init< T >,
   box_inherit_write< T >,
   box_inherit_read< T >,
   pullup_filter< T >
{

   static void GODAFOSS_INLINE init(){
      T::init();
      direct< T >::direction_set_input();
   }

   static void GODAFOSS_INLINE write( bool v ){
       if( v ){
          T::direction_set_input();
       } else {
          T::direction_set_output();
          T::write( 0 );
       }
   }

   static void GODAFOSS_INLINE flush(){
      T::direction_flush();
      T::flush();
   }

};
