#============================================================================
#
# makefile-include for bmptk: 
# sets 
# - CPP_LANGUAGE to C++ 17, and enables concepts
# - the search path (the GODAFOSS/include, targets, and avr8-hacks)
# - the dependencies (all hwlib source files)
# 
# (c) Wouter van Ooijen (wouter@voti.nl) 2017
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)   
#
#============================================================================

# required C++ command-line settings

CPP_LANGUAGE      += -std=c++17 -fconcepts

# the file locations

SEARCH            += $(GODAFOSS)/library

# core files

HEADERS           += core/gf-basics.hpp
HEADERS           += core/gf-ints.hpp

HEADERS           += core/gf-box.hpp
HEADERS           += core/gf-box-filters.hpp
HEADERS           += core/gf-box-invert.hpp
HEADERS           += core/gf-box-direct.hpp
HEADERS           += core/gf-box-adapters.hpp

HEADERS           += core/gf-pin.hpp
HEADERS           += core/gf-pin-adapters.hpp
HEADERS           += core/gf-pin-demos.hpp

HEADERS           += core/gf-port.hpp
HEADERS           += core/gf-port-adapters.hpp
HEADERS           += core/gf-port-dummies.hpp
HEADERS           += core/gf-port-from-pins.hpp
HEADERS           += core/gf-port-demos.hpp

HEADERS           += core/gf-timing-wait.hpp
HEADERS           += core/gf-timing-clock.hpp

HEADERS           += targets/gf-arduino-due.hpp
HEADERS           += targets/gf-sam3x8e.hpp
HEADERS           += targets/gf-db103.hpp
HEADERS           += targets/gf-lpc1114.hpp


# add Catch
# (used by some tests)
SEARCH            += $(CATCH)/include


