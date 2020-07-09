// =============================================================================
//
// gf-ints.hpp
//
// =============================================================================
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
// =============================================================================
//
// @title ints specified by number of bits
//
// @define godafoss::uint_bits
//
// uint_bits< N >::fast is the smallest 'fast'
// unsigned integer type that stores (at least) N bits.
//
// uint_bits< N >::least is the smallest (but not necesarrily fast)
// unsigned integer type that stores (at least) N bits.
//
// As both are unsigned they should be used for bit patterns, not for amounts.
//
// Note that both can be larger than requested, so they should not be used
// for modulo arithmetic (at least not without masking out excess bits).
//
// Use uint_bits< N >::fast for parameters,
// use uint_bits< N >::least for array storage.
//
// =============================================================================


// if no exact match, get something bigger
template< uint64_t n > struct _uint_bits_fast {
   typedef typename _uint_bits_fast< n + 1 >::type type;
};

// if no exact match, get something bigger
template< uint64_t n > struct _uint_bits_least {
   typedef typename _uint_bits_least< n + 1 >::type type;
};

template< uint64_t n > struct uint_bits {

   // nothing bigger available
   static_assert(
      n <= 8 * sizeof( uint_fast64_t ),
      "no unsigned integer type is large enough"
   );

   // try 1 bit bigger
   typedef typename _uint_bits_fast< n >::type fast;
   typedef typename _uint_bits_least< n >::type least;
};


// =============================================================================

template<> struct _uint_bits_fast< 8 * sizeof( uint_fast8_t ) > {
   typedef uint_fast8_t type;
};

#if UINT_FAST16_MAX > UINT_FAST8_MAX
template<> struct _uint_bits_fast< 8 * sizeof( uint_fast16_t ) > {
   typedef uint_fast16_t type;
};
#endif

#if UINT_FAST32_MAX > UINT_FAST16_MAX
template<> struct _uint_bits_fast< 8 * sizeof( uint_fast32_t ) > {
   typedef uint_fast32_t type;
};
#endif

#if UINT_FAST64_MAX > UINT_FAST32_MAX
template<> struct _uint_bits_fast< 8 * sizeof( uint_fast64_t ) > {
   typedef uint_fast64_t type;
};
#endif

// =============================================================================

template<> struct _uint_bits_least< 8 * sizeof( uint_least8_t ) > {
   typedef uint_least8_t type;
};

#if UINT_LEAST16_MAX > UINT_LEAST8_MAX
template<> struct _uint_bits_least< 8 * sizeof( uint_least16_t ) > {
   typedef uint_least16_t type;
};
#endif

#if UINT_LEAST32_MAX > UINT_LEAST16_MAX
template<> struct _uint_bits_least< 8 * sizeof( uint_least32_t ) > {
   typedef uint_least32_t type;
};
#endif

#if UINT_LEAST64_MAX > UINT_LEAST32_MAX
template<> struct _uint_bits_least< 8 * sizeof( uint_least64_t ) > {
   typedef uint_least64_t type;
};
#endif
