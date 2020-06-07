// ==========================================================================
//
// godafoss.hpp
//
// ==========================================================================
//
// This is the file that an application that uses godafoss should include.
//
// It should first define the appropriate GF_TARGET_* marco and 
// then include this godafoss.hpp file.
//
// The bmptk build system will define the appropriate GF_TARGET_* macro
// base on the TARGET specified in the makefile.
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


#ifndef GODAFOSS_H
#define GODAFOSS_H

#ifdef GF_TARGET_native
   #include "targets/gf-native.hpp"
   #define GF_TARGET
#endif

#ifdef GF_TARGET_arduino_uno
   #define GF_TARGET
   #include "targets/gf-board-arduino-uno.hpp"
#endif

#ifdef GF_TARGET_my_first_dev_board
   #define GF_TARGET
   #include "targets/gf-board-my-first-dev-board.hpp"
#endif

#ifdef GF_TARGET_arduino_due
   #define GF_TARGET
   #include "targets/gf-board-arduino-due.hpp"
#endif

#ifdef GF_TARGET_db103
   #define GF_TARGET
   #include "targets/gf-board-db103.hpp"
#endif

#ifdef GF_TARGET_stm32f030f4
   #define GF_TARGET
   #include "targets/gf-chip-stm32f030f4.hpp"
#endif

#ifndef GF_TARGET
   #error no valid GF_TARGET_* defined
#endif   

// could pop the warning settings here?

#endif // GODAFOSS_H