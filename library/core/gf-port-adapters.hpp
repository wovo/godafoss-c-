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
concept bool can_port_in_out =  
      is_port_in_out< T >
   || is_port_oc< T >;

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
   can_pin_in_out_list< Ts...> 
   || sizeof...( Ts ) == 1 && ( can_port_in_out< Ts > && ... )
struct port_in_out;

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
   can_pin_oc_list< Ts...> 
   || sizeof...( Ts ) == 1 && ( can_port_oc< Ts > && ... )
struct port_oc;


// ==========================================================================
//
// adapters
//
// ==========================================================================

template< is_port_in_out T > 
struct port_in_out< T > :
   be_port_in_out< T::n_pins >,
   box_init_filter< T >,
   box_direction_filter< T >,
   box_write_filter< T >,
   box_read_filter< T >
{};   

template< is_port_oc T >
struct pin_in_out< T > : 
   be_pin_out< bool >,
   box_init_filter< T >,
   box_write_filter< T >,
   box_read_filter< T >
{   

   static GODAFOSS_INLINE void direction_set_input(){
      invert< T >::write( 0 );
   }

   static GODAFOSS_INLINE void direction_set_output(){}

   static GODAFOSS_INLINE void direction_set_flush(){}

};

// ==========================================================================

template< is_port_in_out T > 
struct port_out< T > :
   be_port_out< T::n_pins >,
   box_init_filter< T >,
   box_write_filter< T >
{

   static GODAFOSS_INLINE void init(){
      T::init();
      direct< T >::direction_set_output();
   }

};   

template< is_port_out T > 
struct port_out< T > :
   be_port_out< T::n_pins >,
   box_init_filter< T >,
   box_write_filter< T >
{};   

template< is_port_oc T > 
struct port_out< T > :
   be_port_out< T::n_pins >,
   box_init_filter< T >,
   box_write_filter< T >
{};   

// ==========================================================================

template< is_port_in_out T > 
struct port_in< T > :
   be_port_in< T::n_pins >,
   box_init_filter< T >,
   box_read_filter< T >
{

   static GODAFOSS_INLINE void init(){
      T::init();
      direct< T >::direction_set_input();
   }

};   

template< is_port_in T > 
struct port_in< T > :
   be_port_in< T::n_pins >,
   box_init_filter< T >,
   box_read_filter< T >
{};   

template< is_port_oc T > 
struct port_in< T > :
   be_port_in< T::n_pins >,
   box_init_filter< T >,
   box_read_filter< T >
{

   static GODAFOSS_INLINE void init(){
      T::init();
      direct< invert< T >>::write( 0 );
   }

};   

// ==========================================================================

template< is_port_oc T > 
struct port_oc< T > :
   be_port_oc< T::n_pins >,
   box_init_filter< T >,
   box_write_filter< T >,
   box_read_filter< T >
{};