#*******************************{begin:header}*******************************#
#                gsim - GPS/INS measurement simulation system                #
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

.SUFFIXES:

# tooling
##########

CC=gcc
AR=ar
LD=gcc

# general config
#################

BUILDDIR := build
BINDIR := $(BUILDDIR)/bin
OBJDIR := $(BUILDDIR)/obj
LIBDIR := $(BUILDDIR)/lib
SRCDIR := src


# general targets
##################

.PHONY: _rebuild_all _rebuild_navmssgen _rebuild_statgen _rebuild_libgsim

_rebuild_all: _rebuild_navmsggen _rebuild_statgen _rebuild_libgsim


# navmsggen input
##################

navmsggen_SRCDIR := $(SRCDIR)/navmsggen
navmsggen_SOURCES := $(wildcard $(navmsggen_SRCDIR)/*.c)

navmsggen_INCDIR := ../rrnx/src/librrnx/include ../cgenerics/src/libcgenerics/include ../coregps/src/libcoregps/include ../grease/src/libgrease/include
navmsggen_LIBS := ../rrnx/build/lib/librrnx.a ../cgenerics/build/lib/libcgenerics.a ../coregps/build/lib/libcoregps.a ../grease/build/lib/libgrease.a

# navmsggen output
###################

navmsggen_OBJDIR := $(OBJDIR)/navmsggen
navmsggen_OBJS := $(patsubst $(navmsggen_SRCDIR)/%,$(navmsggen_OBJDIR)/%,$(navmsggen_SOURCES:.c=.o))

navmsggen_BIN := $(BINDIR)/navmsggen

# navmsggen output generation
##############################

navmsggen_CFLAGS := -Wall -std=c99 $(addprefix -I ,$(navmsggen_INCDIR))
navmsggen_LDLIBS :=
navmsggen_LDFLAGS :=

# navmssgen recipes
####################

_rebuild_navmsggen: $(navmsggen_BIN)

# Pull in previously computed dependencies
-include $(navmsggen_OBJS:.o=.d)

# Compile a single object
$(navmsggen_OBJDIR)/%.o: $(navmsggen_SRCDIR)/%.c | $(navmsggen_OBJDIR)
	$(CC) -c $(navmsggen_CFLAGS) $(navmsggen_CPPFLAGS) -o $@ $<
	@$(CC) $(navmsggen_CFLAGS) -MM -MT"$@" $< -MF"$(patsubst %.o,%.d,$@)"

# Link a single binary
$(navmsggen_BIN): $(navmsggen_OBJS) $(navmsggen_LIBS) | $(BINDIR)
	$(LD) $(navmsggen_LDFLAGS) $^ $(navmsggen_LDLIBS) -o $@


# Create directory
$(navmsggen_OBJDIR):
	-mkdir -p $(navmsggen_OBJDIR)







# statgen input
################

statgen_SRCDIR := $(SRCDIR)/statgen
statgen_SOURCES := $(wildcard $(statgen_SRCDIR)/*.c)

statgen_INCDIR := ../latitude/src/liblatitude/include ../grease/src/libgrease/include ../maid/src/libmaid/include ../euclid/src/libeuclid/include src/libgsim/include
statgen_LIBS := ../latitude/build/lib/liblatitude.a ../grease/build/lib/libgrease.a ../maid/build/lib/libmaid.a ../euclid/build/lib/libeuclid.a build/lib/libgsim.a

# statgen output
#################

statgen_OBJDIR := $(OBJDIR)/statgen
statgen_OBJS := $(patsubst $(statgen_SRCDIR)/%,$(statgen_OBJDIR)/%,$(statgen_SOURCES:.c=.o))

statgen_BIN := $(BINDIR)/statgen

# statgen output generation
##############################

statgen_CFLAGS := -Wall -std=c99 $(addprefix -I ,$(statgen_INCDIR))
statgen_LDLIBS :=
statgen_LDFLAGS := -lm

# statgen recipes
####################

_rebuild_statgen: $(statgen_BIN)

# Pull in previously computed dependencies
-include $(statgen_OBJS:.o=.d)

# Compile a single object
$(statgen_OBJDIR)/%.o: $(statgen_SRCDIR)/%.c | $(statgen_OBJDIR)
	$(CC) -c $(statgen_CFLAGS) $(statgen_CPPFLAGS) -o $@ $<
	@$(CC) $(statgen_CFLAGS) -MM -MT"$@" $< -MF"$(patsubst %.o,%.d,$@)"

# Link a single binary
$(statgen_BIN): $(statgen_OBJS) $(statgen_LIBS) | $(BINDIR)
	$(LD) $(statgen_LDFLAGS) $^ $(statgen_LDLIBS) -o $@


# Create directory
$(statgen_OBJDIR):
	-mkdir -p $(statgen_OBJDIR)




# TARGET: libgsim
##################

# libgsim input
################

libgsim_INCDIR := $(SRCDIR)/libgsim/include
libgsim_SRCDIR := $(SRCDIR)/libgsim/src
libgsim_SOURCES := $(wildcard $(libgsim_SRCDIR)/*.c)

# libgsim output
#################

libgsim_LIBDIR := $(LIBDIR)
libgsim_OBJDIR := $(OBJDIR)/libgsim
libgsim_LIBNAME := $(libgsim_LIBDIR)/libgsim.a
libgsim_OBJS := $(patsubst $(libgsim_SRCDIR)/%,$(libgsim_OBJDIR)/%,$(libgsim_SOURCES:.c=.o))

# libgsim output generation
############################

libgsim_CFLAGS=-Wall -std=c99 -I$(liblatitude_INCDIR)

_rebuild_libgsim: $(libgsim_LIBNAME)

$(libgsim_LIBNAME): $(libgsim_OBJS) | $(libgsim_LIBDIR)
	$(AR) rs $(libgsim_LIBNAME) $(libgsim_OBJS)

# Pull in previously computed dependencies
-include $(libgsim_OBJS:.o=.d)

# Compile a single object
$(libgsim_OBJDIR)/%.o: $(libgsim_SRCDIR)/%.c | $(libgsim_OBJDIR)
	$(CC) -c $(libgsim_CFLAGS) $(libgsim_CPPFLAGS) -o $@ $<
	@$(CC) $(libgsim_CFLAGS) -MM -MT"$@" $< -MF"$(patsubst %.o,%.d,$@)"

# Directory creation.
# These are order-only prerequisities for other recipes

$(libgsim_OBJDIR):
	-mkdir -p $(libgsim_OBJDIR)

$(libgsim_LIBDIR):
	-mkdir -p $(libgsim_LIBDIR)






# recipes common to all
########################


# Create directory
$(BINDIR):
	-mkdir -p $(BINDIR)

clean:
	rm -rf build


