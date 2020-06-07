// ==========================================================================
//
// gf-analog.hpp
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
// analog input
//
// ==========================================================================

// quote ''adc'' };
template< typename T >
concept adc = requires {
   T::is_adc;
   is_item< T >;
   is_input< T >;
//   constexpr T::value_type adc_min;
//   constexpr T::value_type adc_max;
};

template< int n_bits >
struct be_adc :
   be_item< typename uint_bits< n_bits >::fast >,
   be_input< typename uint_bits< n_bits >::fast >
{
   static const bool is_adc = true;
   using value_type = typename uint_bits< n_bits >::fast;
   static const value_type adc_min = 0;
   static const value_type adc_max = ( 1 << n_bits ) -1;
};


// ==========================================================================
//
// analog output
//
// ==========================================================================

// quote ''dac'' };
/*
template< typename T >
concept dac = requires( 
   T::is_dac,
   item< T >,
   output< T >,
   constexpr T::value_type dac_min,
   constexpr T::value_type dac_max
);
*/

