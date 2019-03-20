// ==========================================================================
//
// gf-basics.hpp
// 
// the basic stuff of the library
//
// ==========================================================================
//
// This file is part of godafoss, 
// a C++ library for close-to-the-hardware programming.
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
// macros that influence a functions' properties
//
// ==========================================================================
   
#define GODAFOSS_INLINE __attribute__((always_inline))

#define GODAFOSS_NO_INLINE __attribute__((noinline))

#define GODAFOSS_NORETURN GODAFOSS_INLINE [[noreturn]]

#define GODAFOSS_RAM_FUNCTION \
   __attribute__( ( noinline, long_call, section(".data") ) )

   
// ==========================================================================
//
// macro that causes a (void) function to be run only once
//
// ==========================================================================
  
#define GODAFOSS_RUN_ONCE {	           \
   static bool _done = false;          \
   if( _done ){                        \
      return;                          \
   }                                   \
   _done = true;                       \
}   
