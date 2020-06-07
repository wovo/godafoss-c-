// ==========================================================================
//
// gf-box-no-inline.hpp
//
// ==========================================================================
//
// no_inline< T > provides functions that call the corresponding functions
// in T without a forced inlining.
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
// init
//
// ==========================================================================

template< typename T >
struct _no_inline_box_init : T {};

template< is_box T >
struct _no_inline_box_init< T > : T {
	
   static void init(){ 
      T::init(); 
   }	

};


// ==========================================================================
//
// write
//
// ==========================================================================

template< typename T >
struct _no_inline_box_write : T {};

template< is_output T >
struct _no_inline_box_write< T > : T {
	
   using _vt = typename T::value_type;
    
   static void write( _vt v ){ 
      T::write( v ); 
   }
  
   static void flush(){ 
      T::flush(); 
   }	
   
};


// ==========================================================================
//
// in
//
// ==========================================================================

template< typename T >
struct _no_inline_box_read : T {};

template< is_input T >
struct _no_inline_box_read< T > : T { 

   using _vt = typename T::value_type;
    
   static _vt read(){ 
      return T::read(); 
   }
   
   static void refresh(){ 
      T::refresh(); 
   }
   
}; 


// ==========================================================================
//
// direction
//
// ==========================================================================

template< typename T >
struct _no_inline_box_direction : T {};

template< is_simplex T >
struct _no_inline_box_direction< T > : T { 
    
   static void direction_set_output(){ 
      T::direction_set_output(); 
   }

   static void direction_set_input(){ 
      T::direction_set_input();; 
   }
   
   static void direction_flush(){ 
      T::direction_flush(); 
   }
   
}; 


// ==========================================================================
//
// no_inline
//
// ==========================================================================

template< typename T >
using no_inline = 
   _no_inline_box_init<
   _no_inline_box_write<
   _no_inline_box_read<
   _no_inline_box_direction< T > > > >;
