// =============================================================================
//
// gf-test-case-box.hpp
//
// =============================================================================
//
// Tests of the box abstractions.
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


struct only_init {
   static void init();
};

template< typename T >
struct box_box : gf::box_root< T >, only_init {};

void test_case_box_box(){
   EQUAL( false,  gf::box< bool > );
   EQUAL( false,  gf::box< int > );
   EQUAL( false,  gf::box< only_init > );
   EQUAL( true,   gf::box< box_box< bool > > );
   EQUAL( true,   gf::box< box_box< int > > );
}

template< typename T >
struct box_item : gf::item_root< T >, only_init {};

template< typename T >
struct box_stream : gf::stream_root< T >, only_init {};

void test_case_box_item(){
   EQUAL( false,  gf::item< bool > );
   EQUAL( false,  gf::item< int > );
   EQUAL( false,  gf::item< only_init > );
   EQUAL( false,  gf::item< box_box< bool > > );
   EQUAL( false,  gf::item< box_box< int > > );
   EQUAL( true,   gf::item< box_item< bool > > );
   EQUAL( true,   gf::item< box_item< int > > );
   EQUAL( false,  gf::item< box_stream< bool > > );
   EQUAL( false,  gf::item< box_stream< int > > );
}

void test_case_box_stream(){
   EQUAL( false,  gf::stream< bool > );
   EQUAL( false,  gf::stream< int > );
   EQUAL( false,  gf::stream< only_init > );
   EQUAL( false,  gf::stream< box_box< bool > > );
   EQUAL( false,  gf::stream< box_box< int > > );
   EQUAL( false,  gf::stream< box_item< bool > > );
   EQUAL( false,  gf::stream< box_item< int > > );
   EQUAL( true,   gf::stream< box_stream< bool > > );
   EQUAL( true,   gf::stream< box_stream< int > > );
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

void test_case_box_input(){
   EQUAL( false,  gf::input< bool > );
   EQUAL( false,  gf::input< int > );
   EQUAL( false,  gf::input< only_init > );
   EQUAL( false,  gf::input< box_box< bool > > );
   EQUAL( false,  gf::input< box_box< int > > );
   EQUAL( false,  gf::input< box_item< bool > > );
   EQUAL( false,  gf::input< box_item< int > > );
   EQUAL( false,  gf::input< box_stream< bool > > );
   EQUAL( false,  gf::input< box_stream< int > > );
}

void test_case_box(){
   test_case_box_box();
   test_case_box_stream();
   test_case_box_box();
   test_case_box_input();
}
