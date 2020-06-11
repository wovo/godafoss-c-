// =============================================================================
//
// gf-test-framework.hpp
//
// =============================================================================
//
// This is a very simple test framework, that can be used
// both running native, and running on a target that has a uart<>.
//
// This is not realy a header file.
// For its use, check the tests for the various targets.
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


namespace gf  = godafoss;
using uart    = godafoss::target<>::uart<>;

class test {
public:

   static inline int _equal_calls_count = 0;
   static inline int _equal_failures_count = 0;

   static void start(){
      timing::init();
      timing::ms< 1'000>::wait();
      static gf::ostream< gf::formatter< uart > > cout;
      cout
         << "\nTest starts.\n";

   }

   static void end(){
      static gf::ostream< gf::formatter< uart > > cout;
      if( _equal_failures_count == 0 ){
         cout
	         << "\nTest success: "
            << gf::dec << _equal_calls_count
		      << " test were successfull.\n";
      } else {
         cout
	         << "\nTEST FAILURE: "
		      << gf::dec << _equal_failures_count
		      << " test were NOT successfull.\n";
      }
   }

   template< typename A, typename B >
   static void _equal(
      const char * f, int n,            // file name and line number
      const char * ta, const char *tb,  // arguments, stringified
      const A & a, const B & b          // arguments, as-is
   ){
      static gf::ostream< gf::formatter< uart > > cout;
      ++_equal_calls_count;
      if( a != b ){
         ++_equal_failures_count;
         cout
            << f << ":" << gf::dec << n
            << " check failed \n"
            << "   GODAFOSS_TEST_EQUAL( " << ta << " , " << tb << " )\n"
            << "   left  \"" << ta << "\" = " << a << "\n"
            << "   right \"" << tb << "\" = " << b << "\n\n";
      }
   }

};

#define EQUAL( a, b ) test::_equal( __FILE__, __LINE__, #a, #b, a, b );
#define REQUIRE( a ) EQUAL( a, true )

// true iff P< T > yields something that can be instantiated
// note: this will NOT hide an assertion failure
template< template< typename > typename P, typename T >
concept valid = requires{ P< T >(); };
