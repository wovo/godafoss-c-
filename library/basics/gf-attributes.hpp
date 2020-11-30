// =============================================================================
//
// gf-attributes.hpp
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
//
// @define GODAFOSS_INLINE
// @title function and class attribute macros
//
// @insert GODAFOSS_INLINE
// GODAFOSS_INLINE forces a function to be inline.
// It is used when the function body is very simple, 
// for instance when it calls only one deeper function.
// This serves (only) to reduce code size and execution time.
//
// @bar
// @define GODAFOSS_NO_INLINE
// @insert GODAFOSS_NO_INLINE
// GODAFOSS_NO_INLINE forces a function to be not inline.
// This is used to preserve low-level properties of a function,
// like the number of cycles taken by the function preamble and postamble.
// This can be important to get predictable timing.
//
// @bar
// @define GODAFOSS_NO_RETURN
// @insert GODAFOSS_NO_RETURN
// GODAFOSS_NORETURN indicates that a function will not return.
// It is used for functions that contain a never-ending loop.
// This can reduce code size.
//
// @bar
// @define GODAFOSS_IN_RAM
// @insert GODAFOSS_IN_RAM
// GODAFOSS_IN_RAM places the function body in RAM (instead of FLASH).
// On some targets, this is necessary to get predictable timing,
// or faster execution.
//
// @bar
// @define GODAFOSS_RUN_ONCE
// @insert GODAFOSS_RUN_ONCE
// GODAFOSS_RUN_ONCE causes the remainder of the function 
// (the part after the macro) to be executed only once.
//
// @bar
// @define godafoss::not_constructible
// @insert not_constructible
// Inheriting from not_constructible makes it impossible to create objects
// of the class.
//
// @bar
// @define godafoss::not_copyable
// @insert not_copyable
// Inheriting from not_copyable makes it impossible to copy an object
// of the class.
//
// =============================================================================


// @quote GODAFOSS_INLINE 1 ...
#define GODAFOSS_INLINE \
   __attribute__((always_inline))

// @quote GODAFOSS_NO_INLINE 1 ...
#define GODAFOSS_NO_INLINE \
   __attribute__((noinline))

// @quote GODAFOSS_NO_RETURN 1 ...
#define GODAFOSS_NO_RETURN \
   [[noreturn]] GODAFOSS_INLINE

// @quote GODAFOSS_IN_RAM 1 ...
#define GODAFOSS_IN_RAM \
   __attribute__( ( noinline, long_call, section(".data") ) )

// @quote GODAFOSS_WEAK 1 ...
#define GODAFOSS_WEAK \
   __attribute__((weak))


// =============================================================================

// @quote GODAFOSS_RUN_ONCE 1 ...
#define GODAFOSS_RUN_ONCE              \
{                                      \
   static bool done = false;           \
   if( done ){                         \
      return;                          \
   }                                   \
   done = true;                        \
}


// =============================================================================

// @quote not_constructible 1 ... };
struct not_constructible {
private:
   not_constructible();
};

// @quote not_copyable 1 ... };
struct not_copyable {
private:
   not_copyable( const not_copyable & );
   not_copyable & operator=( const not_copyable & );
public:
   not_copyable();
};
