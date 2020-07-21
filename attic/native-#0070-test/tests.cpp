// =============================================================================
//
// tetst.cpp
//
// =============================================================================
//
// native CATCH2-based tests for the godafoss library
//
// =============================================================================
//
// This file is part of godafoss (https://github.com/wovo/godafoss),
// a C++ library for close-to-the-hardware programming.
//
// Copyright
//    Wouter van Ooijen 2020
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// =============================================================================

#include "godafoss.hpp"
#include "catch.hpp"

#include <iostream>
#include <iomanip>

namespace gf = godafoss;


// =============================================================================
//
// gf-attributes.hpp
//
// =============================================================================

int count1 = 0;
int count2 = 0;

void test_run_once(){
   count1++;
   GODAFOSS_RUN_ONCE;
   count2++;
}

TEST_CASE( "GODAFOSS_RUN_ONCE" ){
   REQUIRE( count1 == 0 );
   REQUIRE( count2 == 0 );
   test_run_once();
   REQUIRE( count1 == 1 );
   REQUIRE( count2 == 1 );
   test_run_once();
   REQUIRE( count1 == 2 );
   REQUIRE( count2 == 1 );
}


// =============================================================================
//
// gf-shared-names.hpp
//
// =============================================================================

GODAFOSS_SUPPORTED( pin, pin_from )

// support for char claimed, but not provided
template< typename A >
   requires std::same_as< A, char >
struct pin_supported< A > {
   static constexpr bool supported = true;
};

// support for cint claimed and provided
template< typename A >
   requires std::same_as< A, int >
struct pin_supported< A > {
   static constexpr bool supported = true;
};

template< typename A >
   requires std::same_as< A, int >
struct pin_from< A > { };

// support for double provided, but not claimed
template< typename A >
   requires std::same_as< A, double >
struct pin_from< A > { };

TEST_CASE( "GODAFOSS_SUPPORTED" ){
   // This should produce the library internal error
   // because support is claimed but not provided
   // (void) pin_from< char >();

   // OK, support is both claimed and provided
   (void) pin_from< int >();

   // This should produce the constraint failure
   // because support is not claimed (and not provided)
   // (void) pin_from< float >();

   // This should also produce the constraint failure
   // because support is not claimed (it is provided, but that shouldn't matter)
   // (void) pin_from< double >();

   // debug test for the test
   //std::cout << valid< pin_from, int > << "\n";
   //std::cout << valid< pin_from, float > << "\n";
   //std::cout << valid< pin_from, double > << "\n";

   REQUIRE( true  == ( valid< pin_from, int > ) );
   REQUIRE( false == ( valid< pin_from, float > ) );
   REQUIRE( false == ( valid< pin_from, double > ) );
}


// =============================================================================
//
// gf-ints.hpp
//
// =============================================================================

#define INT_LINE( type, max )                                      \
   << std::setw( 14 ) << std::left << #type << " = "               \
   << std::setw( 2 ) << 8 * sizeof( type )                         \
   << " bits  (" << max << ")\n"

TEST_CASE( "integers" ){

   if( 0 ) { std::cout
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


// =============================================================================
//
// gf-random.hpp
//
// =============================================================================


// =============================================================================
//
// gf-passing.hpp
//
// =============================================================================

TEST_CASE( "passing" ){
}

// =============================================================================
//
// gf-box.hpp
//
// =============================================================================

struct only_init {
   static void init();
};

template< typename T >
struct box_box : gf::box_root< T >, only_init {};

TEST_CASE( "box" ){
   REQUIRE( false  == ( gf::box< bool > ) );
   REQUIRE( false  == ( gf::box< int > ) );
   REQUIRE( false  == ( gf::box< only_init > ) );
   REQUIRE( true   == ( gf::box< box_box< bool > > ) );
   REQUIRE( true   == ( gf::box< box_box< int > > ) );
}

template< typename T >
struct box_item : gf::item_root< T >, only_init {};

template< typename T >
struct box_stream : gf::stream_root< T >, only_init {};

TEST_CASE( "item" ){
   REQUIRE( false  == ( gf::item< bool > ) );
   REQUIRE( false  == ( gf::item< int > ) );
   REQUIRE( false  == ( gf::item< only_init > ) );
   REQUIRE( false  == ( gf::item< box_box< bool > > ) );
   REQUIRE( false  == ( gf::item< box_box< int > > ) );
   REQUIRE( true   == ( gf::item< box_item< bool > > ) );
   REQUIRE( true   == ( gf::item< box_item< int > > ) );
   REQUIRE( false  == ( gf::item< box_stream< bool > > ) );
   REQUIRE( false  == ( gf::item< box_stream< int > > ) );
}

TEST_CASE( "stream" ){
   REQUIRE( false  == ( gf::stream< bool > ) );
   REQUIRE( false  == ( gf::stream< int > ) );
   REQUIRE( false  == ( gf::stream< only_init > ) );
   REQUIRE( false  == ( gf::stream< box_box< bool > > ) );
   REQUIRE( false  == ( gf::stream< box_box< int > > ) );
   REQUIRE( false  == ( gf::stream< box_item< bool > > ) );
   REQUIRE( false  == ( gf::stream< box_item< int > > ) );
   REQUIRE( true   == ( gf::stream< box_stream< bool > > ) );
   REQUIRE( true   == ( gf::stream< box_stream< int > > ) );
}

template< typename T >
struct box_input : gf::input_root< T >, only_init {};

template< typename T >
struct box_output : gf::output_root< T >, only_init {};

struct only_refresh {
   static void refresh();
};

template< typename T >
struct only_init_read {
   static T read();
};

TEST_CASE( "input" ){
   REQUIRE( false  == ( gf::input< bool > ) );
   REQUIRE( false  == ( gf::input< int > ) );
   REQUIRE( false  == ( gf::input< only_init > ) );
   REQUIRE( false  == ( gf::input< box_box< bool > > ) );
   REQUIRE( false  == ( gf::input< box_box< int > > ) );
   REQUIRE( false  == ( gf::input< box_item< bool > > ) );
   REQUIRE( false  == ( gf::input< box_item< int > > ) );
   REQUIRE( false  == ( gf::input< box_stream< bool > > ) );
   REQUIRE( false  == ( gf::input< box_stream< int > > ) );
}

