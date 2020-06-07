// =============================================================================
//
// gf-test-case-attributes.hpp
//
// =============================================================================
//
// Tests of the GODAFOSS_RUN_ONCE mechanism.
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


int count1 = 0;
int count2 = 0;

void test_run_once(){
   count1++;
   GODAFOSS_RUN_ONCE;
   count2++;
}

void test_case_attributes(){

   EQUAL( count1, 0 );
   EQUAL( count2, 0 );

   test_run_once();

   EQUAL( count1, 1 );
   EQUAL( count2, 1 );

   test_run_once();

   EQUAL( count1, 2 );
   EQUAL( count2, 1 );

}
