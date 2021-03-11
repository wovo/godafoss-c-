################################################################################
#
# makefile.inc
#
################################################################################
#
# This file is part of godafoss (https://github.com/wovo/godafoss),
# a C++ library for close-to-the-hardware programming.#
#
# Copyright
#    Wouter van Ooijen 2017-2020
#
# Distributed under the Boost Software License, Version 1.0.
# (See the accompanying LICENSE_1_0.txt in the root directory of this
# library, or a copy at http://www.boost.org/LICENSE_1_0.txt)
#
################################################################################
#
# makefile-include for the bmptk build system
#
# This script should be included by a project that uses godafoss and bmptk.
# GODAFOSS must have been set to the godafoss directory.
#
# This script sets:
#    - CPP_LANGUAGE
#    - SEARCH (the bmptk search path)
#    - HEADERS (the bmptk dependencies)
#
################################################################################

CPP_LANGUAGE      := -std=c++20  -fconcepts-diagnostics-depth=3

# used by some tests
SEARCH            += $(GODAFOSS)/../Catch2/include

# the godafoss files are all included relative to this point
SEARCH            += $(GODAFOSS)/library

HEADERS           += godafoss.hpp
HEADERS           += gf-all.hpp

HEADERS           += basics/gf-attributes.hpp
HEADERS           += basics/gf-shared-names.hpp
HEADERS           += basics/gf-ints.hpp
HEADERS           += basics/gf-loop.hpp
HEADERS           += basics/gf-passing.hpp
HEADERS           += basics/gf-random.hpp
HEADERS           += basics/gf-application.hpp
HEADERS           += basics/gf-background.hpp

HEADERS           += item/gf-item.hpp
HEADERS           += item/gf-item-inherit.hpp
HEADERS           += item/gf-item-no-inline.hpp
HEADERS           += item/gf-item-invert.hpp
HEADERS           += item/gf-item-direct.hpp
HEADERS           += item/gf-item-buffered.hpp
HEADERS           += item/gf-item-adapters.hpp

HEADERS           += ostream/gf-ostream.hpp
HEADERS           += ostream/gf-uart.hpp

HEADERS           += adts/gf-torsor.hpp
HEADERS           += adts/gf-string.hpp
HEADERS           += adts/gf-fraction.hpp
HEADERS           += adts/gf-subrange.hpp
HEADERS           += adts/gf-xy.hpp
HEADERS           += adts/gf-color.hpp
HEADERS           += adts/gf-date-and-time.hpp

HEADERS           += pins/gf-pin.hpp
HEADERS           += pins/gf-pin-pullup-pulldown.hpp
HEADERS           += pins/gf-pin-adapters.hpp
HEADERS           += pins/gf-pin-dummies.hpp
HEADERS           += pins/gf-pin-all.hpp
HEADERS           += pins/gf-pin-demos.hpp

HEADERS           += basics/gf-analog.hpp

HEADERS           += ports/gf-port.hpp
HEADERS           += ports/gf-port-adapters.hpp
HEADERS           += ports/gf-port-dummies.hpp
HEADERS           += ports/gf-port-from-pins.hpp
HEADERS           += ports/gf-port-demos.hpp

HEADERS           += modifiers/gf-modifiers-pins.hpp
HEADERS           += modifiers/gf-modifiers-ports.hpp

HEADERS           += timing/gf-timing-duration.hpp
HEADERS           += timing/gf-timing-wait.hpp
HEADERS           += timing/gf-timing-clock.hpp
HEADERS           += timing/gf-slow-down.hpp

HEADERS           += char-io/gf-terminal.hpp
HEADERS           += char-io/gf-terminal-demos.hpp

HEADERS           += graphics/gf-window.hpp
HEADERS           += graphics/gf-window-all.hpp
HEADERS           += graphics/gf-window-decorators.hpp
HEADERS           += graphics/gf-drawables.hpp
HEADERS           += graphics/gf-window-demos.hpp

HEADERS           += protocols/gf-i2c-profile.hpp
HEADERS           += protocols/gf-i2c-bus.hpp
HEADERS           += protocols/gf-i2c-bit-banged.hpp

HEADERS           += protocols/gf-spi.hpp
HEADERS           += protocols/gf-memory.hpp

HEADERS           += chips/gf-servo.hpp
HEADERS           += chips/gf-hd44780.hpp
HEADERS           += chips/gf-ks0108.hpp
HEADERS           += chips/gf-hc595.hpp
HEADERS           += chips/gf-pcf8574.hpp
HEADERS           += chips/gf-pcf8591.hpp
HEADERS           += chips/gf-ssd1306.hpp
HEADERS           += chips/gf-nrf24l01.hpp
HEADERS           += chips/gf-max7219.hpp
HEADERS           += chips/gf-ws2801.hpp
HEADERS           += chips/gf-ws2812.hpp
HEADERS           += chips/gf-st7567.hpp
HEADERS           += chips/gf-ds1302.hpp
HEADERS           += chips/gf-ds1307.hpp
HEADERS           += chips/gf-ds3231.hpp
HEADERS           += chips/gf-hx711.hpp
HEADERS           += chips/gf-pca9685.hpp
HEADERS           += chips/gf-dht11.hpp

HEADERS           += target-hals/gf-atmega.hpp
#HEADERS           += target-hals/gf-atmega32u4.hpp
#HEADERS           += target-hals/gf-lpc1114.hpp
HEADERS           += target-hals/gf-sam3xa.hpp
HEADERS           += target-hals/gf-native.hpp

HEADERS           += target-boards/gf-arduino-uno.hpp
HEADERS           += target-boards/gf-arduino-leonardo.hpp
HEADERS           += target-boards/gf-ks0474.hpp
HEADERS           += target-boards/gf-my-first-dev-board.hpp
HEADERS           += target-boards/gf-arduino-due.hpp
HEADERS           += target-boards/gf-db103.hpp

