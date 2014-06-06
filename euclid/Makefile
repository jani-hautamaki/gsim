#*******************************{begin:header}*******************************#
#                 euclid - The C Library for Linear Algebra                  #
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

# Target: libeuclid
#######################

# Input
########

libeuclid_INCDIR := src/libeuclid/include
libeuclid_SRCDIR := src/libeuclid/src
libeuclid_SOURCES := $(wildcard $(libeuclid_SRCDIR)/*.c)

# Output
#########

libeuclid_LIBDIR := build/lib
libeuclid_OBJDIR := build/obj/libeuclid
libeuclid_LIBNAME := $(libeuclid_LIBDIR)/libeuclid.a
libeuclid_OBJS := $(patsubst $(libeuclid_SRCDIR)/%,$(libeuclid_OBJDIR)/%,$(libeuclid_SOURCES:.c=.o))

# Output generation
####################

libeuclid_CFLAGS=-Wall -std=c99 -I$(libeuclid_INCDIR)
#libeuclid_CPPFLAGS=



# The rule for constructing the library:
# All *.c files in the src/libeuclid directory
# must be compiled into the library
#

_rebuild_library: $(libeuclid_LIBNAME)

# Create libeuclid.a
$(libeuclid_LIBNAME): $(libeuclid_OBJS) | $(libeuclid_LIBDIR)
	$(AR) rs $(libeuclid_LIBNAME) $(libeuclid_OBJS)

# Pull in previously computed dependencies
-include $(libeuclid_OBJS:.o=.d)

# Compile a single object
$(libeuclid_OBJDIR)/%.o: $(libeuclid_SRCDIR)/%.c | $(libeuclid_OBJDIR)
	$(CC) -c $(libeuclid_CFLAGS) $(libeuclid_CPPFLAGS) -o $@ $<
	@$(CC) $(libeuclid_CFLAGS) -MM -MT"$@" $< -MF"$(patsubst %.o,%.d,$@)"

# Directory creation.
# These are order-only prerequisities for other recipes

$(libeuclid_OBJDIR):
	-mkdir -p $(libeuclid_OBJDIR)

$(libeuclid_LIBDIR):
	-mkdir -p $(libeuclid_LIBDIR)



# Target: samples
##################

# Input
########

samples_INCDIR=src/libeuclid/include
samples_SRCDIR=src/samples
samples_SOURCES=$(wildcard $(samples_SRCDIR)/*.c)
samples_LIBS=$(libeuclid_LIBNAME)

# Output
#########

samples_OBJDIR=build/obj/samples
samples_OBJS=$(patsubst $(samples_SRCDIR)/%,$(samples_OBJDIR)/%,$(samples_SOURCES:.c=.o))

samples_BINDIR=build/samples
samples_BINS=$(patsubst $(samples_SRCDIR)/%,$(samples_BINDIR)/%,$(samples_SOURCES:.c=))

# Output generation
####################

samples_CFLAGS=-Wall -std=c99 -I$(libeuclid_INCDIR)
samples_LDLIBS=
samples_LDFLAGS=-lm

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


