// ==========================================================================
//
// gf-no-inline.hpp
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
// box: init
//
// ==========================================================================

template< typename T >
struct _no_inline_init_wrapper : T {};

template< is_box T >
struct _no_inline_init_wrapper< T > : T {
	
   static void init(){ 
      T::init(); 
   }	

};


// ==========================================================================
//
// out
//
// ==========================================================================

template< typename T >
struct _no_inline_box_out_wrapper : T {};

template< is_output T >
struct _no_inline_box_out_wrapper< T > : T {
	
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
struct _no_inline_box_in_wrapper : T {};

template< is_input T >
struct _no_inline_box_in_wrapper< T > : T { 

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
// simplex: direction
//
// ==========================================================================

template< typename T >
struct _no_inline_box_direction_wrapper : T {};

template< is_simplex T >
struct _no_inline_box_direction_wrapper< T > : T { 
    
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
// streams out
//
// ==========================================================================

template< typename T >
struct _no_inline_stream_out_wrapper : T {};

// nothing special yet


// ==========================================================================
//
// streams in
//
// ==========================================================================

template< typename T >
struct _no_inline_stream_in_wrapper : T {};

// nothing special yet


// ==========================================================================
//
// wrapper
//
// ==========================================================================

template< typename T >
using no_inline = 
   _no_inline_init_wrapper<
   _no_inline_box_out_wrapper<
   _no_inline_box_in_wrapper<
   _no_inline_box_direction_wrapper< 
   _no_inline_stream_out_wrapper<
   _no_inline_stream_in_wrapper< T > > > > > >;

