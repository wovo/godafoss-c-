// ==========================================================================
//
// gf-box-adapters.hpp
//
// ==========================================================================
//
// These adapter adapts a box to be (only) an input, 
// output or input_output box (if possible).
//
// ==========================================================================
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
// ==========================================================================


// ==========================================================================
//
// concepts
//
// ==========================================================================

template< typename T >
concept can_output =  
      is_output< T >
   || is_input_output< T >;

template< typename T >
concept can_input =  
      is_input< T >
   || is_input_output< T >;

template< typename T >
concept an_input_output =  
     is_input_output< T >;
	 
	 
// ==========================================================================
//
// adapt to an output box
//
// ==========================================================================

template< can_output T > struct box_out;

template< is_output T >
struct box_out< T > :
   box_inherit_init< T >,
   box_inherit_write< T >	
{};	

template< is_input_output T >
struct box_out< T > :
   box_inherit_write< T >
{ 

   static void init(){
      T::init(); 
      direct< T >::direction_set_output();
   }	

};


// ==========================================================================
//
// adapt to be an input box
//
// ==========================================================================

template< can_input T > struct box_in;

template< is_input T >
struct box_in< T > :
   box_inherit_init< T >, 	
   box_inherit_read< T >		
{};	

template< is_input_output T >
struct box_in< T > : 
   box_inherit_read< T > 
{

   static void GODAFOSS_INLINE init(){
      T::init(); 
      direct< T >::direction_set_input();
   }	

};


// ==========================================================================
//
// adapt to be an input_output box
//
// ==========================================================================

template< is_input_output T >
struct box_in_out : 
   box_inherit_init< T >, 	
   box_inherit_read< T >, 	
   box_inherit_write< T >,	
   box_inherit_direction< T > 	
{};
