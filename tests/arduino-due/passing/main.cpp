// =============================================================================
//
// test parameter passing
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
namespace gf  = godafoss;
using target  = gf::target<>;
using timing  = target::timing;
using uart    = target::uart<>;

#include "gf-test-framework.hpp"

#include "gf-test-case-passing.hpp"

int main( void ){
   test::start();

   test_case_passing();

   test::end();
}
