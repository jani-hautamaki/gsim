#*******************************{begin:header}*******************************#
#                       sundial - The Datetime Library                       #
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


# Target: libsundial
#######################

# Input
########

libsundial_INCDIR=src/libsundial/include
libsundial_SRCDIR=src/libsundial/src
libsundial_SOURCES=$(wildcard $(libsundial_SRCDIR)/*.c)

# Output
#########

libsundial_LIBDIR := build/lib
libsundial_OBJDIR := build/obj/libsundial
libsundial_LIBNAME := $(libsundial_LIBDIR)/libsundial.a
libsundial_OBJS := $(patsubst $(libsundial_SRCDIR)/%,$(libsundial_OBJDIR)/%,$(libsundial_SOURCES:.c=.o))

# Output generation
####################

libsundial_CFLAGS=-Wall -pedantic -std=c99 -I$(libsundial_INCDIR)
#libsundial_CPPFLAGS=



# The rule for constructing the library:
# All *.c files in the src/libsundial directory
# must be compiled into the library
#

_rebuild_library: $(libsundial_LIBNAME)

# Create libsundial.a
$(libsundial_LIBNAME): $(libsundial_OBJS) | $(libsundial_LIBDIR)
	$(AR) rs $(libsundial_LIBNAME) $(libsundial_OBJS)

# Pull in previously computed dependencies
-include $(libsundial_OBJS:.o=.d)

# Compile a single object
$(libsundial_OBJDIR)/%.o: $(libsundial_SRCDIR)/%.c | $(libsundial_OBJDIR)
	$(CC) -c $(libsundial_CFLAGS) $(libsundial_CPPFLAGS) -o $@ $<
	@$(CC) $(libsundial_CFLAGS) -MM -MT"$@" $< -MF"$(patsubst %.o,%.d,$@)"

# Directory creation.
# These are order-only prerequisities for other recipes

$(libsundial_OBJDIR):
	-mkdir -p $(libsundial_OBJDIR)

$(libsundial_LIBDIR):
	-mkdir -p $(libsundial_LIBDIR)



# Target: samples
##################

# Input
########

samples_INCDIR=src/libsundial/include
samples_SRCDIR=src/samples
samples_SOURCES=$(wildcard $(samples_SRCDIR)/*.c)
samples_LIBS=$(libsundial_LIBNAME)

# Output
#########

samples_OBJDIR=build/obj/samples
samples_OBJS=$(patsubst $(samples_SRCDIR)/%,$(samples_OBJDIR)/%,$(samples_SOURCES:.c=.o))

samples_BINDIR=build/samples
samples_BINS=$(patsubst $(samples_SRCDIR)/%,$(samples_BINDIR)/%,$(samples_SOURCES:.c=))

# Output generation
####################

samples_CFLAGS=-Wall -std=c99 -I$(libsundial_INCDIR)
samples_LDLIBS=-lm
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


