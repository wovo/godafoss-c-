################################################################################
#
# makefile.inc
#
################################################################################
#
# makefile-include for the bmptk build system
#
# sets
#    - SEARCH (the search path)
#    - HEADERS (the dependencies)
#
# This file should be included by a project that uses godafoss and bmptk.
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

# externals

# godafoss files

#SEARCH += /usr/arm-none-eabi/include
#SEARCH += /usr/arm-none-eabi/include/c++/10.1.0
#SEARCH += /usr/arm-none-eabi/include/c++/10.1.0/arm-none-eabi

SEARCH            += $(GODAFOSS)/library

HEADERS           += godafoss.hpp
HEADERS           += gf-all.hpp

HEADERS           += basics/gf-attributes.hpp
HEADERS           += basics/gf-shared-names.hpp
HEADERS           += basics/gf-ints.hpp
HEADERS           += basics/gf-passing.hpp
HEADERS           += basics/gf-random.hpp
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

HEADERS           += protocols/gf-i2c.hpp
HEADERS           += protocols/gf-spi.hpp
HEADERS           += protocols/gf-mememory.hpp

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

HEADERS           += targets/gf-chip-atmega328.hpp
HEADERS           += targets/gf-chip-lpc1114.hpp
HEADERS           += targets/gf-chip-sam3x8e.hpp
HEADERS           += targets/gf-board-arduino-uno.hpp
HEADERS           += targets/gf-board-my-first-dev-board.hpp
HEADERS           += targets/gf-board-arduino-due.hpp
HEADERS           += targets/gf-board-db103.hpp
HEADERS           += targets/gf-native.hpp
