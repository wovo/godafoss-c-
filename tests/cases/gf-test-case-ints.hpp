// =============================================================================
//
// gf-test-case-ints.hpp
//
// =============================================================================
//
// Tests of gf::uint_bits< x >
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


#define INT_LINE( type, max )                                    \
   << gf::setw( 14 ) << gf::left << #type << " = "               \
   << gf::setw( 2 ) << 8 * sizeof( type )                        \
   << " bits  (" << max << ")\n"

void test_case_ints(){

   static gf::ostream< gf::formatter< uart > > cout;

   if( 0 ) { cout
      INT_LINE( uint_fast8_t,  UINT_FAST8_MAX )
      INT_LINE( uint_fast16_t, UINT_FAST16_MAX )
      INT_LINE( uint_fast32_t, UINT_FAST32_MAX )
      INT_LINE( uint_fast64_t, UINT_FAST64_MAX )
      << "\n"
      INT_LINE( uint_least8_t,  UINT_LEAST8_MAX )
      INT_LINE( uint_least16_t, UINT_LEAST16_MAX )
      INT_LINE( uint_least32_t, UINT_LEAST32_MAX )
      INT_LINE( uint_least64_t, UINT_LEAST64_MAX );
   }

   REQUIRE( ( std::same_as< gf::uint_bits< 1 >::fast, std::uint_fast8_t > ) );
   REQUIRE( ( std::same_as< gf::uint_bits< 8 >::fast, std::uint_fast8_t > ) );

   int tests = 0;

   #if UINT_FAST16_MAX > UINT_FAST8_MAX
      ++tests;
      REQUIRE( ( std::same_as< gf::uint_bits< 8 * sizeof( std::uint_fast8_t )     >::fast, std::uint_fast8_t > ) );
      REQUIRE( ( std::same_as< gf::uint_bits< 8 * sizeof( std::uint_fast8_t ) + 1 >::fast, std::uint_fast16_t > ) );
      REQUIRE( ( std::same_as< gf::uint_bits< 8 * sizeof( std::uint_fast16_t )    >::fast, std::uint_fast16_t > ) );
   #endif

   #if UINT_FAST32_MAX > UINT_FAST16_MAX
      ++tests;
      REQUIRE( ( std::same_as< gf::uint_bits< 8 * sizeof( std::uint_fast16_t )     >::fast, std::uint_fast16_t > ) );
      REQUIRE( ( std::same_as< gf::uint_bits< 8 * sizeof( std::uint_fast16_t ) + 1 >::fast, std::uint_fast32_t > ) );
      REQUIRE( ( std::same_as< gf::uint_bits< 8 * sizeof( std::uint_fast32_t )     >::fast, std::uint_fast32_t > ) );
   #endif

   #if UINT_FAST64_MAX > UINT_FAST32_MAX
      ++tests;
      REQUIRE( ( std::same_as< gf::uint_bits< 8 * sizeof( std::uint_fast32_t )     >::fast, std::uint_fast32_t > ) );
      REQUIRE( ( std::same_as< gf::uint_bits< 8 * sizeof( std::uint_fast32_t ) + 1 >::fast, std::uint_fast64_t > ) );
      REQUIRE( ( std::same_as< gf::uint_bits< 8 * sizeof( std::uint_fast64_t )     >::fast, std::uint_fast64_t > ) );
   #endif

   REQUIRE( tests >= 1 );

   REQUIRE( ( std::same_as< gf::uint_bits< 1 >::least, std::uint_least8_t > ) );
   REQUIRE( ( std::same_as< gf::uint_bits< 8 >::least, std::uint_least8_t > ) );

   tests = 0;

   #if UINT_LEAST16_MAX > UINT_LEAST8_MAX
      ++tests;
      REQUIRE( ( std::same_as< gf::uint_bits< 8 * sizeof( std::uint_least8_t )     >::least, std::uint_least8_t > ) );
      REQUIRE( ( std::same_as< gf::uint_bits< 8 * sizeof( std::uint_least8_t ) + 1 >::least, std::uint_least16_t > ) );
      REQUIRE( ( std::same_as< gf::uint_bits< 8 * sizeof( std::uint_least16_t )    >::least, std::uint_least16_t > ) );
   #endif

   #if UINT_LEAST32_MAX > UINT_LEAST16_MAX
      ++tests;
      REQUIRE( ( std::same_as< gf::uint_bits< 8 * sizeof( std::uint_least16_t )     >::least, std::uint_least16_t > ) );
      REQUIRE( ( std::same_as< gf::uint_bits< 8 * sizeof( std::uint_least16_t ) + 1 >::least, std::uint_least32_t > ) );
      REQUIRE( ( std::same_as< gf::uint_bits< 8 * sizeof( std::uint_least32_t )     >::least, std::uint_least32_t > ) );
   #endif

   #if UINT_LEAST64_MAX > UINT_LEAST32_MAX
      ++tests;
      REQUIRE( ( std::same_as< gf::uint_bits< 8 * sizeof( std::uint_least32_t )     >::least, std::uint_least32_t > ) );
      REQUIRE( ( std::same_as< gf::uint_bits< 8 * sizeof( std::uint_least32_t ) + 1 >::least, std::uint_least64_t > ) );
      REQUIRE( ( std::same_as< gf::uint_bits< 8 * sizeof( std::uint_least64_t )     >::least, std::uint_least64_t > ) );
   #endif

   REQUIRE( tests >= 3 );

}
