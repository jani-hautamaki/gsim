#*******************************{begin:header}*******************************#
#                       rrnx - The Robust RINEX Library                      #
#****************************************************************************#
#
#       Part of the GPS/INS measurement simulation system GSIM,
#       https://code.google.com/p/gsim
#
#       Copyright (C) 2013-2014 Jani Hautamaki <jani.hautamaki@hotmail.com>
#
#       Licensed under the terms of GNU General Public License v3.
#
#       You should have received a copy of the GNU General Public License v3
#       along with this program as the file LICENSE.txt; if not, please see
#       http://www.gnu.org/licenses/gpl-3.0.html
#
#*********************************{end:header}*******************************#

# Pull in template definitions
include ../Makefile.*

# Disable the default implicit rules (simplies debugging)
.SUFFIXES:

# Treat all targets as secondary (simplifies build)
.SECONDARY:

# List phony targets defined in this makefile
.PHONY: all clean doc

all: build_librrnx build_tests

clean: clean_librrnx clean_tests clean_doc

doc: | build/doc
	doxygen

.PHONY: clean_doc
clean_doc:
	rm -rf build/doc

# TODO:
#install: install_librrnx install_tests

# Programs / Tooling
CC=gcc
LD=gcc

# Arguments common for all compiler invocations
CFLAGS := -Wall --std=c99

# Compiler flags for test snippets
tests_CFLAGS := -I src/librrnx/include

# Self-contained dependency libraries
tests_LIBS := build/lib/librrnx.a

$(eval $(call LIBRARY_template,librrnx))
$(eval $(call SNIPPETS_template,tests))

$(eval $(call MKDIR_template))
$(eval $(call RMDIR_template))
