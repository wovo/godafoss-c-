// =============================================================================
//
// gf-pin-adapters.hpp
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
// @title specific pin adapters
//
// These adapters create a pin cto from a specific (same or other) pin cto.
//
// The created pin has only the properties required for that pin:
// other properties of the source pin are not available via the created pin.
// The exception is pullup and pulldown features: those are
// available via the created pins.
//
// These adapters can only be used when the source pin is know.
// For general use, the pin adapters that accept any (possible) source
// pin are more covenient.
//
// @define pin_in_from_pin_in
// @insert pin_in_from_pin_in
//
// @define pin_in_from_pin_in_out
// @insert pin_in_from_pin_in_out
//
// @define pin_in_from_pin_oc
// @insert pin_in_from_pin_oc
//
// @define pin_out_from_pin_out
// @insert pin_out_from_pin_out
//
// @define pin_out_from_pin_in_out
// @insert pin_out_from_pin_in_out
//
// @define pin_out_from_pin_oc
// @insert pin_out_from_pin_oc
//
// @define pin_in_out_from_pin_in_out
// @insert pin_in_out_from_pin_in_out
//
// @define pin_in_out_from_pin_oc
// @insert pin_in_out_from_pin_oc
//
// @define pin_oc_from_pin_oc
// @insert pin_oc_from_pin_oc
//
// =============================================================================



// =============================================================================
//
// in
//
// =============================================================================

// @quote pin_in_from_pin_in 2 ... {};
template< is_pin_in T >
struct pin_in_from_pin_in :
   pin_in_root,
   inherit_init< T >,
   inherit_read< T >,
   pullup_pulldown_filter< T >
{};

// @quote pin_in_from_pin_in_out 2 ... {};
template< is_pin_in_out T >
struct pin_in_from_pin_in_out :
   pin_in_root,
   inherit_read< T >,
   pullup_pulldown_filter< T >
{

   static void GODAFOSS_INLINE __init(){
      direct< T >::direction_set_input();
   }

   using resources = use_list<
      T,
      execute< __init >
   >;


};

// @quote pin_in_from_pin_oc 2 ... {};
template< is_pin_oc T >
struct pin_in_from_pin_oc :
   pin_in_root,
   inherit_read< T >,
   pullup_pulldown_filter< T >
{

   static void GODAFOSS_INLINE __init(){
      direct< T >::write( 1 );
   }

   using resources = use_list<
      T,
      execute< __init >
   >;



};

// =============================================================================

template< typename T >
concept can_pin_in_from =
      is_pin_in< T >
   || is_pin_in_out< T >
   || is_pin_oc< T >;

template< can_pin_in_from T >
struct pin_in_from {
   // The specializations should cover all possibilities.
   static_assert( sizeof( T ) < 0, "library internal error\n" );
};

template< is_pin_in T  >
struct pin_in_from< T > : pin_in_from_pin_in< T > {};

template< is_pin_in_out T  >
struct pin_in_from< T > : pin_in_from_pin_in_out< T > {};

template< is_pin_oc T  >
struct pin_in_from< T > : pin_in_from_pin_oc< T > {};


// =============================================================================
//
// out
//
// =============================================================================

// @quote pin_out_from_pin_out 2 ... {};
template< is_pin_out T >
struct pin_out_from_pin_out :
   pin_out_root,
   inherit_init< T >,
   inherit_write< T >
{};

// @quote pin_out_from_pin_in_out 2 ... {};
template< is_pin_in_out T >
struct pin_out_from_pin_in_out :
   pin_out_root,
   inherit_write< T >
{

   //static void GODAFOSS_INLINE init(){
   //   T::init();
   //   direct< T >::direction_set_output();
   //}

   using resources = use_list<
      direct< T >,
      execute< direct< T >::direction_set_output >
   >;

};

// @quote pin_out_from_pin_oc 2 ... {};
template< is_pin_oc T >
struct pin_out_from_pin_oc :
   pin_out_root,
   inherit_init< T >,
   inherit_write< T >
{};

// =============================================================================

template< typename T >
concept can_pin_out_from =
      is_pin_out< T >
   || is_pin_in_out< T >
   || is_pin_oc< T >;

template< can_pin_out_from T >
struct pin_out_from {
   // The specializations should cover all possibilities.
   static_assert( sizeof( T ) < 0, "library internal error\n" );
};

template< is_pin_out T  >
struct pin_out_from< T > : pin_out_from_pin_out< T > {};

template< is_pin_in_out T  >
struct pin_out_from< T > : pin_out_from_pin_in_out< T > {};

template< is_pin_oc T  >
struct pin_out_from< T > : pin_out_from_pin_oc< T > {};


// =============================================================================
//
// in_out
//
// =============================================================================

// @quote pin_in_out_from_pin_in_out 2 ... {};
template< is_pin_in_out T >
struct pin_in_out_from_pin_in_out :
   pin_out_root,
   inherit_init< T >,
   inherit_read< T >,
   inherit_write< T >,
   pullup_pulldown_filter< T >
{};

// @quote pin_in_out_from_pin_oc 2 ... {};
template< is_pin_oc T >
struct pin_in_out_from_pin_oc :
   pin_in_out_root,
   inherit_init< T >,
   inherit_write< T >,
   inherit_read< T >,
   pullup_pulldown_filter< T >
{

   static GODAFOSS_INLINE void direction_set_input(){
      // make the pin high, which is effectively input
      T::write( 1 );
   }

   static GODAFOSS_INLINE void direction_set_output(){}

   static GODAFOSS_INLINE void direction_set_flush(){
      T::flush();
   }

};

// =============================================================================

template< typename T >
concept can_pin_in_out_from =
      is_pin_in_out< T >
   || is_pin_oc< T >;

template< can_pin_in_out_from T >
struct pin_in_out_from{
   // The specializations should cover all possibilities.
   static_assert( sizeof( T ) < 0, "library internal error\n" );
};

template< is_pin_in_out T >
struct pin_in_out_from< T > : pin_in_out_from_pin_in_out< T > {};

template< is_pin_oc T >
struct pin_in_out_from< T > : pin_in_out_from_pin_oc< T > {};


// =============================================================================
//
// oc
//
// =============================================================================

// @quote pin_oc_from_pin_in_out 2 ... {};
template< is_pin_in_out T >
struct pin_oc_from_pin_in_out :
   pin_oc_root,
   inherit_init< T >,
   inherit_write< T >,
   inherit_read< T >,
   pullup_pulldown_filter< T >
{

   //static void GODAFOSS_INLINE init(){
   //   T::init();
   //   direct< T >::direction_set_input();
   //}

   using resources = use_list<
      T,
      execute< direct< T >::direction_set_input >
   >;

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

// @quote pin_oc_from_pin_oc 2 ... {};
template< is_pin_oc T >
struct pin_oc_from_pin_oc :
   pin_oc_root,
   inherit_init< T >,
   inherit_write< T >,
   inherit_read< T >,
   pullup_pulldown_filter< T >
{};

// =============================================================================

template< typename T >
concept can_pin_oc_from =
      is_pin_in_out< T >
   || is_pin_oc< T >;

template< can_pin_oc_from T >
struct pin_oc_from {
   // The specializations should cover all possibilities.
   static_assert( sizeof( T ) < 0, "library internal error\n" );
};

template< is_pin_in_out T  >
struct pin_oc_from< T > : pin_oc_from_pin_in_out< T > {};

template< is_pin_oc T  >
struct pin_oc_from< T > : pin_oc_from_pin_oc< T > {};
