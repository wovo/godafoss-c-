// =============================================================================
//
// gf-test-case-random.hpp
//
// =============================================================================
//
// Tests of the random fuctions.
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


void test_case_random(){

   // just to make sure that it can be called
   gf::random_seed( 12 );

   // to make sure that ths test increases the test count
   REQUIRE( true );

   constexpr int buckets = 65536;

   uint32_t count[ buckets ];

   for( auto & x : count ){
      x = 0;
   }

   constexpr uint32_t n = 1000;

   for( uint32_t i = 0; i < buckets * n ; i++ ){
      int32_t r = gf::random16();

      // prevent counting an awful lot of REQUIREs
      if( ( r < 0 ) || ( r >= buckets ) ){
         REQUIRE( r >= 0 );
         REQUIRE( r <  buckets );
      } else {
         count[ r ]++;
      }
   }

   // experimentally derived...
   constexpr int marge = 20;

   for( int i = 0; i < buckets; i++ ){
      auto x = count[ i ];
      if( ( x < n - marge ) || ( x > n + marge ) ) {
         EQUAL( count[ i ], n );
      }
   }
}

