// ==========================================================================
//
// gf-box-adapters.hpp
//
// ==========================================================================
//
// This file is part of godafoss, 
// a C++ library for close-to-the-hardware programming.
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
concept bool can_output =  
      is_output< T >
   || is_input_output< T >;

template< typename T >
concept bool can_input =  
      is_input< T >
   || is_input_output< T >;

template< typename T >
concept bool can_input_output =  
     is_input_output< T >;
	 
	 
// ==========================================================================
//
// adapt to an out box
//
// ==========================================================================

template< can_output T > struct box_out;

template< is_output T >
struct box_out< T > :
   box_init_filter< T >,
   box_write_filter< T >	
{};	

template< is_input_output T >
struct box_out< T > :
   box_write_filter< T >
{ 
   static void init(){
      T::init(); 
      direct< T >::direction_set_output();
   }	
};


// ==========================================================================
//
// adapt to an in box
//
// ==========================================================================

template< can_input T > struct box_in;

template< is_input T >
struct box_in< T > :
   box_init_filter< T >, 	
   box_read_filter< T >		
{};	

template< is_input_output T >
struct box_in< T > : 
   box_read_filter< T > 
{
   static void GODAFOSS_INLINE init(){
      T::init(); 
      direct< T >::direction_set_input();
   }	
};


// ==========================================================================
//
// adapt to an in out box
//
// ==========================================================================

template< is_input_output T >
struct box_in_out : 
   box_init_filter< T >, 	
   box_read_filter< T >, 	
   box_write_filter< T >,	
   box_direction_filter< T > 	
{};
