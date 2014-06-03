#*******************************{begin:header}*******************************#
#               grease - The Grease Utilities for Applications               #
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

_default: _rebuild_library _rebuild_samples


# Target: libgrease
#######################

# Input
########

libgrease_INCDIR := src/libgrease/include
libgrease_SRCDIR := src/libgrease/src
libgrease_SOURCES := $(wildcard $(libgrease_SRCDIR)/*.c)

# Output
#########

libgrease_LIBDIR := build/lib
libgrease_OBJDIR := build/obj/libgrease
libgrease_LIBNAME := $(libgrease_LIBDIR)/libgrease.a
libgrease_OBJS := $(patsubst $(libgrease_SRCDIR)/%,$(libgrease_OBJDIR)/%,$(libgrease_SOURCES:.c=.o))

# Output generation
####################

libgrease_CFLAGS=-Wall -std=c99 -I$(libgrease_INCDIR)
#libgrease_CPPFLAGS=



# The rule for constructing the library:
# All *.c files in the src/libgrease directory
# must be compiled into the library
#

_rebuild_library: $(libgrease_LIBNAME)

# Create libgrease.a
$(libgrease_LIBNAME): $(libgrease_OBJS) | $(libgrease_LIBDIR)
	$(AR) rs $(libgrease_LIBNAME) $(libgrease_OBJS)

# Pull in previously computed dependencies
-include $(libgrease_OBJS:.o=.d)

# Compile a single object
$(libgrease_OBJDIR)/%.o: $(libgrease_SRCDIR)/%.c | $(libgrease_OBJDIR)
	$(CC) -c $(libgrease_CFLAGS) $(libgrease_CPPFLAGS) -o $@ $<
	@$(CC) $(libgrease_CFLAGS) -MM -MT"$@" $< -MF"$(patsubst %.o,%.d,$@)"

# Directory creation.
# These are order-only prerequisities for other recipes

$(libgrease_OBJDIR):
	-mkdir -p $(libgrease_OBJDIR)

$(libgrease_LIBDIR):
	-mkdir -p $(libgrease_LIBDIR)



# Target: samples
##################

# Input
########

samples_INCDIR=src/libgrease/include
samples_SRCDIR=src/samples
samples_SOURCES=$(wildcard $(samples_SRCDIR)/*.c)
samples_LIBS=$(libgrease_LIBNAME)

# Output
#########

samples_OBJDIR=build/obj/samples
samples_OBJS=$(patsubst $(samples_SRCDIR)/%,$(samples_OBJDIR)/%,$(samples_SOURCES:.c=.o))

samples_BINDIR=build/samples
samples_BINS=$(patsubst $(samples_SRCDIR)/%,$(samples_BINDIR)/%,$(samples_SOURCES:.c=))

# Output generation
####################

samples_CFLAGS=-Wall -std=c99 -I$(libgrease_INCDIR)
samples_LDLIBS=
samples_LDFLAGS=

# Keep all intermediate files 
.SECONDARY: $(samples_OBJS)

# Recipes
##########

#_rebuild_samples:
_rebuild_samples: $(samples_BINS)

# Pull in previously computed dependencies
-include $(samples_OBJS:.o=.d)

# Compile a single object
$(samples_OBJDIR)/%.o: $(samples_SRCDIR)/%.c | $(samples_OBJDIR)
	$(CC) -c $(samples_CFLAGS) $(samples_CPPFLAGS) -o $@ $<
	@$(CC) $(samples_CFLAGS) -MM -MT"$@" $< -MF"$(patsubst %.o,%.d,$@)"


# Link a single binary
$(samples_BINDIR)/%: $(samples_OBJDIR)/%.o $(samples_LIBS) | $(samples_BINDIR)
	$(LD) $(samples_LDFLAGS) $^ $(samples_LDLIBS) -o $@


$(samples_OBJDIR):
	-mkdir -p $(samples_OBJDIR)

$(samples_BINDIR):
	-mkdir -p $(samples_BINDIR)




# Target: clean
##################

clean:
	rm -rf build


