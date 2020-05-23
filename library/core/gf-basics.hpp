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
// Instantiating the macro GODAFOSS_SUPPORTED( NAME_1, NAME_2 ) creates:
//
// a concept NAME_compatible< T > that is satisfied only when
//    a NAME_subpported< T > or < T, Ts > specialization exists that
//    has a static constexpr bool = true;
//
// a concept NAME_compatible_list< T... > that is satisfied when
//    all its parameters T... satisfy NAME_compatible< T >
//
// a base class template NAME_2< typename T > that (when instantiated)
//    creates an error message.
//
// Usage 1:
// 
//   GODAFOSS_SOPPORTED( pin_in, pin_in_from )
//      create a specialization of pin_in_compatible
//      now a template can constrain    
//
// ==========================================================================
  
#define GODAFOSS_SUPPORTED( NAME_1, NAME_2 )                    \
                                                                \
template< typename T, typename... P >                           \
struct NAME_1 ## _supported { };                                \
                                                                \
template< typename T >                                          \
struct NAME_1 ## _supported< T > { };                           \
                                                                \
template< typename... Ts >                                      \
concept NAME_1 ## _compatible = requires {                      \
   NAME_1 ## _supported< Ts... >::supported;                    \
};                                                              \
                                                                \
template< typename... Ts >                                      \
concept NAME_1 ## _compatible_list =                            \
   ( NAME_1 ## _compatible< Ts > && ... );                      \
                                                                \
template< typename... P >                                       \
   requires NAME_1 ## _compatible< P... >                       \
struct NAME_2 {                                                 \
   static_assert(                                               \
      sizeof...( P ) < 0,                                       \
         "library internal error\n"                             \
         "   " #NAME_2 "<> claims support for this type(s)"     \
         " of parameter(s),\n"                                  \
         "   but no actual implementation is available.\n"      \
         "   (check the 'required from here' line)" );          \
};                                                              
