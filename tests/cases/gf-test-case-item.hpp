// =============================================================================
//
// gf-test-case-item.hpp
//
// =============================================================================
//
// Tests of the item abstractions.
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
// item
//
// =============================================================================

struct empty {
   using value_type = bool;
};

struct empty_init {
   using value_type = bool;
   static void init();
};

struct only_init {
   static void init();
};

template< typename T >
struct item_item : gf::item_root< T >, only_init {};

void test_case_item_item(){
   //EQUAL( false,  gf::is_item< bool > );
   //EQUAL( false,  gf::is_item< int > );
   EQUAL( false,  gf::is_item< empty > );
   EQUAL( false,  gf::is_item< empty_init > );
   EQUAL( true,   gf::is_item< item_item< bool > > );
   EQUAL( true,   gf::is_item< item_item< int > > );
}


// =============================================================================
//
// box, pipe
//
// =============================================================================

template< typename T >
struct item_box : gf::box_root< T >, only_init {};

template< typename T >
struct item_pipe : gf::pipe_root< T >, only_init {};

void test_case_item_box(){
   //EQUAL( false,  gf::is_box< bool > );
   //EQUAL( false,  gf::is_box< int > );
   EQUAL( false,  gf::is_box< empty_init > );
   EQUAL( false,  gf::is_box< item_item< bool > > );
   EQUAL( false,  gf::is_box< item_item< int > > );
   EQUAL( true,   gf::is_box< item_box< bool > > );
   EQUAL( true,   gf::is_box< item_box< int > > );
   EQUAL( false,  gf::is_box< item_pipe< bool > > );
   EQUAL( false,  gf::is_box< item_pipe< int > > );
}

void test_case_item_pipe(){
   //EQUAL( false,  gf::is_pipe< bool > );
   //EQUAL( false,  gf::is_pipe< int > );
   EQUAL( false,  gf::is_pipe< empty_init > );
   EQUAL( false,  gf::is_pipe< item_item< bool > > );
   EQUAL( false,  gf::is_pipe< item_item< int > > );
   EQUAL( false,  gf::is_pipe< item_box< bool > > );
   EQUAL( false,  gf::is_pipe< item_box< int > > );
   EQUAL( true,   gf::is_pipe< item_pipe< bool > > );
   EQUAL( true,   gf::is_pipe< item_pipe< int > > );
}


// =============================================================================
//
// input
//
// =============================================================================

template< typename T >
struct item_input : gf::input_root< T >, only_init {};

template< typename T >
struct item_output : gf::output_root< T >, only_init {};

struct only_refresh {
   static void refresh();
};

template< typename T >
struct only_init_read {
   static T read();
};

void test_case_item_input(){
   //EQUAL( false,  gf::is_input< bool > );
   //EQUAL( false,  gf::is_input< int > );
   EQUAL( false,  gf::is_input< empty_init > );
   EQUAL( false,  gf::is_input< item_item< bool > > );
   EQUAL( false,  gf::is_input< item_item< int > > );
   EQUAL( false,  gf::is_input< item_box< bool > > );
   EQUAL( false,  gf::is_input< item_box< int > > );
   EQUAL( false,  gf::is_input< item_pipe< bool > > );
   EQUAL( false,  gf::is_input< item_pipe< int > > );
}


// =============================================================================
//
// test_case_item
//
// =============================================================================

void test_case_item(){
   test_case_item_item();
   test_case_item_box();
   test_case_item_pipe();
   test_case_item_input();
}
