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

# CPP_LANGUAGE      += -std=c++17 -fconcepts

# the file locations

SEARCH            += $(GODAFOSS)/library
SEARCH            += $(GODAFOSS)/externals

# core files

HEADERS           += torsor.hpp

HEADERS           += godafoss.hpp
HEADERS           += gf-all.hpp

HEADERS           += core/gf-basics.hpp
HEADERS           += core/gf-ints.hpp
HEADERS           += core/gf-passing.hpp
HEADERS           += core/gf-output-stream.hpp
HEADERS           += core/gf-random.hpp
HEADERS           += core/gf-xy.hpp
HEADERS           += core/gf-decorators.hpp

HEADERS           += box/gf-box.hpp
HEADERS           += box/gf-box-inherit.hpp
HEADERS           += box/gf-box-no-inline.hpp
HEADERS           += box/gf-box-invert.hpp
HEADERS           += box/gf-box-direct.hpp
HEADERS           += box/gf-box-buffered.hpp
HEADERS           += box/gf-box-adapters.hpp

HEADERS           += pins/gf-pin.hpp
HEADERS           += pins/gf-pin-adapters.hpp
HEADERS           += pins/gf-pin-dummies.hpp
HEADERS           += pins/gf-pin-all.hpp
HEADERS           += pins/gf-pin-demos.hpp

HEADERS           += core/gf-analog.hpp

HEADERS           += ports/gf-port.hpp
HEADERS           += ports/gf-port-adapters.hpp
HEADERS           += ports/gf-port-dummies.hpp
HEADERS           += ports/gf-port-from-pins.hpp
HEADERS           += ports/gf-port-demos.hpp

HEADERS           += timing/gf-timing-wait.hpp
HEADERS           += timing/gf-timing-clock.hpp
HEADERS           += timing/gf-slow-down.hpp

HEADERS           += core/gf-i2c.hpp
HEADERS           += core/gf-spi.hpp

HEADERS           += char-io/gf-uart.hpp
HEADERS           += char-io/gf-ostream.hpp
HEADERS           += char-io/gf-ostream-object.hpp
HEADERS           += char-io/gf-terminal.hpp
HEADERS           += char-io/gf-terminal-demos.hpp

HEADERS           += graphics/gf-color.hpp
HEADERS           += graphics/gf-window.hpp
HEADERS           += graphics/gf-window-all.hpp
HEADERS           += graphics/gf-window-decorators.hpp
HEADERS           += graphics/gf-drawables.hpp
HEADERS           += graphics/gf-window-demos.hpp


HEADERS           += chips/gf-servo.hpp
HEADERS           += chips/gf-hd44780.hpp
HEADERS           += chips/gf-hc595.hpp
HEADERS           += chips/gf-pcf8574.hpp
HEADERS           += chips/gf-pcf8591.hpp
HEADERS           += chips/gf-ssd1306.hpp
HEADERS           += chips/gf-nrf24l01.hpp

HEADERS           += targets/gf-chip-atmega328.hpp
HEADERS           += targets/gf-chip-lpc1114.hpp
HEADERS           += targets/gf-chip-sam3x8e.hpp
HEADERS           += targets/gf-board-arduino-uno.hpp
HEADERS           += targets/gf-board-my-first-dev-board.hpp
HEADERS           += targets/gf-board-arduino-due.hpp
HEADERS           += targets/gf-board-db103.hpp
HEADERS           += targets/gf-native.hpp


# add Catch
# (used by some tests)
SEARCH            += $(CATCH)/include


