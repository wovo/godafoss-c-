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

SEARCH            += $(GODAFOSS)/externals

HEADERS           += torsor.hpp

# godafoss files

SEARCH            += $(GODAFOSS)/library

HEADERS           += godafoss.hpp
HEADERS           += gf-all.hpp

HEADERS           += basics/gf-attributes.hpp
HEADERS           += basics/gf-shared-names.hpp
HEADERS           += basics/gf-ints.hpp
HEADERS           += basics/gf-passing.hpp
HEADERS           += basics/gf-output-stream.hpp
HEADERS           += basics/gf-random.hpp
HEADERS           += basics/gf-xy.hpp

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

HEADERS           += basics/gf-analog.hpp

HEADERS           += ports/gf-port.hpp
HEADERS           += ports/gf-port-adapters.hpp
HEADERS           += ports/gf-port-dummies.hpp
HEADERS           += ports/gf-port-from-pins.hpp
HEADERS           += ports/gf-port-demos.hpp

HEADERS           += timing/gf-timing-wait.hpp
HEADERS           += timing/gf-timing-clock.hpp
HEADERS           += timing/gf-slow-down.hpp

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

HEADERS           += basics/gf-i2c.hpp
HEADERS           += basics/gf-spi.hpp

HEADERS           += chips/gf-servo.hpp
HEADERS           += chips/gf-hd44780.hpp
HEADERS           += chips/gf-ks0108.hpp
HEADERS           += chips/gf-hc595.hpp
HEADERS           += chips/gf-pcf8574.hpp
HEADERS           += chips/gf-pcf8591.hpp
HEADERS           += chips/gf-ssd1306.hpp
HEADERS           += chips/gf-nrf24l01.hpp
HEADERS           += chips/gf-max7219.hpp

HEADERS           += targets/gf-chip-atmega328.hpp
HEADERS           += targets/gf-chip-lpc1114.hpp
HEADERS           += targets/gf-chip-sam3x8e.hpp
HEADERS           += targets/gf-board-arduino-uno.hpp
HEADERS           += targets/gf-board-my-first-dev-board.hpp
HEADERS           += targets/gf-board-arduino-due.hpp
HEADERS           += targets/gf-board-db103.hpp
HEADERS           += targets/gf-native.hpp
