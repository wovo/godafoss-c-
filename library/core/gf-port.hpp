// ==========================================================================
//
// gf-port.hpp
//
// ==========================================================================
//
// This file is part the https://www.github.com/godafoss 
// free C++ library for close-to-the-hardware programming.
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
// base
//
// ==========================================================================

template< typename T, int n >
concept bool is_port_base = requires {
   is_item< typename uint_t< n >::fast >;	
   T::is_port_base;
};

template< int n >
struct be_port_base : 
   be_item< typename uint_t< n >::fast >
{
   static const bool is_port_base  = true;
   static const int  n_pins        = n;
};


// ==========================================================================
//
// in out
//
// ==========================================================================

template< typename T >
concept bool is_port_in_out = requires { 
   is_port_base< T, T::n_pins >;
   is_simplex< T::value_type >;
   T::is_port_in_out;   
};

template< int n >
struct be_port_in_out :
   be_port_base< n >,
   be_input_output< typename uint_t< n >::fast >
{
   static constexpr bool is_port_in_out = true;
};


// ==========================================================================
//
// out
//
// ==========================================================================

template< typename T >
concept bool is_port_out = requires {
   is_port_base< T, T::n_pins >;
   is_output< T::value_type >;
   T::is_port_out;
};

template< int n >
struct be_port_out :
   be_port_base< n >,
   be_output< typename uint_t< n >::fast >
{
   static constexpr bool is_port_out = true;
};


// ==========================================================================
//
// in
//
// ==========================================================================

template< typename T >
concept bool is_port_in = requires {
   is_port_base< T, T::n_pins >;
   is_input< T::value_type >;
   T::is_port_in;
};

template< int n >
struct be_port_in :
   be_port_base< n >,
   be_input< typename uint_t< n >::fast >
{
   static constexpr bool is_port_in = true;
};


// ==========================================================================
//
// oc
//
// ==========================================================================

template< typename T >
concept bool is_port_oc = requires {
   is_port_base< T, T::n_pins >;
   is_duplex< T::value_type >;
   T::is_port_oc;
};

template< int n >
struct be_port_oc :
   be_port_base< n >,
   be_duplex< typename uint_t< n >::fast >
{
   static constexpr bool is_port_oc = true;
};
