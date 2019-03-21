// ==========================================================================
//
// File      : gf-all.hpp
// Part of   : C++ library for close-to-the-hardware programming
// Copyright : wouter@voti.nl 2019
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// ==========================================================================

// This file includes all target-independent files 
// to add a file, add it here and in the Makefile.inc
//
// C/C++ library files are included here rather than in the individual
// library files.
// The individual header files depend on the order in which they 
// are included here, and don't have multiple-include guards.
// They are not meant to be included individually.
//
// In normal use, this file is included by a target-specific file.

#ifndef GF_ALL_H
#define GF_ALL_H

// ==========================================================================
//
// The library headers we need are included here rather than in the files
// that need them. 
//
// A dirty hack is needed because the AVR8 compiler lacks certain headers
// https://github.com/maniacbug/StandardCplusplus
//
// ==========================================================================

#ifdef HWCPP_TARGET_use_avr8_hacks
   #include HWCPP_INCLUDE( targets/avr8-hacks/cstddef )
   #include HWCPP_INCLUDE( targets/avr8-hacks/climits )
   #include HWCPP_INCLUDE( targets/avr8-hacks/cstdint )
   #include HWCPP_INCLUDE( targets/avr8-hacks/algorithm )
   #include HWCPP_INCLUDE( targets/avr8-hacks/ratio )
   #include HWCPP_INCLUDE( targets/avr8-hacks/type_traits )
   #include HWCPP_INCLUDE( targets/avr8-hacks/array )
#else
   #include <cstddef>
   #include <climits>
   #include <algorithm>
   #include <cstdint>
   #include <ratio>
   #include <type_traits>
   #include <array>
#endif


// ==========================================================================
//
// This is a (greedy) selection from
// https://gcc.gnu.org/onlinedocs/gcc-7.2.0/gcc/Warning-Options.html
//
// For the library code a few false alarms are worth even a single error
// that is detected by these (rather high) warning level settings.
//
// ==========================================================================

#pragma GCC diagnostic push

#pragma GCC diagnostic error "-Wall"
#pragma GCC diagnostic error "-Wextra"
//#pragma GCC diagnostic error "-Wpedantic"
#pragma GCC diagnostic error "-Wdouble-promotion"
#pragma GCC diagnostic error "-Wchkp"
// -fdelete-null-pointer-checks
#pragma GCC diagnostic error "-Wnull-dereference"
#pragma GCC diagnostic error "-Wswitch-default"
#pragma GCC diagnostic error "-Wunused"
#pragma GCC diagnostic error "-Wuninitialized"
#pragma GCC diagnostic error "-Wstrict-overflow=5"

#pragma GCC diagnostic error "-Wsuggest-attribute=pure"
#pragma GCC diagnostic error "-Wsuggest-attribute=const"
#pragma GCC diagnostic error "-Wsuggest-attribute=noreturn"
#pragma GCC diagnostic error "-Wsuggest-attribute=format"
#pragma GCC diagnostic error "-Wmissing-format-attribute"
#pragma GCC diagnostic error "-Wsuggest-final-types"
#pragma GCC diagnostic error "-Wsuggest-final-methods"
#pragma GCC diagnostic error "-Wsuggest-override"

#pragma GCC diagnostic error "-Warray-bounds=2"
#pragma GCC diagnostic error "-Wduplicated-branches"
#pragma GCC diagnostic error "-Wduplicated-cond"
#pragma GCC diagnostic error "-Wfloat-equal"
#pragma GCC diagnostic error "-Wundef"
#pragma GCC diagnostic error "-Wcast-qual"
#pragma GCC diagnostic error "-Wcast-align"
#pragma GCC diagnostic error "-Wzero-as-null-pointer-constant"
#pragma GCC diagnostic error "-Wparentheses"
#pragma GCC diagnostic error "-Wdate-time"
#pragma GCC diagnostic error "-Wconversion"
#pragma GCC diagnostic error "-Wlogical-op"
#pragma GCC diagnostic error "-Winline"

#pragma GCC diagnostic ignored "-Wstrict-overflow"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wsuggest-attribute=const"
#pragma GCC diagnostic ignored "-Wattributes"

// -Wno-long-long.


// ==========================================================================
//
// include all non-HAL library files, enclosed in the hwcpp namespace
//
// ==========================================================================

namespace godafoss {

#include "core/gf-basics.hpp"
#include "core/gf-ints.hpp"

#include "core/gf-box.hpp"
#include "core/gf-box-filters.hpp"
#include "core/gf-box-invert.hpp"
#include "core/gf-box-direct.hpp"
#include "core/gf-box-adapters.hpp"

#include "core/gf-pin.hpp"
#include "core/gf-pin-adapters.hpp"
#include "core/gf-pin-demos.hpp"

#include "core/gf-port.hpp"
#include "core/gf-port-adapters.hpp"
#include "core/gf-port-dummies.hpp"
#include "core/gf-port-from-pins.hpp"
#include "core/gf-port-demos.hpp"

#include "core/gf-timing-wait.hpp"
#include "core/gf-timing-clock.hpp"

} // namespace godafoss

#endif // GF_ALL_H