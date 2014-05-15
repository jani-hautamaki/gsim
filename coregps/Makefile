#*******************************{begin:header}*******************************#
#          coregps - Fundamental GPS data structures and algorithms
#****************************************************************************#
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

.PHONY: _default _rebuild_library _rebuild_samples clean


# Tooling
##########
CC=gcc
AR=ar
LD=gcc


# Actions
##########

#_default: _rebuild_library _rebuild_samples
_default: _rebuild_library

# Target: ligbcoregps
#######################

# Input
########

libcoregps_INCDIR=src/libcoregps/include
libcoregps_SRCDIR=src/libcoregps/src
libcoregps_SOURCES=$(wildcard $(libcoregps_SRCDIR)/*.c)

# Output
#########

libcoregps_LIBDIR=build/lib
libcoregps_OBJDIR=build/obj/libcoregps
libcoregps_LIBNAME=$(libcoregps_LIBDIR)/libcoregps.a
libcoregps_OBJS=$(patsubst $(libcoregps_SRCDIR)/%,$(libcoregps_OBJDIR)/%,$(libcoregps_SOURCES:.c=.o))

# Output generation
####################

libcoregps_CFLAGS=-Wall -std=c99 -I$(libcoregps_INCDIR)
#libcoregps_CPPFLAGS=


# Recipes
##########

_rebuild_library: $(libcoregps_LIBNAME)


# The rule for constructing the library:
# All *.c files in the src/ligbcoregps directory
# must be compiled into the library
#


$(libcoregps_LIBNAME): $(libcoregps_OBJS) | $(libcoregps_LIBDIR)
	$(AR) rs $(libcoregps_LIBNAME) $(libcoregps_OBJS)

# Pull in previously computed dependencies
-include $(libcoregps_OBJS:.o=.d)

# Compile a single object
$(libcoregps_OBJDIR)/%.o: $(libcoregps_SRCDIR)/%.c | $(libcoregps_OBJDIR)
	$(CC) -c $(libcoregps_CFLAGS) $(libcoregps_CPPFLAGS) -o $@ $<
	@$(CC) $(libcoregps_CFLAGS) -MM -MT"$@" $< -MF"$(patsubst %.o,%.d,$@)"

# Directory creation.
# These are order-only prerequisities for other recipes

$(libcoregps_OBJDIR):
	-mkdir -p $(libcoregps_OBJDIR)

$(libcoregps_LIBDIR):
	-mkdir -p $(libcoregps_LIBDIR)


# Target: _rebuild_samples
###########################

_rebuild_samples:



# Target: clean
##################

clean:
	rm -rf build


