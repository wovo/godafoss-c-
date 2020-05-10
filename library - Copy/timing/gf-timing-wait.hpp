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
   T::is_timing_wait;
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
struct be_timing_wait {
   static const bool is_timing_wait = true;
   //using ticks_type = typename T::ticks_type;
   
   template< uint64_t n >
   struct ns {
      
      static const uint64_t n_ticks = T::ticks_from_ns( n );	   
      
      static void GODAFOSS_INLINE init(){
         T::init();
      }        
      
      static void GODAFOSS_INLINE wait(){
          T:: template wait_ticks_template< n_ticks >();		    
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
