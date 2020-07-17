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
// concept and root
//
// ==========================================================================

template< typename T, int n >
concept is_port = requires {
      T::_port_marker;
   }
   && is_box< T, typename uint_bits< n >::fast >
   && T::n_pins == n;

template< int n >
struct port_root :
   box_root< typename uint_bits< n >::fast >
{
   static const bool _port_marker  = true;
   static const int  n_pins        = n;

   using _vt = typename box_root< typename uint_bits< n >::fast >::value_type;

   GODAFOSS_INLINE static _vt invert( _vt v ){
      return ~v;
   }

};


// ==========================================================================
//
// in
//
// ==========================================================================

template< typename T >
concept is_port_in = requires {
      T::_port_in_marker;
   }
   && is_port< T, T::n_pins >
   && is_input< T >;

template< int n >
struct port_in_root :
   port_root< n >,
   input_root< typename uint_bits< n >::fast >
{
   static constexpr bool _port_marker = true;
};



// ==========================================================================
//
// out
//
// ==========================================================================

template< typename T >
concept is_port_out = requires {
      T::_port_out_marker;
   }
   && is_port< T, T::n_pins >
   && is_output< T >;

template< int n >
struct port_out_root :
   port_root< n >,
   output_root< typename uint_bits< n >::fast >
{
   static constexpr bool _port_out_marker = true;
};


// ==========================================================================
//
// in out
//
// ==========================================================================

template< typename T >
concept is_port_in_out = requires {
      T::_port_in_out_marker;
   }
   && is_port< T, T::n_pins >
   && is_simplex< T >
;

template< int n >
struct port_in_out_root :
   port_root< n >,
   simplex_root< typename uint_bits< n >::fast >
{
   static constexpr bool _port_in_out_marker = true;
};


// ==========================================================================
//
// oc
//
// ==========================================================================

template< typename T >
concept is_port_oc = requires {
      T::_port_oc_marker;
   }
   && is_port< T, T::n_pins >
   && is_duplex< T >;

template< int n >
struct port_oc_root :
   port_root< n >,
   duplex_root< typename uint_bits< n >::fast >
{
   static constexpr bool _port_oc_marker = true;
};
