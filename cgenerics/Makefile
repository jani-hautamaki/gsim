#*******************************{begin:header}*******************************#
#            cgenerics - The C Library for Generic Data Structures           #
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

_rebuild_library: $(libcgenerics_LIBNAME)




# Target: libcgenerics
#######################

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



# The rule for constructing the library:
# All *.c files in the src/libcgenerics directory
# must be compiled into the library
#

# Create libcgenerics.a
$(libcgenerics_LIBNAME): $(libcgenerics_OBJS) | $(libcgenerics_LIBDIR)
	$(AR) rs $(libcgenerics_LIBNAME) $(libcgenerics_OBJS)

# Pull in previously computed dependencies
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



# Target: samples
##################

# Input
########

samples_INCDIR=src/libcgenerics/include
samples_SRCDIR=src/samples
samples_SOURCES=$(wildcard $(samples_SRCDIR)/*.c)
samples_LIBS=$(libcgenerics_LIBNAME)

# Output
#########

samples_OBJDIR=build/obj/samples
samples_OBJS=$(patsubst $(samples_SRCDIR)/%,$(samples_OBJDIR)/%,$(samples_SOURCES:.c=.o))

samples_BINDIR=build/samples
samples_BINS=$(patsubst $(samples_SRCDIR)/%,$(samples_BINDIR)/%,$(samples_SOURCES:.c=))

# Output generation
####################

samples_CFLAGS=-Wall -std=c99 -I$(libcgenerics_INCDIR)
samples_LDLIBS=-lcgenerics
samples_LDFLAGS=-L $(libcgenerics_LIBDIR)

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


