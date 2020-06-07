// ==========================================================================
//
// gf_uart.hpp
//
// ==========================================================================
//
// This file is part of godafoss, 
// a C++ library for close-to-the-hardware programming.
//
// Copyright Wouter van Ooijen 2018
// 
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================


// ==========================================================================
//
// concept
//
// ==========================================================================

template< typename T >
concept is_uart = requires {
   is_stream< T >;
   is_duplex< T >;
   std::same_as< typename T::value_type, char >;
   T::is_uart;
};


// ==========================================================================
//
// builder
//
// ==========================================================================

template< typename T >
concept is_hardware_uart = requires (
   char c
) {
   { T::write_blocks() }        -> std::same_as< bool >;
   { T::write_unchecked( c ) }  -> std::same_as< void >;
   { T::read_blocks() }         -> std::same_as< bool >;
   { T::read_unchecked() }      -> std::same_as< char >;
};

template< typename T >
struct be_uart : 
   be_stream< char >,
   be_duplex< char >
{
   
   static const bool is_uart = true;
   
   static bool GODAFOSS_INLINE write_blocks(){
      return T::write_blocks();	   
   }	
   
   static void write( char c ){
	  while( T::write_blocks() ){}	
      T::write_unchecked( c );	   
   }
   
   static void GODAFOSS_INLINE flush(){}	
   
   static bool GODAFOSS_INLINE read_blocks(){
      return T::read_blocks();	   
   }	
   
   static void read(){
	  while( T::read_blocks() ){}	
      return T::read_unchekced();	   
   }	
   
   static void GODAFOSS_INLINE refresh(){}	
   
};
