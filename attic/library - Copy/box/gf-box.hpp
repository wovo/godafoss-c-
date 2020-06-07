// ==========================================================================
//
// gf-box.hpp
// 
// ==========================================================================
//
// the basic interface properties:
//    - box: holds some data elements(s))
//    - item: box that always holds one element of the data
//    - stream: box that holds a sequence of data elements
//    - input: box that supports read()
//    - output: box that supports write()
//    - input_output: both input and output
//    - duplex: both input and output at the same time
//    - simplex: both input and output, but only one at a time
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
// box
//
// holds some data elements(s))
//
// ==========================================================================

// quote ''box'' };
template< typename T >
concept is_box = requires {  
   T::is_box;
   T::is_item  || T::is_stream; 
   T::is_input || T::is_output; 
   { T::init() } -> std::same_as< void >;
};

template< typename T >
struct be_box_root {
   static const bool is_box = true;	
   using value_type = T;
};

template< typename T >
struct be_box : be_box_root< T > {};

template<>
struct be_box< bool > :
   be_box_root< bool >
{
   GODAFOSS_INLINE static bool invert( bool v ){
      return !v;	   
   }
};


// ==========================================================================
//
// item 
//
// box that always holds one element of the data
//
// ==========================================================================

// quote ''item'' );
template< typename T >
concept is_item = requires {
   is_box< T >;
   T::is_item;
};

template< typename T >
struct be_item : 
   be_box< T > 
{
   static const bool is_item   = true;	
   static const bool is_stream = false;	
};


// ==========================================================================
//
// stream
//
// box that holds a sequence of data elements
//
// ==========================================================================

// quote ''stream'' );
template< typename T >
concept is_stream = requires {
   is_box< T >;
   T::is_stream;
};

template< typename T >
struct be_stream : 
   be_box< T > 
{
   static const bool is_item   = false;	
   static const bool is_stream = true;	
};


// ==========================================================================
//
// input
//
// box that supports read()
//
// ==========================================================================

// quote ''input'' };
template< typename T >
concept is_input = requires {
   is_box< T >;
   T::is_input;
   { T::refresh() }  -> std::same_as< void >;
   { T::read() }     -> std::same_as< typename T::value_type >;
};

template< typename T >
struct be_input : 
   be_box< T > 
{
   static const bool is_input = true;	
};


// ==========================================================================
//
// output
//
// box that supports write()
//
// ==========================================================================

// quote ''output'' };
template< typename T >
concept is_output = requires ( 
   typename T::value_type v
){
   is_box< T >;
   T::is_output;
   { T::write( v ) }  -> std::same_as< void >;
   { T::flush()    }  -> std::same_as< void >;
};

template< typename T >
struct be_output : 
   be_box< T > 
{
   static const bool is_output = true;	
};


// ==========================================================================
//
// input_output
//
// both input and output
//
// ==========================================================================

// quote ''input_output'' );
template< typename T >
concept is_input_output = requires {
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
// duplex
//
// both input and output, at the same time
//
// ==========================================================================

// quote ''duplex'' );
template< typename T >
concept is_duplex = requires {
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
//
// simplex
//
// both input and output, but only one at a time
//
// ==========================================================================

// quote ''simplex'' };
template< typename T >
concept is_simplex = requires ( 
){
   is_input_output< T >;
   T::is_simplex;
   { T::direction_set_input() }   -> std::same_as< void >;
   { T::direction_set_output() }  -> std::same_as< void >;
   { T::direction_flush() }       -> std::same_as< void >;
};

template< typename T >
struct be_simplex : be_input_output< T > {
   static const bool is_simplex = true;	
};

