.POSIX:

TARGETS = a4x $(a4x_OBJECTS) libfragments.a $(FRAGMENT_OBJS)

# Default build options. Override these by either passing the C compiler
# options via "CFLAGS=" directly as an argument on the "make" command line, or
# by "export MAKEFLAGS=e" first and then setting "export CFLAGS=". The same
# applies to CPPFLAGS (passed to the C preprocessor) and LDFLAGS (passed to
# the linker).
CPPFLAGS = -D NDEBUG
CFLAGS = -O
LDFLAGS = -s

# Additional flags added internally by the Makefile.
PRIVATE_CFLAGS = -I include
PRIVATE_LDFLAGS = -L .

a4x_OBJECTS = a4x.o

FRAGMENT_OBJS = getopt_simplest.o r4g.o

# Augmented flags which are actually used for compiling.
AUG_CFLAGS= $(CPPFLAGS) $(CFLAGS) $(PRIVATE_CFLAGS)
AUG_LDFLAGS= $(LDFLAGS) $(PRIVATE_LDFLAGS)

.PHONY: all clean

all: $(TARGETS)

clean:
	-rm $(TARGETS)

libfragments.a: $(FRAGMENT_OBJS)
	$(AR) $(ARFLAGS) $@ $(FRAGMENT_OBJS)

.c.o:
	$(CC) -o $@ $(AUG_CFLAGS) -c $<

a4x: a4x.o libfragments.a
	$(CC) -o $@ $(AUG_LDFLAGS) a4x.o -l fragments

# Dependencies detected with "gcc -MM -I . -I include", one per line, sorted.

a4x.o: \
 a4x.c include/r4g/r4g_u0ywydbuiziuzssqsi5l0mdid.h \
 include/dim_sdbrke8ae851uitgzm4nv3ea2.h \
 include/getopt_nh7lll77vb62ycgwzwf30zlln.h \

getopt_simplest.o: \
 getopt_simplest.c \
 include/getopt_nh7lll77vb62ycgwzwf30zlln.h \

r4g.o: \
 include/r4g/r4g_u0ywydbuiziuzssqsi5l0mdid.h \
 r4g.c \

# Be sure to keep the empty line before THIS line.
