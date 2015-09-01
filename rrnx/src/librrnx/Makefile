#*******************************{begin:header}*******************************#
#                       rrnx - The Robust RINEX Library                      #
#****************************************************************************#
#
#       Part of the GPS/INS measurement simulation system GSIM,
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

#
# References:
#
# [1] GNU Make documentation:
#     http://www.gnu.org/software/make/manual/make.html
#

# Delete the default suffixes (same as "-r" or "--no-builtin-rules")
.SUFFIXES:

# Output directories
BUILDDIR=../../build
OBJDIR=$(BUILDDIR)/obj/librrnx
LIBDIR=$(BUILDDIR)/lib
# Output files
LIBNAME=$(LIBDIR)/librrnx.a


# Input directory
SRCDIR=src


# Gather all source code files into a single list.
SOURCES=$(wildcard $(SRCDIR)/*.c)

# Compute dynamically list of objects that need to be compiled.
OBJS=$(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.c=.o))
# Maybe also:
# $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Modify pre-defined variables
AR=ar
CC=gcc
CFLAGS=-c -Wall --std=c99 -Iinclude
LDFLAGS=

# TODO:
#LINKER   = gcc -o
# linking flags here
#LFLAGS   = -Wall -I. -lm



# targets which are not real
.PHONY: clean all

#############################################################################
# Actual receipes

# Why there are objs here?
all: $(LIBNAME)

# Create a library
$(LIBNAME): $(OBJS) | $(LIBDIR)
	$(AR) rs $(LIBNAME) $(OBJS)


#############################################################################
# Pull in dependencies for the .o files that are already built.
# Enables the compiler to know when an existing .o file needs rebuild.
#

-include $(OBJS:.o=.d)


#############################################################################
# General pattern for objects:
#   1) Compile the file,
#   2) Calculate dependencies for the next run.
#
# Note:
#   "Note that .d files exist if and only if the corresponding .o file exists. 
#    This makes sense, since if the .o file doesn't exist yet, 
#    we don't need a .d file to tell us it has to be rebuilt."
#    -from http://scottmcpeak.com/autodepend/autodepend.html
#
# Special symbols used here:
#   $<    is the source file
#   $@    is the target file
#

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<
	@$(CC) $(CFLAGS) -MM -MT"$@" $< -MF"$(patsubst %.o,%.d,$@)"
	

# Directory creation; an order-only prerequisite
$(OBJDIR):
	-mkdir -p $(OBJDIR)

$(LIBDIR):
	-mkdir -p $(LIBDIR)

# Remove compilation products
clean:
	rm -rf $(OBJDIR) $(LIBDIR)


# http://stackoverflow.com/questions/97338/gcc-dependency-generation-for-a-different-output-directory
# automatically generate dependency rules
#%.d : %.c
#	$(CC) $(CFLAGS) -MF"$@" -MG -MM -MP -MT"$@" -MT"$(<:.c=.o)" "$<"
# -MF  write the generated dependency rule to a file
# -MG  assume missing headers will be generated and don't stop with an error
# -MM  generate dependency rule for prerequisite, skipping system headers
# -MP  add phony target for each header to prevent errors when header is missing
# -MT  add a target to the generated dependency




