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
// @title function and class attributes
//
// @define GODAFOSS_INLINE
// GODAFOSS_INLINE forces a function to be inline.
// It is used when the function body is very simple, for instance when
// it calls only one deeper function.
// This serves (only) to reduce code size and execution time.
//
// @define GODAFOSS_NO_INLINE
// GODAFOSS_NO_INLINE forces a function to be not inline.
// This is used to preserve low-level properties of a function,
// like the number of cylces taken by the function preable and postamble.
// This can be important to get predictable timing.
//
// @define GODAFOSS_NORETURN
// GODAFOSS_NORETURN indicates that a function will not return.
// It is used for functions that contain a never-ending loop.
// This can reduce code size.
//
// @define GODAFOSS_IN_RAM
// GODAFOSS_IN_RAM places the function body in RAM (instead of FLASH).
// On some targets, this is necesarry to get predicatable timing,
// or faster execution.
//
// @define GODAFOSS_RUN_ONCE
// GODAFOSS_RUN_ONCE causes the remainder of the function (the part after
// the macro) to be executed only once.
//
// @define godafoss::not_constructible
// Inheriting from not_constructible makes it impossible to create objects
// of that class.
//
// @define godafoss::not_copyable
// Inheriting from not_copyable makes it impossible to copy an object
// of that class.
//
// =============================================================================


#define GODAFOSS_INLINE attribute__((always_inline))

#define GODAFOSS_NO_INLINE attribute__((noinline))

#define GODAFOSS_NORETURN [[noreturn]] GODAFOSS_INLINE

#define GODAFOSS_IN_RAM \
   __attribute__( ( noinline, long_call, section(".data") ) )

#define _GODAFOSS_WEAK __attribute__((weak))

#define GODAFOSS_RUN_ONCE {            \
   static bool done = false;           \
   if( done ){                         \
      return;                          \
   }                                   \
   done = true;                        \
}


// =============================================================================

class not_constructible {
private:
   not_constructible();
};

class not_copyable {
private:
   not_copyable( const not_copyable & );
   not_copyable & operator=( const not_copyable & );
public:
   not_copyable();
};
