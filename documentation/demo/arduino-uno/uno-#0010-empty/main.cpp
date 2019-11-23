// ==========================================================================
//
// empty project
//
// (c) Wouter van Ooijen (wouter@voti.nl) 2017
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt) 
//
// ==========================================================================

// #include "godafoss.hpp"

#define GODAFOSS_INLINE

// quote ''box'' };
template< typename T >
concept bool is_box = requires {  
   T::is_box;
   T::is_item || T::is_stream; 
   { T::init() } -> void;
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
   static const bool is_item   = true;	
   static const bool is_stream = false;	
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
   static const bool is_item   = false;	
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



// ==========================================================================
//
// in_out
//
// ==========================================================================

// quote ''pin_in_out'' );
template< typename T >
concept bool is_pin_in_out = requires {
   is_item< bool >;
   is_simplex< bool >;
   T::is_pin_in_out;
};

struct be_pin_in_out : 
   be_item< bool >, 
   be_simplex< bool > 
{
   static constexpr bool is_pin_in_out  = true;	
   static constexpr bool is_pin_out     = false;	
   static constexpr bool is_pin_in      = false;	
   static constexpr bool is_pin_oc      = false;	
};


// ==========================================================================
//
// out
//
// ==========================================================================

// quote ''pin_out'' );
template< typename T >
concept bool is_pin_out = requires {
   is_item< bool >;
   is_output< bool >;
   T::is_pin_out;
};

struct be_pin_out : 
   be_item< bool >, 
   be_output< bool >
{
   static constexpr bool is_pin_in_out  = false;	
   static constexpr bool is_pin_out     = true;	
   static constexpr bool is_pin_in      = false;	
   static constexpr bool is_pin_oc      = false;	
};


// ==========================================================================
//
// in
//
// ==========================================================================

// quote ''pin_in'' );
template< typename T >
concept bool is_pin_in = requires {
   is_item< bool >;
   is_input< bool >;
   T::is_pin_in;
};

struct be_pin_in : 
   be_item< bool >, 
   be_input< bool > 
{
   static constexpr bool is_pin_in_out  = false;	
   static constexpr bool is_pin_out     = false;	
   static constexpr bool is_pin_in      = true;	
   static constexpr bool is_pin_oc      = false;	
};


// ==========================================================================
//
// oc
//
// ==========================================================================

// quote ''pin_oc'' );
template< typename T >
concept bool is_pin_oc = requires {
//   is_item< bool >;
//   is_duplex< bool >;
   T::is_pin_oc;
};

struct be_pin_oc : 
   be_item< bool >, 
   be_duplex< bool > 
{
   static constexpr bool is_pin_in_out  = false;	
   static constexpr bool is_pin_out     = false;	
   static constexpr bool is_pin_in      = false;	
   static constexpr bool is_pin_oc      = true;	
};


// quote ''can_pin_out'' );
template< typename T >
concept bool can_pin_out = 
     is_pin_in_out< T > 
  || is_pin_out< T > 
  || is_pin_oc< T >;

// ========== base case : let box do the work 

template< can_pin_out T > 
struct pin_out {};

template< is_pin_in_out T >
struct pin_out< T > {};

int main( void ){}