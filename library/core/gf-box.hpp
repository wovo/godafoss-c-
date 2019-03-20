// ==========================================================================
//
// gf-box.hpp
// 
// the basic stuff of the library
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

#define HWCPP_INLINE
#define _HWCPP_RUN_ONCE


// ==========================================================================
//
// box, item, stream
//
// ==========================================================================

// quote ''box'' };
template< typename T >
concept bool is_box = requires {  
   T::is_box;
   T::is_item || T::is_stream;
   { T::init() } -> void;
};

template< typename T >
struct be_box {
   static const bool is_box = true;	
   using value_type = T;
};

// ==========================================================================

// quote ''item'' );
template< typename T >
concept bool is_item = requires {
   is_box< T >;
   T::is_item;
};

template< typename T >
struct be_item : 
   be_box< T > 
{
   static const bool is_item = true;	
};

// ==========================================================================

// quote ''stream'' );
template< typename T >
concept bool is_stream = requires {
   is_box< T >;
   T::is_stream;
};

template< typename T >
struct be_stream : 
   be_box< T > 
{
   static const bool is_stream = true;	
};


// ==========================================================================
//
// input, output
//
// ==========================================================================

// quote ''input'' };
template< typename T >
concept bool is_input = requires {
   is_box< T >;
   T::is_input;
   { T::refresh() } -> void;
   { T::read() } -> typename T::value_type;
};

template< typename T >
struct be_input : 
   be_box< T > 
{
   static const bool is_input = true;	
};


// ==========================================================================

// quote ''output'' };
template< typename T >
concept bool is_output = requires ( 
   typename T::value_type v
){
   is_box< T >;
   T::is_output;
   { T::write( v ) } -> void;
   { T::flush() } -> void;
};

template< typename T >
struct be_output : 
   be_box< T > 
{
   static const bool is_output = true;	
};


// ==========================================================================

// quote ''input_output'' );
template< typename T >
concept bool is_input_output = requires {
   is_input< T >;
   is_output< T >;
};

template< typename T >
struct be_input_output : 
   be_input< T >, 
   be_output< T > 
{};


// ==========================================================================
//
// simplex, duplex
//
// ==========================================================================

// quote ''duplex'' );
template< typename T >
concept bool is_duplex = requires {
   is_input_output< T >;
   T::is_duplex;
};

template< typename T >
struct be_duplex : 
   be_input_output< T > 
{
   static const bool is_duplex = true;	
};


// ==========================================================================

// quote ''simplex'' };
template< typename T >
concept bool is_simplex = requires ( 
){
   is_input_output< T >;
   T::is_simplex;
   { T::direction_set_input() } -> void;
   { T::direction_set_output() } -> void;
   { T::direction_flush() } -> void;
};

template< typename T >
struct be_simplex : be_input_output< T > {
   static const bool is_simplex = true;	
};

