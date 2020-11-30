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
// @define godafoss::uint_bits
// @title integer types specified by number of bits
//
// @insert text
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
// For uint's that are larger than provided by the implementation,
// a primitive large integer ADT is used that has sufficient functionality
// to be used internally in the library, but not more.
//
// Use uint_bits< N >::fast for variables and parameters,
// use uint_bits< N >::least for arrays.
//
// @section example
//
// bla bla
//
// =============================================================================


// =============================================================================
//
// very limited larger unsigned integers
//
// The provided functionality is exactly enough for ports,
// but probably not for anything else.
//
// =============================================================================

template< typename T, int N >
struct big_uint {
private:

   T data[ N ];

public:

   constexpr big_uint(){}

   constexpr big_uint( const big_uint & rhs ){
      *this = rhs;
   }

   constexpr big_uint( uint_fast64_t rhs ){
      for( int i = 0; i < N; ++i ){
         data[ i ] = rhs & std::numeric_limits< T >::max();
         rhs = rhs >> ( 4 * sizeof( T ));
         rhs = rhs >> ( 4 * sizeof( T ));
      }
   }

   constexpr big_uint & operator=( const big_uint & rhs ){
      for( int i = 0; i < N; ++i ){
         data[ i ] = rhs.data[ i ];
      }
      return *this;
   }

   constexpr bool operator==( const big_uint & rhs ) const {
      for( int i = 0; i < N; ++i ){
         if( data[ i ] != rhs.data[ i ] ){
            return false;
         }
      }
      return true;
   }

   constexpr big_uint operator&( const big_uint & rhs ) const {
      big_uint result;
      for( int i = 0; i < N; ++i ){
         result.data[ i ] = data[ i ] & rhs.data[ i ];
      }
      return result;
   }

   constexpr big_uint operator|( const big_uint & rhs ) const {
      big_uint result;
      for( int i = 0; i < N; ++i ){
         result.data[ i ] = data[ i ] | rhs.data[ i ];
      }
      return result;
   }

   constexpr big_uint operator<<( int n ) const {
      big_uint result( *this );
      for( int shifts = 0; shifts < n; ++shifts ){
         T last = 0;
         for( int i = 0; i < N; ++i ){
            T shifted = result.data[ i ] << 1;
            constexpr T mask = ((T) 0x01 ) << ( 8 * sizeof( T ) - 1 );
            if( ( last & mask ) != 0 ){
               shifted |= 0x01;
            }
            last = result.data[ i ];
            result.data[ i ] = shifted;
         }
      }
      return result;
   }

   constexpr big_uint operator>>( int n ) const {
      big_uint result( *this );
      for( int shifts = 0; shifts < n; ++shifts ){
         T last = 0;
         for( int i = N - 1; i >= 0; --i ){
            T shifted = result.data[ i ] << 1;
            constexpr T mask = ((T) 0x01 ) << ( 8 * sizeof( T ) - 1 );
            if( ( last & 0x01 ) != 0 ){
               shifted |= mask;
            }
            last = result.data[ i ];
            result.data[ i ] = shifted;
         }
      }
      return result;
   }

};


// =============================================================================
//
//
// =============================================================================

// if no exact match, get something bigger
template< uint32_t n > struct _uint_bits_fast {
   typedef typename _uint_bits_fast< n + 1 >::type type;
};

// if no exact match, get something bigger
template< uint32_t n > struct _uint_bits_least {
   typedef typename _uint_bits_least< n + 1 >::type type;
};

// @quote text 1
template< uint32_t n > struct uint_bits {

   // nothing bigger available than uint_fast64_t
   static_assert(
      n <= 8 * 8 * sizeof( uint_fast32_t ),
      "no unsigned integer type is large enough"
   );

   // @quote text 1 ...
   typedef typename
      _uint_bits_fast< n >::type
   // @quote text 1
      fast;

   // @quote text 1 ...
   typedef typename
       _uint_bits_least< n >::type
   // @quote text 1
      least;

   // @quote text 1
};


// =============================================================================
//
// _uint_bits_fast<>
//
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

template<> struct _uint_bits_fast< 2 * 8 * sizeof( uint_fast64_t ) > {
   typedef big_uint< uint_fast64_t, 2 >  type;
};

template<> struct _uint_bits_fast< 4 * 8 * sizeof( uint_fast64_t ) > {
   typedef big_uint< uint_fast64_t, 4 >  type;
};


// =============================================================================
//
// _uint_bits_least<>
//
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

template<> struct _uint_bits_least< 2 * 8 * sizeof( uint_fast64_t ) > {
   typedef big_uint< uint_fast64_t, 2 >  type;
};

template<> struct _uint_bits_least< 4 * 8 * sizeof( uint_fast64_t ) > {
   typedef big_uint< uint_fast64_t, 4 >  type;
};
