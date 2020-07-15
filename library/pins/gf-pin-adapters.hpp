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
// @title pin adapters
//
// These adapters adapt a pin to be (only) an input pin,
// (only) an output pin,
// or (only) an input_output pin, or (obly) an open collector pin.
// (in each case, if such adaption is possible).
//
// These adapters serve, of course, to adapt a given pin to the
// adapted role, but also to ensure that the code that uses the adapted
// pin, doesn't use any features beyond the ones of the adapted role.
//
// -----------------------------------------------------------------------------
//
// @define godafoss::pin_input
// @section pin_input
//
// @insert can_input
// @insert pin_in_from
//
// The pin_input<> decorator decorates an pin to be an input pin,
// which is possible if the pin satisfies the can_input concept,
// which requires the pin to be either an input or an input_output.
//
// -----------------------------------------------------------------------------
//
// @define godafoss::pin_output
// @section pin_output
//
// @insert can_output
// @insert pin_out_from
//
// The pin_output<> decorator decorates an pin to be an output pin,
// which is possible if the pin satisfies the can_output concept,
// which requires the pin to be either an input or an input_output.
//
// -----------------------------------------------------------------------------
//
// @define godafoss::pin_input_output
// @section pin_input_output
//
// @insert can_input_output
// @insert pin_in_out_from
//
// The pin_input_output<> decorator decorates
// an pin to be an input_output pin,
// which is possible if the pin satisfies the can_input_output concept,
// which requires the pin to an input_output.
//
// -----------------------------------------------------------------------------
//
// @define godafoss::pin_oc
// @section pin_input_oc
//
// @insert can_oc
// @insert pin_oc_from
//
// The pin_input_output<> decorator decorates
// an pin to be an input_output pin,
// which is possible if the pin satisfies the can_input_output concept,
// which requires the pin to an input_output.
//
// =============================================================================


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
   inherit_read< T >,
   pullup_pulldown_filter< T >
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
   inherit_init< T >,
   inherit_read< T >,
   pullup_pulldown_filter< T >
{};

template< pin_oc T >
struct pin_in_supported< T > {
   static constexpr bool supported = true;;
};

template< pin_oc T >
struct pin_in_from< T > :
   pin_in_root,
   inherit_read< T >,
   pullup_pulldown_filter< T >
{

   static void GODAFOSS_INLINE init(){
      T::init();
      direct< T >::write( 1 );
   }

};


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
   inherit_init< T >,
   inherit_write< T >
{};

template< pin_in_out T >
struct pin_out_supported< T > {
   static constexpr bool supported = true;;
};

template< pin_in_out T >
struct pin_out_from< T > :
   pin_out_root,
   inherit_write< T >
{

   static void GODAFOSS_INLINE init(){
      T::init();
      direct< T >::direction_set_output();
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
   inherit_init< T >,
   inherit_read< T >,
   inherit_write< T >,
   pullup_pulldown_filter< T >
{};


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
   inherit_init< T >,
   inherit_write< T >,
   inherit_read< T >,
   pullup_pulldown_filter< T >
{};

template< pin_in_out T >
struct pin_oc_supported< T > {
   static constexpr bool supported = true;
};

template< pin_in_out T >
struct pin_oc_from< T > :
   pin_oc_root,
   inherit_init< T >,
   inherit_write< T >,
   inherit_read< T >,
   pullup_pulldown_filter< T >
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
