#*******************************{begin:header}*******************************#
#                   gsim - GPS/INS measurement simulation                    #
#****************************************************************************#
#
#       Part of the GPS/INS measurement simulation system GSIM,
#       https://github.com/jani-hautamaki/gsim
#
#       Copyright (C) 2013-2015 Jani Hautamaki <jani.hautamaki@hotmail.com>
#
#       Licensed under the terms of GNU General Public License v3.
#
#       You should have received a copy of the GNU General Public License v3
#       along with this program as the file LICENSE.txt; if not, please see
#       http://www.gnu.org/licenses/gpl-3.0.html
#
#*********************************{end:header}*******************************#

# Recursive

# List of subdirectories to make
# "tests" depend on "librrnx"
SUBDIRS=librrnx tests

.PHONY: all $(SUBDIRS) clean
     
all: $(SUBDIRS)

librrnx:
	$(MAKE) -C librrnx

# librrnx must be built prior to building tests
tests: | librrnx
	$(MAKE) -C tests

clean:
	$(MAKE) -C librrnx clean
	$(MAKE) -C tests clean

