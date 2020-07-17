// ==========================================================================
//
// gf-port-adapters.hpp
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


// ==========================================================================
//
// adapters
//
// ==========================================================================

GODAFOSS_SUPPORTED( port_in_out, port_in_out_from )

template< is_port_in_out T >
struct port_in_out_supported< T > {
   static constexpr bool supported = true;;
};

template< is_port_in_out T >
struct port_in_out_from< T > :
   port_in_out_root< T::n_pins >,
   inherit_init< T >,
   inherit_direction< T >,
   inherit_write< T >,
   inherit_read< T >
{};

template< is_port_oc T >
struct port_in_out_supported< T > {
   static constexpr bool supported = true;;
};

template< is_port_oc T >
struct port_in_out_from< T > :
   port_in_out_root< T::n_pins >,
   inherit_init< T >,
   inherit_write< T >,
   inherit_read< T >
{

   static GODAFOSS_INLINE void direction_set_input(){
      // make all pins high, which is effectively input
      T::write( ~0 );
   }

   static GODAFOSS_INLINE void direction_set_output(){}

   static GODAFOSS_INLINE void direction_set_flush(){
      T::flush();
   }

};

// ==========================================================================

GODAFOSS_SUPPORTED( port_out, port_out_from )

template< is_port_in_out T >
struct port_out_supported< T > {
   static constexpr bool supported = true;;
};

template< is_port_in_out T >
struct port_out_from< T > :
   port_out_root< T::n_pins >,
   inherit_init< T >,
   inherit_write< T >
{

   static GODAFOSS_INLINE void init(){
      T::init();
      T::direction_set_output();
      T::direction_flush();
   }

};

template< is_port_out T >
struct port_out_supported< T > {
   static constexpr bool supported = true;;
};

template< is_port_out T >
struct port_out_from< T > :
   port_out_root< T::n_pins >,
   inherit_init< T >,
   inherit_write< T >
{};

template< is_port_oc T >
struct port_out_supported< T > {
   static constexpr bool supported = true;;
};

template< is_port_oc T >
struct port_out_from< T > :
   port_out_root< T::n_pins >,
   inherit_init< T >,
   inherit_write< T >
{};

// ==========================================================================


GODAFOSS_SUPPORTED( port_in, port_in_from )

template< is_port_in_out T >
struct port_in_supported< T > {
   static constexpr bool supported = true;;
};

template< is_port_in_out T >
struct port_in_from< T > :
   port_in_root< T::n_pins >,
   inherit_init< T >,
   inherit_read< T >
{

   static GODAFOSS_INLINE void init(){
      T::init();
      T::direction_set_input();
      T::direction_flush();
   }

};

template< is_port_in T >
struct port_in_supported< T > {
   static constexpr bool supported = true;;
};

template< is_port_in T >
struct port_in_from< T > :
   port_in_root< T::n_pins >,
   inherit_init< T >,
   inherit_read< T >
{};

template< is_port_oc T >
struct port_in_supported< T > {
   static constexpr bool supported = true;;
};

template< is_port_oc T >
struct port_in_from< T > :
   port_in_root< T::n_pins >,
   inherit_init< T >,
   inherit_read< T >
{

   static GODAFOSS_INLINE void init(){
      T::init();
      T::write( 0 );
      T::flush();
   }

};

// ==========================================================================


GODAFOSS_SUPPORTED( port_oc, port_oc_from )

template< is_port_oc T >
struct port_oc_supported< T > {
   static constexpr bool supported = true;;
};

template< is_port_oc T >
struct port_oc_from< T > :
   port_oc_root< T::n_pins >,
   inherit_init< T >,
   inherit_write< T >,
   inherit_read< T >
{};
