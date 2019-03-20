// ==========================================================================
//
// basics.hpp
// 
// the basic stuff of the library
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
// PUBLIC
//
// analog input
//
// ==========================================================================

// quote ''adc'' };
template< typename T >
concept bool adc = requires( 
   T::is_adc,
   item< T >,
   input< T >,
   constexpr T::value_type adc_min,
   constexpr T::value_type adc_max
);


// ==========================================================================
//
// PUBLIC
//
// analog output
//
// ==========================================================================

// quote ''dac'' };
template< typename T >
concept bool dac = requires( 
   T::is_dac,
   item< T >,
   output< T >,
   constexpr T::value_type dac_min,
   constexpr T::value_type dac_max
);


