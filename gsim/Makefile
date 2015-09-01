#*******************************{begin:header}*******************************#
#                gsim - GPS/INS measurement simulation system                #
#****************************************************************************#
#
#       Part of the GPS/INS measurement simulation system GSIM,
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

.PHONY: _rebuild_all _rebuild_navmssgen _rebuild_statgen _rebuild_libgsim _rebuild_msgdump _rebuild_measgen

_rebuild_all: _rebuild_navmsggen _rebuild_statgen _rebuild_libgsim _rebuild_msgdump _rebuild_measgen

# dependencies for libgsim
###########################

libgsim_INCDIR := $(SRCDIR)/libgsim/include ../grease/src/libgrease/include ../coregps/src/libcoregps/include ../sundial/src/libsundial/include
libgsim_LIBS := $(LIBDIR)/libgsim.a ../grease/build/lib/libgrease.a ../coregps/build/lib/libcoregps.a ../sundial/build/lib/libsundial.a 


# navmsggen input
##################

navmsggen_SRCDIR := $(SRCDIR)/navmsggen
navmsggen_SOURCES := $(wildcard $(navmsggen_SRCDIR)/*.c)

navmsggen_INCDIR := $(libgsim_INCDIR) ../rrnx/src/librrnx/include ../cgenerics/src/libcgenerics/include
navmsggen_LIBS := $(libgsim_LIBS) ../rrnx/build/lib/librrnx.a ../cgenerics/build/lib/libcgenerics.a

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

statgen_INCDIR := $(libgsim_INCDIR) ../latitude/src/liblatitude/include ../maid/src/libmaid/include ../euclid/src/libeuclid/include
statgen_LIBS := $(libgsim_LIBS) ../latitude/build/lib/liblatitude.a ../maid/build/lib/libmaid.a ../euclid/build/lib/libeuclid.a build/lib/libgsim.a

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




# TARGET: msgdump
##################

# msgdump input
################

msgdump_SRCDIR := $(SRCDIR)/msgdump
msgdump_SOURCES := $(wildcard $(msgdump_SRCDIR)/*.c)

msgdump_INCDIR := $(libgsim_INCDIR)
msgdump_LIBS := $(libgsim_LIBS)

# msgdump output
#################

msgdump_OBJDIR := $(OBJDIR)/msgdump
msgdump_OBJS := $(patsubst $(msgdump_SRCDIR)/%,$(msgdump_OBJDIR)/%,$(msgdump_SOURCES:.c=.o))

msgdump_BIN := $(BINDIR)/msgdump

# msgdump output generation
############################

msgdump_CFLAGS := -Wall -std=c99 $(addprefix -I ,$(msgdump_INCDIR))
msgdump_LDLIBS :=
msgdump_LDFLAGS := -lm

# msgdump recipes
##################

_rebuild_msgdump: $(msgdump_BIN)

# Pull in previously computed dependencies
-include $(msgdump_OBJS:.o=.d)

# Compile a single object
$(msgdump_OBJDIR)/%.o: $(msgdump_SRCDIR)/%.c | $(msgdump_OBJDIR)
	$(CC) -c $(msgdump_CFLAGS) $(msgdump_CPPFLAGS) -o $@ $<
	@$(CC) $(msgdump_CFLAGS) -MM -MT"$@" $< -MF"$(patsubst %.o,%.d,$@)"

# Link a single binary
$(msgdump_BIN): $(msgdump_OBJS) $(msgdump_LIBS) | $(BINDIR)
	$(LD) $(msgdump_LDFLAGS) $^ $(msgdump_LDLIBS) -o $@


# Create directory
$(msgdump_OBJDIR):
	-mkdir -p $(msgdump_OBJDIR)




# TARGET: measgen
##################

# measgen input
################

measgen_SRCDIR := $(SRCDIR)/measgen
measgen_SOURCES := $(wildcard $(measgen_SRCDIR)/*.c)

measgen_INCDIR := $(libgsim_INCDIR)
measgen_LIBS := $(libgsim_LIBS)

# measgen output
#################

measgen_OBJDIR := $(OBJDIR)/measgen
measgen_OBJS := $(patsubst $(measgen_SRCDIR)/%,$(measgen_OBJDIR)/%,$(measgen_SOURCES:.c=.o))

measgen_BIN := $(BINDIR)/measgen

# measgen output generation
############################

measgen_CFLAGS := -Wall -std=c99 $(addprefix -I ,$(measgen_INCDIR))
measgen_LDLIBS :=
measgen_LDFLAGS := -lm

# measgen recipes
##################

_rebuild_measgen: $(measgen_BIN)

# Pull in previously computed dependencies
-include $(measgen_OBJS:.o=.d)

# Compile a single object
$(measgen_OBJDIR)/%.o: $(measgen_SRCDIR)/%.c | $(measgen_OBJDIR)
	$(CC) -c $(measgen_CFLAGS) $(measgen_CPPFLAGS) -o $@ $<
	@$(CC) $(measgen_CFLAGS) -MM -MT"$@" $< -MF"$(patsubst %.o,%.d,$@)"

# Link a single binary
$(measgen_BIN): $(measgen_OBJS) $(measgen_LIBS) | $(BINDIR)
	$(LD) $(measgen_LDFLAGS) $^ $(measgen_LDLIBS) -o $@


# Create directory
$(measgen_OBJDIR):
	-mkdir -p $(measgen_OBJDIR)




# TARGET: libgsim
##################

# libgsim input
################

libgsim_INCDIR := $(SRCDIR)/libgsim/include ../grease/src/libgrease/include ../coregps/src/libcoregps/include ../sundial/src/libsundial/include
libgsim_SRCDIR := $(SRCDIR)/libgsim/src
libgsim_SOURCES := $(wildcard $(libgsim_SRCDIR)/*.c)

libgsim_LIBS := $(LIBDIR)/libgsim.a ../libgrease/build/lib/libgrease.a ../coregps/build/libcoregps/libcoregps.a ../sundial/build/lib/libsundial.a 

# libgsim output
#################

libgsim_LIBDIR := $(LIBDIR)
libgsim_OBJDIR := $(OBJDIR)/libgsim
libgsim_LIBNAME := $(libgsim_LIBDIR)/libgsim.a ../coregps/build/lib/libcoregps.a
libgsim_OBJS := $(patsubst $(libgsim_SRCDIR)/%,$(libgsim_OBJDIR)/%,$(libgsim_SOURCES:.c=.o))

# libgsim output generation
############################

libgsim_CFLAGS := -Wall -std=c99 $(addprefix -I ,$(libgsim_INCDIR))

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



