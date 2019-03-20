// ==========================================================================
//
// gf-pin-adapters.hpp
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

// quote ''can_pin_in_out'' );
template< typename T >
concept bool can_pin_in_out = requires {
      is_pin_in_out< T > 
   || is_pin_oc< T >;
};

// quote ''can_pin_in'' );
template< typename T >
concept bool can_pin_in = requires {
      is_pin_in< T > 
   || is_pin_in_out< T > 
   || is_pin_oc< T >;
};

// quote ''can_pin_out'' );
template< typename T >
concept bool can_pin_out = requires {
      is_pin_out< T > 
   || is_pin_oc< T >;
};

// quote ''can_pin_oc'' );
template< typename T >
concept bool can_pin_oc = requires {
      is_pin_in_out< T > 
   || is_pin_oc< T >;
};


// ==========================================================================
//
// in_out
//
// ==========================================================================

template< typename T > struct pin_in_out;

template< is_pin_in_out T > 
struct pin_in_out< T > :
   be_pin_out< bool >,
   box_init_filter< T >,
   box_write_filter< T >
{};   

template< is_pin_oc T >
struct pin_in_out< T > : 
   be_pin_out< bool >,
   box_init_filter< T >,
   box_write_filter< T >  
{};	


// ==========================================================================
//
// out
//
// ==========================================================================

template< typename T > struct pin_out;

template< is_pin_in_out T >
struct pin_out< T > : 
   be_pin_out< bool >,
   box_write_filter< T >
{
   
   static void HWCPP_INLINE init(){
      T::init();
      direct< T >::direction_set_output();
   }  
   
};

template< is_pin_out T >
struct pin_out< T > : 
   be_pin_out< bool >,
   box_init_filter< T >,
   box_write_filter< T >
{};

template< is_pin_oc T >
struct pin_out< T > : 
   be_pin_out< bool >,
   box_init_filter< T >,
   box_write_filter< T >
{};


// ==========================================================================
//
// in
//
// ==========================================================================

template< typename T > struct pin_in;

template< is_pin_in_out T >
struct pin_in< T > : 
   be_pin_in< bool >,
   box_read_filter< T >
{
	
   static void HWCPP_INLINE init(){
      T::init();
      direct< T >::direction_set_input();
   }
   
};

template< is_pin_in T >
struct pin_in< T > : 
   be_pin_in< bool >,
   box_init_filter< T >,
   box_read_filter< T >
{};

template< is_pin_oc T >
struct pin_in< T > : 
   be_pin_in< bool >,
   box_read_filter< T >
{
	
   static void HWCPP_INLINE init(){
      T::init();
      direct< T >::write( 1 );
   }
   
};


// ==========================================================================
//
// oc
//
// ==========================================================================

template< typename T > struct pin_oc;

template< is_pin_oc T >
struct pin_oc< T > : 
   be_pin_oc< bool >,
   box_init_filter< T >,
   box_write_filter< T >,
   box_read_filter< T >
{};