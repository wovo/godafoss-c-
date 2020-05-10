// ==========================================================================
//
// gf-all.hpp
//
// ==========================================================================
//
// This file includes all target-independent files. 
//
// To add a file, add it here and in the Makefile.inc
//
// C/C++ library files are included here rather than in 
// the individual library files.
// The individual header files depend on the order in which they 
// are included here, and don't have multiple-include guards.
// They are not meant to be included individually.
//
// In typical use, this file is included (only) by a target HAL file.
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


#ifndef GF_ALL_H
#define GF_ALL_H

// ==========================================================================
//
// The library headers we need are included here 
// rather than in the files that need them. 
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
//#pragma GCC diagnostic error "-Wsuggest-attribute=noreturn"
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
// include all non-HAL library files, enclosed in the godafoss namespace
//
// ==========================================================================

namespace godafoss {
   
//#include "../externals/torsor.hpp"

#include "core/gf-basics.hpp"
#include "core/gf-ints.hpp"
#include "core/gf-passing.hpp"
#include "core/gf-output-stream.hpp"
#include "core/gf-xy.hpp"

#include "box/gf-box.hpp"
#include "box/gf-box-inherit.hpp"
#include "box/gf-box-no-inline.hpp"
#include "box/gf-box-invert.hpp"
#include "box/gf-box-direct.hpp"
#include "box/gf-box-buffered.hpp"
#include "box/gf-box-adapters.hpp"

#include "pins/gf-pin.hpp"
#include "pins/gf-pin-adapters.hpp"
#include "pins/gf-pin-dummies.hpp"
#include "pins/gf-pin-out-from-pins.hpp"
#include "pins/gf-pin-demos.hpp"

#include "core/gf-analog.hpp"

#include "ports/gf-port.hpp"
#include "ports/gf-port-adapters.hpp"
#include "ports/gf-port-dummies.hpp"
#include "ports/gf-port-from-pins.hpp"
#include "ports/gf-port-demos.hpp"

#include "timing/gf-timing-wait.hpp"
#include "timing/gf-timing-clock.hpp"
#include "timing/gf-slow-down.hpp"

#include "core/gf-spi.hpp"
#include "core/gf-i2c.hpp"

#include "char-io/gf-uart.hpp"
#include "char-io/gf-ostream.hpp"
#include "char-io/gf-ostream-object.hpp"
#include "char-io/gf-terminal.hpp"
#include "char-io/gf-terminal-demos.hpp"

#include "graphics/gf-color.hpp"
//#include "graphics/gf-window.hpp"
//#include "graphics/gf-drawables.hpp"
//#include "graphics/gf-window-demos.hpp"

#include "chips/gf-servo.hpp"
#include "chips/gf-hd44780.hpp"
#include "chips/gf-hc595.hpp"
#include "chips/gf-pcf8574.hpp"
//#include "chips/gf-pcf8591.hpp"
//#include "chips/gf-ssd1306.hpp"
#include "chips/gf-nrf24l01.hpp"

} // namespace godafoss

#endif // GF_ALL_H