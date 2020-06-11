// =============================================================================
//
// gf-test-case-passing.hpp
//
// =============================================================================
//
// Tests of gf::by_const< T >
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


#define TEST_PASSING( NAME, TYPE, XX )                                      \
                                                                            \
GODAFOSS_NO_INLINE void f_ ## NAME ## _value( const TYPE x ){               \
   volatile auto dummy = XX;                                                \
   (void) dummy;                                                            \
}                                                                           \
                                                                            \
GODAFOSS_NO_INLINE void f_ ## NAME ## _reference( const TYPE & x ){         \
   volatile auto dummy = XX;                                                \
   (void) dummy;                                                            \
}                                                                           \
                                                                            \
GODAFOSS_NO_INLINE void f_ ## NAME ## _by_const( gf::by_const< TYPE > x ){  \
   volatile auto dummy = XX;                                                \
   (void) dummy;                                                            \
}                                                                           \
                                                                            \
GODAFOSS_NO_INLINE void f_ ## NAME ## _sentinel(){}                         \
                                                                            \
bool check_pass_ ## NAME ( bool log ){                                      \
   auto a =                                                                 \
      (long long int) &f_ ## NAME ## _reference                             \
    - (long long int) &f_ ## NAME ## _value;                                \
   auto b =                                                                 \
      (long long int) &f_ ## NAME ## _by_const                              \
    - (long long int) &f_ ## NAME ## _reference;                            \
   auto c =                                                                 \
      (long long int) &f_ ## NAME ## _sentinel                              \
    - (long long int) &f_ ## NAME ## _by_const;                             \
   bool error = ( ( c > a ) || ( c > b ) );                                 \
   if( log || error ){                                                      \
      gf::ostream< gf::formatter< uart > > cout;                            \
      cout                                                                  \
         << "sizeof( " # TYPE ")= " << sizeof( TYPE ) << "\n"               \
         << "   by value      " << a << "\n"                                \
         << "   by reference  " << b << "\n"                                \
         << "   by by_const<> " << c << "\n"                                \
         << "\n";                                                           \
   }                                                                        \
   return ! error;                                                          \
}                                                                           \

template< int N >
struct ss { char c; char s[ N ]; };

TEST_PASSING( a, char, x )
TEST_PASSING( b, short int, x )
TEST_PASSING( c, int, x )
TEST_PASSING( d, long int, x )
TEST_PASSING( e, long long int, x )
TEST_PASSING( f1, ss< 1 >, x.c )
TEST_PASSING( f2, ss< 2 >, x.c )
TEST_PASSING( f3, ss< 3 >, x.c )
TEST_PASSING( f4, ss< 4 >, x.c )
TEST_PASSING( f5, ss< 5 >, x.c )
TEST_PASSING( f6, ss< 6 >, x.c )
TEST_PASSING( f7, ss< 7 >, x.c )
TEST_PASSING( f8, ss< 8 >, x.c )
TEST_PASSING( f9, ss< 9 >, x.c )
TEST_PASSING( f10, ss< 10 >, x.c )

void test_case_passing( bool log = false ){
   REQUIRE( check_pass_a( log ) );
   REQUIRE( check_pass_b( log ) );
   REQUIRE( check_pass_c( log ) );
   REQUIRE( check_pass_d( log ) );
   REQUIRE( check_pass_e( log ) );
   REQUIRE( check_pass_f1( log ) );
   REQUIRE( check_pass_f2( log ) );
   REQUIRE( check_pass_f3( log ) );
   REQUIRE( check_pass_f4( log ) );
   REQUIRE( check_pass_f5( log ) );
   REQUIRE( check_pass_f6( log ) );
   REQUIRE( check_pass_f7( log ) );
   REQUIRE( check_pass_f8( log ) );
   REQUIRE( check_pass_f9( log ) );
   REQUIRE( check_pass_f10( log ) );
}
