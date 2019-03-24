// ==========================================================================
//
// File      : godafoss.hpp
// Part of   : C++ library for close-to-the-hardware programming
// Copyright : wouter@voti.nl 2019
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// An application defines the appropriate GF_TARGET_* marco and 
// include this godafoss.hpp file.

// The bmptk build system will define the GF_TARGET_* macro
// base on the TARGET specified in the makefile.

#ifndef GODAFOSS_H
#define GODAFOSS_H

#ifdef GF_TARGET_arduino_uno
   #define GF_TARGET
   #include "targets/gf-board-arduino-uno.hpp"
#endif

#ifdef GF_TARGET_arduino_due
   #define GF_TARGET
   #include "targets/gf-board-arduino-due.hpp"
#endif

#ifdef GF_TARGET_db103
   #define GF_TARGET
   #include "targets/gf-board-db103.hpp"
#endif

#ifndef GF_TARGET
   #error no valid GF_TARGET_* defined
#endif   

#endif // GODAFOSS_H