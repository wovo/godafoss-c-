// =============================================================================
//
// This runs all tests that can be run native.
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


#include "godafoss.hpp"
#include "tests/gf-test-framework.hpp"
#include "tests/gf-test-case-attributes.hpp"
#include "tests/gf-test-case-shared-names.hpp"
#include "tests/gf-test-case-ints.hpp"
#include "tests/gf-test-case-random.hpp"
#include "tests/gf-test-case-box.hpp"

int main(){

   test_case_attributes();
   test_case_shared_names();
   test_case_ints();
   test_case_random();
   test_case_box();

   test::end();
}
