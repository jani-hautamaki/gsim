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

#
# References:
#
# [1] GNU Make documentation:
#     http://www.gnu.org/software/make/manual/make.html
#

.PHONY: all doc src


all: src


src:
	$(MAKE) -C src


doc: | build/doc
	doxygen

build/doc:
	mkdir -p build/doc


clean:
	rm -rf build/doc
	$(MAKE) -C src clean
