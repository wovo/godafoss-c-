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

SEARCH            += $(GODAFOSS)

# core files

HEADERS           += include/core/basics.hpp
HEADERS           += include/targets/arduino-due.hpp


# add Catch
# (used by some tests)
SEARCH            += $(CATCH)/include


