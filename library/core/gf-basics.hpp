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
//    - macro that supports the *_from / *_compatible pattern
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

   
// ==========================================================================
//
// Instantiating the macro GODAFOSS_FROM_COMPATIBLE( NAME ) creates
// a base class template NAME_FROM< typename T > that (when instantiated)
//    creates an error message
// a concept NAME_compatible< T > that is satisfied only when
//    a NAME_from< T > class template specialization exist that
//    creates (is) a NAME
// a concept NAME_compatible_list< T... > that is satisfied when
//    all its parameters T... satisfy NAME_compatible< T >
//
// ==========================================================================
  
#define GODAFOSS_FROM_COMPATIBLE( NAME )                              \
                                                                      \
template< typename... P >                                             \
struct NAME ## _from {                                                \
   static_assert(                                                     \
      sizeof...( P ) < 0,                                             \
         #NAME "_from<> doesn't support this type of parameter(s)\n"  \
         "   (check the 'required from here' line)" );                \
};                                                                    \
                                                                      \
template< typename T >                                                \
concept NAME ## _compatible = NAME< NAME ## _from< T > >;             \
                                                                      \
template< typename... Ts >                                            \
concept NAME ## _compatible_list =                                    \
   ( NAME ## _compatible< Ts > && ... );                              
