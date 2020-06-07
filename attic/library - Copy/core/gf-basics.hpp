// ==========================================================================
//
// gf-basics.hpp
// 
// ==========================================================================
//
// some basic stuff:
//    - macros that make a function
//       - inline
//       - not inline 
//       - placed in RAM (instead of FLASH)
//       - weak
//    - macro that makes a function run once
//
// ==========================================================================
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
// ==========================================================================


// ==========================================================================
//
// macros that influence the properties of a functions, 
// but not it semantics
//
// ==========================================================================
   
#define GODAFOSS_INLINE __attribute__((always_inline))

#define GODAFOSS_NO_INLINE __attribute__((noinline))

#define GODAFOSS_NORETURN [[noreturn]] GODAFOSS_INLINE

#define GODAFOSS_RAM_FUNCTION \
   __attribute__( ( noinline, long_call, section(".data") ) )
   
#ifdef __MINGW32__
   #define GODAFOSS_WEAK // weak doesn't seem to work on MINGW32
#else
   #define GODAFOSS_WEAK __attribute__((weak))
#endif   

   
// ==========================================================================
//
// macro that causes a (void) function to be run only once
//
// ==========================================================================
  
#define GODAFOSS_RUN_ONCE {	         \
   static bool done = false;           \
   if( done ){                         \
      return;                          \
   }                                   \
   done = true;                        \
}   
