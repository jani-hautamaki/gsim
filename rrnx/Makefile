

# GNU Make documentation:
# http://www.gnu.org/software/make/manual/make.html
#
# For pre-defined variables, see
# http://www.gnu.org/software/make/manual/make.html#Implicit-Variables
# These can be cancelled with -R.



# Some directories
SRCDIR=src
OBJDIR=obj
LIBDIR=lib

# The library to craete
LIBNAME=$(LIBDIR)/librrnx.a

# Generate list of sources to compile
SOURCES=$(wildcard $(SRCDIR)/*.c)

# Generate list of objects
OBJS=$(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(SOURCES:.c=.o))

# Delete the default suffixes (same as "-r" or "--no-builtin-rules")
.SUFFIXES:

# Modify pre-defined variables
AR=ar
CC=gcc
CFLAGS=-c -Wall
LDFLAGS=

# targets which are not real
.PHONY: clean all

#############################################################################
# Actual receipes

all: $(OBJS) $(LIBNAME)

# Create a library
$(LIBNAME): $(OBJS)
	$(AR) rs $(LIBNAME) $(OBJS)

# Pull in dependency info for *existing* .o files.
-include $(OBJS:.o=.d)


$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR) $(LIBDIR)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<
	$(CC) $(CFLAGS) -MM -MT"$@" $< -MF"$(patsubst %.o,%.d,$@)"
	

# Directory creation; an order-only prerequisite
$(OBJDIR):
	-mkdir $(OBJDIR)

$(LIBDIR):
	-mkdir $(LIBDIR)

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




