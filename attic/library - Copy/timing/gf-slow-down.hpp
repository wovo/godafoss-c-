// ==========================================================================
//
// gf-slow-down.hpp
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


template< typename T, uint64_t F >
struct slow_down : T {
	
   template< uint64_t n >
   struct ns {
      
      static const uint64_t n_ticks = T::ticks_from_ns( F * n );	   
      
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
};