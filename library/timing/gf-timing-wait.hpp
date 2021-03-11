// ==========================================================================
//
// gf-timing-wait.hpp
//
// ==========================================================================
//
// This file is part the https://www.github.com/godafoss
// free C++ library for close-to-the-hardware programming.
//
// Copyright Wouter van Ooijen 2019
//
// Distributed under the Boost Software License, Version 1.0.
// (See the accompanying LICENSE_1_0.txt in the root directory of this
// library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================



// ==========================================================================
//
// concept
//
// ==========================================================================

template< typename T >
concept is_timing_wait = requires {
      T::_timing_wait_marker;
   }
   && is_static_duration< typename T:: template ns< 5 > >
   && is_static_duration< typename T:: template us< 5 > >
   && is_static_duration< typename T:: template ms< 5 > >
   && is_cto< T  >;

struct timing_wait_root : cto_root {
   static constexpr bool _timing_wait_marker = true;
};


template< is_timing_wait T >
struct timing_wait : timing_wait_root {
   //static void init(){ T::init(); }
   using resources = use_list< T >;
   template< uint64_t n > using ns = typename T::ns< n >;
   template< uint64_t n > using us = typename T::us< n >;
   template< uint64_t n > using ms = typename T::ms< n >;
};


// ==========================================================================
//
// construction
//
// ==========================================================================

template< typename T >
concept is_timing_wait_base = requires {
//   { T::init() } -> void;
//   { T::wait_ticks_function( typename T::ticks_type ) } -> void;
   // template
   //{ T::now_ticks(); } -> void;
//   { ticks_from_ns( uint64_t ) } -> typename T::ticks_type;
   true;
};

template< is_timing_wait_base T >
struct be_timing_wait : timing_wait_root {
   static const bool is_timing_wait = true;
   //using ticks_type = typename T::ticks_type;

   template< uint64_t n >
   struct ns : static_duration_root {

      static const uint64_t n_ticks = T::ticks_from_ns( n );

      //static void GODAFOSS_INLINE init(){
      //   T::init();
      //}
      using resources = use_list< T >;

      static void GODAFOSS_INLINE wait(){
          T:: template wait_ticks_template< n_ticks >();
      }

      static void GODAFOSS_INLINE wait_busy(){
          wait();
      }

   };

   template< uint64_t n >
   using us = ns< n * 1'000 >;

   template< uint64_t n >
   using ms = ns< n * 1'000'000 >;

   static void wait_ns( int64_t n ){
      T::wait_ticks_function( T::ticks_from_ns( n ));
   }

   static void GODAFOSS_INLINE wait_us( int64_t n ){
      wait_ns( n * 1'000 );
   }

   static void GODAFOSS_INLINE wait_ms( int64_t n ){
      wait_ns( n * 1'000'000 );
   }

};
