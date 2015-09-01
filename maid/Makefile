#*******************************{begin:header}*******************************#
#             maid - The Mathematical Aid for Miscellaneous Tasks            #
#****************************************************************************#
#
#       Part of the GPS/INS measurement simulation system GSIM
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

libmaid_INCDIR := src/libmaid/include
libmaid_SRCDIR := src/libmaid/src
libmaid_SOURCES := $(wildcard $(libmaid_SRCDIR)/*.c)

# Output
#########

libmaid_LIBDIR := build/lib
libmaid_OBJDIR := build/obj/libmaid
libmaid_LIBNAME := $(libmaid_LIBDIR)/libmaid.a
libmaid_OBJS := $(patsubst $(libmaid_SRCDIR)/%,$(libmaid_OBJDIR)/%,$(libmaid_SOURCES:.c=.o))

# Output generation
####################

libmaid_CFLAGS=-Wall -std=c99 -I$(libmaid_INCDIR)
#libmaid_CPPFLAGS=



# The rule for constructing the library:
# All *.c files in the src/libmaid directory
# must be compiled into the library
#

_rebuild_library: $(libmaid_LIBNAME)

# Create liblatitude.a
$(libmaid_LIBNAME): $(libmaid_OBJS) | $(libmaid_LIBDIR)
	$(AR) rs $(libmaid_LIBNAME) $(libmaid_OBJS)

# Pull in previously computed dependencies
-include $(libmaid_OBJS:.o=.d)

# Compile a single object
$(libmaid_OBJDIR)/%.o: $(libmaid_SRCDIR)/%.c | $(libmaid_OBJDIR)
	$(CC) -c $(libmaid_CFLAGS) $(libmaid_CPPFLAGS) -o $@ $<
	@$(CC) $(libmaid_CFLAGS) -MM -MT"$@" $< -MF"$(patsubst %.o,%.d,$@)"

# Directory creation.
# These are order-only prerequisities for other recipes

$(libmaid_OBJDIR):
	-mkdir -p $(libmaid_OBJDIR)

$(libmaid_LIBDIR):
	-mkdir -p $(libmaid_LIBDIR)



# Target: samples
##################

# Input
########

samples_INCDIR=src/libmaid/include
samples_SRCDIR=src/samples
samples_SOURCES=$(wildcard $(samples_SRCDIR)/*.c)
samples_LIBS=$(libmaid_LIBNAME)

# Output
#########

samples_OBJDIR=build/obj/samples
samples_OBJS=$(patsubst $(samples_SRCDIR)/%,$(samples_OBJDIR)/%,$(samples_SOURCES:.c=.o))

samples_BINDIR=build/samples
samples_BINS=$(patsubst $(samples_SRCDIR)/%,$(samples_BINDIR)/%,$(samples_SOURCES:.c=))

# Output generation
####################

samples_CFLAGS=-Wall -std=c99 -I$(libmaid_INCDIR)
samples_LDLIBS=-lsundial
samples_LDFLAGS=-L $(libmaid_LIBDIR)

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


