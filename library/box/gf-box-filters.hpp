// ==========================================================================
//
// gf-box-filters.hpp
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
// init
//
// ==========================================================================

template< typename T >
struct box_init_filter {};

template< is_box T >
struct box_init_filter< T > { 
    
   static void GODAFOSS_INLINE init(){ 
      T::init(); 
   }     
}; 


// ==========================================================================
//
// write
//
// ==========================================================================

template< typename T >
struct box_write_filter {};
   
template< is_output T >
struct box_write_filter< T > { 

   using _value_type = typename T::value_type;
    
   static void GODAFOSS_INLINE write( _value_type v ){ 
      T::write( v );  
   }
   
   static void GODAFOSS_INLINE flush(){ 
      T::flush(); 
   }
};    


// ==========================================================================
//
// read
//
// ==========================================================================

template< typename T >
struct box_read_filter {};

template< is_input T >
struct box_read_filter< T > { 

   using _value_type = typename T::value_type;
    
   static _value_type GODAFOSS_INLINE read(){ 
      return T::read(); 
   }
   
   static void GODAFOSS_INLINE refresh(){ 
      T::refresh(); 
   }
}; 


// ==========================================================================
//
// direction
//
// ==========================================================================

template< typename T >
struct box_direction_filter {};
   
template< is_simplex T >
struct box_direction_filter< T > { 
   
   static void GODAFOSS_INLINE direction_set_input(){ 
      T::direction_set_input(); 
   }
   
   static void GODAFOSS_INLINE direction_set_output(){ 
      T::direction_set_output; 
   }
   
   static void GODAFOSS_INLINE direction_flush(){ 
      T::direction_flush(); 
   }
}; 
