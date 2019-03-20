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

#include <cstdint>
#include <array>
//#include <stddef.h>
#include <type_traits>

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

#include "core/gf-timing-wait.hpp"
#include "core/gf-timing-clock.hpp"

} // namespace godafoss

#endif // GF_ALL_H