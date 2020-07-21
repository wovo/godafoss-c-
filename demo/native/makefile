################################################################################
#
# makefile
#
################################################################################
#
# This makefile invokes make on each subdirectory
#
################################################################################
#
# (c) Wouter van Ooijen (www.voti.nl) 2020
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)
#
################################################################################

# https://stackoverflow.com/questions/17834582

SUBDIRS := $(wildcard */.)

.PHONY: build clean run $(SUBDIRS)

build: $(SUBDIRS)
clean: $(SUBDIRS)
run: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)
