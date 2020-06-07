// =============================================================================
//
// gf-test-case-shared-names.hpp
//
// =============================================================================
//
// Tests of the GODAFOSS_SUPPORTED mechanism.
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

void test_case_shared_names(){

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

   EQUAL( true,   ( valid< pin_from, int > ) );
   EQUAL( false,  ( valid< pin_from, float > ) );
   EQUAL( false,  ( valid< pin_from, double > ) );
}
