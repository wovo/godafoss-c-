// =============================================================================
//
// gf-port-adapters.hpp
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
// @title specific port adapters
//
// These adapters create a port cto from a specific (same or other) port cto.
//
// The created port has only the properties required for that port:
// other properties of the source port are not available via the created port.
//
// These adapters can only be used when the source port is know.
// For general use, the port adapters that accept any (possible) source
// port are more covenient.
//
// @define port_in_from_port_in
// @insert port_in_from_port_in
//
// @define port_in_from_port_in_out
// @insert port_in_from_port_in_out
//
// @define port_in_from_port_oc
// @insert port_in_from_port_oc
//
// @define port_out_from_port_out
// @insert port_out_from_port_out
//
// @define port_out_from_port_in_out
// @insert port_out_from_port_in_out
//
// @define port_out_from_port_oc
// @insert port_out_from_port_oc
//
// @define port_in_out_from_port_in_out
// @insert port_in_out_from_port_in_out
//
// @define port_in_out_from_port_oc
// @insert port_in_out_from_port_oc
//
// @define port_oc_from_port_oc
// @insert port_oc_from_port_oc
//
// =============================================================================


// =============================================================================
//
// port_in
//
// =============================================================================

// @quote port_in_from_port_in 2 ... {};
template< is_port_in T >
struct port_in_from_port_in :
   port_in_root< T::n_pins >,
   inherit_init< T >,
   inherit_read< T >
{};

// @quote port_in_from_port_in_out 2 ... {};
template< is_port_in_out T >
struct port_in_from_port_in_out :
   port_in_root< T::n_pins >,
   inherit_init< T >,
   inherit_read< T >
{

   static GODAFOSS_INLINE void init__(){
      T::direction_set_input();
      T::direction_flush();
   }
   using resources = use_list< T, execute< init__ > >;

};

// @quote port_in_from_port_oc 2 ... {};
template< is_port_oc T >
struct port_in_from_port_oc :
   port_in_root< T::n_pins >,
   inherit_init< T >,
   inherit_read< T >
{

   static GODAFOSS_INLINE void init__(){
      T::write( 0 );
      T::flush();
   }
   using resources = use_list< T, execute< init__ > >;

};


// =============================================================================
//
// port_out
//
// =============================================================================

// @quote port_out_from_port_out 2 ... {};
template< is_port_out T >
struct port_out_from_port_out :
   port_out_root< T::n_pins >,
   inherit_init< T >,
   inherit_write< T >
{};

// @quote port_out_from_port_in_out 2 ... {};
template< is_port_in_out T >
struct port_out_from_port_in_out :
   port_out_root< T::n_pins >,
   inherit_init< T >,
   inherit_write< T >
{

   static GODAFOSS_INLINE void init__(){
      T::direction_set_output();
      T::direction_flush();
   }
   using resources = use_list< T, execute< init__ > >;

};

// @quote port_out_from_port_oc 2 ... {};
template< is_port_oc T >
struct port_out_from_port_oc :
   port_out_root< T::n_pins >,
   inherit_init< T >,
   inherit_write< T >
{};


// =============================================================================
//
// port_in_out
//
// =============================================================================

// @quote port_in_out_from_port_in_out 2 ... {};
template< is_port_in_out T >
struct port_in_out_from_port_in_out :
   port_in_out_root< T::n_pins >,
   inherit_init< T >,
   inherit_direction< T >,
   inherit_write< T >,
   inherit_read< T >
{};

// @quote port_in_out_from_port_oc 2 ... {};
template< is_port_oc T >
struct port_in_out_from_port_oc :
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


// =============================================================================
//
// port_oc
//
// =============================================================================

// @quote port_oc_from_port_oc 2 ... {};
template< is_port_oc T >
struct port_oc_from_port_oc :
   port_oc_root< T::n_pins >,
   inherit_init< T >,
   inherit_write< T >,
   inherit_read< T >
{};
