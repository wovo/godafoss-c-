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
// concepts
//
// ==========================================================================

template< typename T >
concept bool can_port_out =  
      is_port_out< T >
   || is_port_in_out< T >
   || is_port_oc< T >;

template< typename T >
concept bool can_port_in =  
      is_port_in< T >
   || is_port_in_out< T >
   || is_port_oc< T >;

template< typename T >
concept bool can_port_in_out =  
      is_port_in_out< T >
   || is_port_oc< T >;

template< typename T >
concept bool can_port_oc =  
   // a port_in_out is NOT acceptable
   is_port_oc< T >;

   
// ==========================================================================
//
// lists of pins
//
// ==========================================================================

template< typename... Ts >
concept bool can_pin_out_list = ( can_pin_out< Ts > && ... );

template< typename... Ts >
concept bool can_pin_in_list = ( can_pin_in< Ts > && ... );

template< typename... Ts >
concept bool can_pin_in_out_list = ( can_pin_in_out< Ts > && ... );

template< typename... Ts >
concept bool can_pin_oc_list = ( can_pin_oc< Ts > && ... );


// ==========================================================================
//
// fallbacks
//
// ==========================================================================

template< typename... Ts >
requires 
   can_pin_out_list< Ts...> 
   || sizeof...( Ts ) == 1 && ( can_port_out< Ts > && ... )
struct port_out;

template< typename... Ts >
requires 
   can_pin_in_list< Ts...> 
   || sizeof...( Ts ) == 1 && ( can_port_in< Ts > && ... )
struct port_in;

template< typename... Ts >
requires 
   can_pin_in_out_list< Ts...> 
   || sizeof...( Ts ) == 1 && ( can_port_in_out< Ts > && ... )
struct port_in_out;

template< typename... Ts >
requires 
   can_pin_oc_list< Ts...> 
   || sizeof...( Ts ) == 1 && ( can_port_oc< Ts > && ... )
struct port_oc;

