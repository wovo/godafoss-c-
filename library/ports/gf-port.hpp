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
// root
//
// ==========================================================================

template< typename T, int n >
concept port = requires {
   box< typename uint_bits< n >::fast >;
   T::port_marker;
};

template< int n >
struct port_root :
   box_root< typename uint_bits< n >::fast >
{
   static const bool port_marker  = true;
   static const int  n_pins       = n;

   using _vt = typename box_root< typename uint_bits< n >::fast >::value_type;

   GODAFOSS_INLINE static _vt invert( _vt v ){
      return ~v;
   }

};


// ==========================================================================
//
// in out
//
// ==========================================================================

template< typename T >
concept port_in_out = requires {
   port< T, T::n_pins >;
   simplex< typename T::value_type >;
   T::port_in_out_marker;
};

template< int n >
struct port_in_out_root :
   port_root< n >,
   input_output_root< typename uint_bits< n >::fast >
{
   static constexpr bool port_in_out_marker = true;
};


// ==========================================================================
//
// out
//
// ==========================================================================

template< typename T >
concept port_out = requires {
   port< T, T::n_pins >;
   output< typename T::value_type >;
   T::port_out_marker;
};

template< int n >
struct port_out_root :
   port_root< n >,
   output_root< typename uint_bits< n >::fast >
{
   static constexpr bool port_out_marker = true;
};


// ==========================================================================
//
// in
//
// ==========================================================================

template< typename T >
concept port_in = requires {
   port< T, T::n_pins >;
   input< typename T::value_type >;
   T::port_in_marker;
};

template< int n >
struct port_in_root :
   port_root< n >,
   input_root< typename uint_bits< n >::fast >
{
   static constexpr bool port_marker = true;
};


// ==========================================================================
//
// oc
//
// ==========================================================================

template< typename T >
concept port_oc = requires {
   port< T, T::n_pins >;
   duplex< typename T::value_type >;
   T::port_oc_marker;
};

template< int n >
struct port_oc_root :
   port_root< n >,
   duplex_root< typename uint_bits< n >::fast >
{
   static constexpr bool port_oc_marker = true;
};
