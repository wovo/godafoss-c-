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
concept box = requires {  
   T::_box_marker;
   { T::init() } -> std::same_as< void >;
};

template< typename T >
struct _box_root {
   static const bool _box_marker = true;	
   using value_type = T;
};

template< typename T >
struct box_root : _box_root< T > {};

// box< bool > has a different invert than other boxes
template<>
struct box_root< bool > :
   _box_root< bool >
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
concept item = requires {
   box< T >;
   T::_item_marker;
};

template< typename T >
struct item_root : 
   box_root< T > 
{
   static const bool _item_marker = true;	
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
concept stream = requires {
   box< T >;
   T::_stream_marker;
};

template< typename T >
struct stream_root : 
   box_root< T > 
{
   static const bool _stream_marker = true;	
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
concept input = requires {
   box< T >;
   T::_input_marker;
   { T::refresh() }  -> std::same_as< void >;
   { T::read() }     -> std::same_as< typename T::value_type >;
};

template< typename T >
struct input_root : 
   box_root< T > 
{
   static const bool _input_marker = true;	
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
concept output = requires ( 
   typename T::value_type v
){
   box< T >;
   T::_output_marker;
   { T::write( v ) }  -> std::same_as< void >;
   { T::flush()    }  -> std::same_as< void >;
};

template< typename T >
struct output_root : 
   box_root< T > 
{
   static const bool _output_marker = true;	
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
concept input_output = requires {
   input< T >;
   output< T >;
};

template< typename T >
struct input_output_root : 
   input_root< T >, 
   output_root< T > 
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
concept duplex = requires {
   input_output< T >;
   T::_duplex_marker;
};

template< typename T >
struct duplex_root : 
   input_output_root< T > 
{
   static const bool _duplex_marker = true;	
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
concept simplex = requires ( 
){
   input_output< T >;
   T::_simplex_marker;
   { T::direction_set_input() }   -> std::same_as< void >;
   { T::direction_set_output() }  -> std::same_as< void >;
   { T::direction_flush() }       -> std::same_as< void >;
};

template< typename T >
struct simplex_root : input_output_root< T > {
   static const bool _simplex_marker = true;	
};
