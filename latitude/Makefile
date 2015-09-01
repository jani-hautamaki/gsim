#*******************************{begin:header}*******************************#
#                   latitude - The Ellipsoid Math Library                    #
#****************************************************************************#
#
#       Part of the GPS/INS measurement simulation system GSIM
#       https://github.com/jani-hautamaki/gsim
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


# Target: liblatitude
#######################

# Input
########

liblatitude_INCDIR := src/liblatitude/include
liblatitude_SRCDIR := src/liblatitude/src
liblatitude_SOURCES := $(wildcard $(liblatitude_SRCDIR)/*.c)

# Output
#########

liblatitude_LIBDIR := build/lib
liblatitude_OBJDIR := build/obj/liblatitude
liblatitude_LIBNAME := $(liblatitude_LIBDIR)/liblatitude.a
liblatitude_OBJS := $(patsubst $(liblatitude_SRCDIR)/%,$(liblatitude_OBJDIR)/%,$(liblatitude_SOURCES:.c=.o))

# Output generation
####################

liblatitude_CFLAGS=-Wall -std=c99 -I$(liblatitude_INCDIR)
#liblatitude_CPPFLAGS=



# The rule for constructing the library:
# All *.c files in the src/liblatitude directory
# must be compiled into the library
#

_rebuild_library: $(liblatitude_LIBNAME)

# Create liblatitude.a
$(liblatitude_LIBNAME): $(liblatitude_OBJS) | $(liblatitude_LIBDIR)
	$(AR) rs $(liblatitude_LIBNAME) $(liblatitude_OBJS)

# Pull in previously computed dependencies
-include $(liblatitude_OBJS:.o=.d)

# Compile a single object
$(liblatitude_OBJDIR)/%.o: $(liblatitude_SRCDIR)/%.c | $(liblatitude_OBJDIR)
	$(CC) -c $(liblatitude_CFLAGS) $(liblatitude_CPPFLAGS) -o $@ $<
	@$(CC) $(liblatitude_CFLAGS) -MM -MT"$@" $< -MF"$(patsubst %.o,%.d,$@)"

# Directory creation.
# These are order-only prerequisities for other recipes

$(liblatitude_OBJDIR):
	-mkdir -p $(liblatitude_OBJDIR)

$(liblatitude_LIBDIR):
	-mkdir -p $(liblatitude_LIBDIR)



# Target: samples
##################

# Input
########

samples_INCDIR=src/liblatitude/include
samples_SRCDIR=src/samples
samples_SOURCES=$(wildcard $(samples_SRCDIR)/*.c)
samples_LIBS=$(liblatitude_LIBNAME)

# Output
#########

samples_OBJDIR=build/obj/samples
samples_OBJS=$(patsubst $(samples_SRCDIR)/%,$(samples_OBJDIR)/%,$(samples_SOURCES:.c=.o))

samples_BINDIR=build/samples
samples_BINS=$(patsubst $(samples_SRCDIR)/%,$(samples_BINDIR)/%,$(samples_SOURCES:.c=))

# Output generation
####################

samples_CFLAGS=-Wall -std=c99 -I$(liblatitude_INCDIR)
samples_LDLIBS=-lsundial
samples_LDFLAGS=-L $(liblatitude_LIBDIR)

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


