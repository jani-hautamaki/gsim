#*******************************{begin:header}*******************************#
#            cgenerics - The C Library for Generic Data Structures           #
#*******************************{begin:header}*******************************#
#
#       Part of the GPS/INS measurement simulation system GSIM
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

# Default action

.PHONY: _default _rebuild_library _rebuild_tests clean


# Tooling
##########
CC=gcc
AR=ar


# Input
########

libcgenerics_INCDIR=src/libcgenerics/include
libcgenerics_SRCDIR=src/libcgenerics/src
libcgenerics_SOURCES=$(wildcard $(libcgenerics_SRCDIR)/*.c)

# Output
#########

libcgenerics_LIBDIR=build/lib
libcgenerics_OBJDIR=build/obj/libcgenerics
libcgenerics_LIBNAME=$(libcgenerics_LIBDIR)/libcgenerics.a
libcgenerics_OBJS=$(patsubst $(libcgenerics_SRCDIR)/%,$(libcgenerics_OBJDIR)/%,$(libcgenerics_SOURCES:.c=.o))

# Output generation
####################

libcgenerics_CFLAGS=-Wall -std=c99 -I$(libcgenerics_INCDIR)
#libcgenerics_CPPFLAGS=



# Actions
##########

_default: _rebuild_library _rebuild_tests

_rebuild_library: $(libcgenerics_LIBNAME)



# The rule for constructing the library:
# All *.c files in the src/libcgenerics directory
# must be compiled into the library
#

# Create libcgenerics.a
$(libcgenerics_LIBNAME): $(libcgenerics_OBJS) | $(libcgenerics_LIBDIR)
	$(AR) rs $(libcgenerics_LIBNAME) $(libcgenerics_OBJS)

-include $(libcgenerics_OBJS:.o=.d)




# Compile a single object
$(libcgenerics_OBJDIR)/%.o: $(libcgenerics_SRCDIR)/%.c | $(libcgenerics_OBJDIR)
	$(CC) -c $(libcgenerics_CFLAGS) $(libcgenerics_CPPFLAGS) -o $@ $<
	@$(CC) $(libcgenerics_CFLAGS) -MM -MT"$@" $< -MF"$(patsubst %.o,%.d,$@)"


# Directory creation. 
# These are order-only prerequisities for other recipes

$(libcgenerics_OBJDIR):
	-mkdir -p $(libcgenerics_OBJDIR)

$(libcgenerics_LIBDIR):
	-mkdir -p $(libcgenerics_LIBDIR)




clean:
	rm -rf build
#	echo rm -rf $(libcgenerics_OBJDIR) $(libcgenerics_LIBDIR)


