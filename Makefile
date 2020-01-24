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
PRIVATE_CFLAGS = -I .
PRIVATE_LDFLAGS = -L .

a4x_OBJECTS = a4x.o

FRAGMENT_OBJS =

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
	$(CC) -o $@ $(AUG_CFLAGS) $<

a4x: a4x.o libfragments.a
	$(CC) -o $@ $(AUG_LDFLAGS) -l fragments a4x.o
