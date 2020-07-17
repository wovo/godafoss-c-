// =============================================================================
//
// gf-test-case-pins.hpp
//
// =============================================================================
//
// Tests of the pin roots and concepts.
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


// =============================================================================
//
// pin_in
//
// =============================================================================

struct my_pin_in : gf::pin_in_root {
   static void init();
   static void refresh();
   static bool read();
};

template< gf::is_pin_in T >
struct needs_pin_in {};


// =============================================================================
//
// pin_out
//
// =============================================================================


struct my_pin_out : gf::pin_out_root {
   static void init();
   static void write( bool );
   static void flush();
};

template< gf::is_pin_out T >
struct needs_pin_out {};


// =============================================================================
//
// pin_in_out
//
// =============================================================================

struct my_pin_in_out : gf::pin_in_out_root {
   static void init();
   static void direction_set_input();
   static void direction_set_output();
   static void direction_flush();
   static void write( bool );
   static void flush();
   static void refresh();
   static bool read();
};

template< gf::is_pin_in_out T >
struct needs_pin_in_out {};


// =============================================================================
//
// pin_oc
//
// =============================================================================

struct my_pin_oc : gf::pin_oc_root {
   static void init();
   static void write( bool );
   static void flush();
   static void refresh();
   static bool read();
};

template< gf::is_pin_oc T >
struct needs_pin_oc {};


// =============================================================================
//
// test_case_pins
//
// =============================================================================


void test_case_pins(){
   (void) needs_pin_in< my_pin_in >();

   EQUAL( gf::is_pin_in< my_pin_in >,      true  );
   EQUAL( gf::is_pin_out< my_pin_in >,     false );
   EQUAL( gf::is_pin_in_out< my_pin_in >,  false );
   EQUAL( gf::is_pin_oc< my_pin_in >,      false );

   (void) needs_pin_out< my_pin_out >();

   EQUAL( gf::is_pin_in< my_pin_out >,      false );
   EQUAL( gf::is_pin_out< my_pin_out >,     true  );
   EQUAL( gf::is_pin_in_out< my_pin_out >,  false );
   EQUAL( gf::is_pin_oc< my_pin_out >,      false );

   (void) needs_pin_in_out< my_pin_in_out >();

   EQUAL( gf::is_pin_in< my_pin_in_out >,      false );
   EQUAL( gf::is_pin_out< my_pin_in_out >,     false  );
   EQUAL( gf::is_pin_in_out< my_pin_in_out >,  true );
   EQUAL( gf::is_pin_oc< my_pin_in_out >,      false );

   (void) needs_pin_oc< my_pin_oc >();

   EQUAL( gf::is_pin_in< my_pin_oc >,      false );
   EQUAL( gf::is_pin_out< my_pin_oc >,     false );
   EQUAL( gf::is_pin_in_out< my_pin_oc >,  false );
   EQUAL( gf::is_pin_oc< my_pin_oc >,      true );

}
