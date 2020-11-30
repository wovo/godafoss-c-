// ============================================================================
//
// gf-all.hpp
//
// ============================================================================
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
// ============================================================================

// ============================================================================
//
// This file includes all target-independent files.
//
// To add a file, add it here and in the Makefile.inc
//
// C/C++ standard library files are included here rather than in
// the individual library files.
// The individual header files are not meant to be included individually:
// they depend on the order in which they are included here, 
// they assume to be inside the godafoss namespace,
// and they don't have multiple-include guards.
//
// In typical use, this file is included (only) by a target HAL file.
//
// ============================================================================

#ifndef GF_ALL_H
#define GF_ALL_H

// ============================================================================
//
// The library headers we need are included here
// rather than in the files that need them.
//
// A dirty hack is needed because the AVR8 compiler lacks certain headers
//
// https://github.com/maniacbug/StandardCplusplus
// https://archive.codeplex.com/?p=libavrstdcpp
//
// ============================================================================

#include <cstddef>
#include <cstdint>
#include <climits>
#include <limits>
#include <algorithm>
//#include <ratio>
#include <type_traits>
#include <array>
#include <concepts>


// ============================================================================
//
// This is a (greedy) selection from
// https://gcc.gnu.org/onlinedocs/gcc-7.2.0/gcc/Warning-Options.html
//
// For the library code a few false alarms are worth even a single error
// that is detected by these (rather high) warning level settings.
//
// ============================================================================

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
//#pragma GCC diagnostic error "-Wsuggest-override" - SFML

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
#pragma GCC diagnostic ignored "-Wsuggest-attribute=pure"
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wvolatile"
#pragma GCC diagnostic ignored "-Wunused-parameter"

// -Wno-long-long.


// ============================================================================
//
// include all non-HAL library files,
// enclosed in the godafoss namespace
//
// ============================================================================

namespace godafoss {

#include "basics/gf-attributes.hpp"
#include "basics/gf-shared-names.hpp"
#include "basics/gf-ints.hpp"
#include "basics/gf-loop.hpp"
#include "basics/gf-passing.hpp"
#include "basics/gf-random.hpp"
#include "basics/gf-background.hpp"
#include "basics/gf-application.hpp"

#include "item/gf-item.hpp"
#include "item/gf-item-inherit.hpp"
#include "item/gf-item-no-inline.hpp"
#include "item/gf-item-invert.hpp"
#include "item/gf-item-direct.hpp"
#include "item/gf-item-buffered.hpp"
#include "item/gf-item-adapters.hpp"

#include "ostream/gf-ostream.hpp"
#include "ostream/gf-uart.hpp"

#include "adts/gf-torsor.hpp"
#include "adts/gf-string.hpp"
#include "adts/gf-fraction.hpp"
#include "adts/gf-subrange.hpp"
#include "adts/gf-xy.hpp"
#include "adts/gf-color.hpp"
#include "adts/gf-date-and-time.hpp"

#include "pins/gf-pin.hpp"
#include "pins/gf-pin-pullup-pulldown.hpp"
#include "pins/gf-pin-adapters.hpp"
#include "pins/gf-pin-dummies.hpp"
#include "pins/gf-pin-all.hpp"
#include "basics/gf-analog.hpp"

#include "ports/gf-port.hpp"
#include "ports/gf-port-adapters.hpp"
#include "ports/gf-port-dummies.hpp"
#include "ports/gf-port-from-pins.hpp"

#include "modifiers/gf-modifiers-pins.hpp"
#include "modifiers/gf-modifiers-ports.hpp"

#include "timing/gf-timing-duration.hpp"
#include "timing/gf-timing-wait.hpp"
#include "timing/gf-timing-clock.hpp"
#include "timing/gf-slow-down.hpp"

#include "pins/gf-pin-demos.hpp"
#include "ports/gf-port-demos.hpp"

#include "char-io/gf-terminal.hpp"
#include "char-io/gf-terminal-demos.hpp"

#include "graphics/gf-window.hpp"
#include "graphics/gf-window-all.hpp"
#include "graphics/gf-window-decorators.hpp"
#include "graphics/gf-drawables.hpp"
#include "graphics/gf-window-demos.hpp"

#include "protocols/gf-spi.hpp"

#include "protocols/gf-i2c-profile.hpp"
#include "protocols/gf-i2c-bus.hpp"
#include "protocols/gf-i2c-bit-banged.hpp"
#include "protocols/gf-memory.hpp"

#include "chips/gf-servo.hpp"
#include "chips/gf-hd44780.hpp"
#include "chips/gf-ks0108.hpp"
#include "chips/gf-hc595.hpp"
#include "chips/gf-pcf8574.hpp"
#include "chips/gf-pcf8591.hpp"
#include "chips/gf-ssd1306.hpp"
#include "chips/gf-nrf24l01.hpp"
#include "chips/gf-max7219.hpp"
#include "chips/gf-ws2801.hpp"
#include "chips/gf-ws2812.hpp"
#include "chips/gf-st7567.hpp"
#include "chips/gf-ds1302.hpp"
#include "chips/gf-ds1307.hpp"
#include "chips/gf-ds3231.hpp"
#include "chips/gf-hx711.hpp"
#include "chips/gf-pca9685.hpp"
#include "chips/gf-dht11.hpp"

} // namespace godafoss

#endif // GF_ALL_H
