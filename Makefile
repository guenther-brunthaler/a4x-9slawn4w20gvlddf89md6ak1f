# The following line declares this Makefile as strictly POSIX-compliant.
.POSIX:

# Default build options. Override these by either passing the C compiler
# options via "CFLAGS=" directly as an argument on the "make" command line, or
# by "export MAKEFLAGS=e" first and then setting "export CFLAGS=". The same
# applies to CPPFLAGS (passed to the C preprocessor) and LDFLAGS (passed to
# the linker).
CPPFLAGS = -D NDEBUG
CFLAGS = -O
LDFLAGS = -s

# Additional flags added internally by the Makefile.
PRIVATE_LDFLAGS = -L .
PRIVATE_CPPFLAGS = \
	-I . \
	-I include \
	-D 'R4G_CONFIG_H_7MPQXSRZEGRNW8FJHR4CYANWJ="local_config.h"'

TARGETS = a4x $(a4x_OBJECTS) libfragments.a $(FRAGMENT_OBJECTS)

a4x_OBJECTS = a4x.o

FRAGMENT_OBJECTS = \
	fflush_c1.o \
	fputs_c1.o \
	getopt_simplest.o \
	putc_c1.o \
	putchar_c1.o \
	puts_c1.o \
	r4g.o \
	raise_write_error_c1.o \

# Augmented flags which are actually used for compiling.
AUG_CFLAGS= $(CPPFLAGS) $(PRIVATE_CPPFLAGS) $(CFLAGS)
AUG_LDFLAGS= $(LDFLAGS) $(PRIVATE_LDFLAGS)

.PHONY: all clean

all: $(TARGETS)

clean:
	-rm $(TARGETS)

libfragments.a: $(FRAGMENT_OBJECTS)
	$(AR) $(ARFLAGS) $@ $(FRAGMENT_OBJECTS)

.c.o:
	$(CC) -o $@ $(AUG_CFLAGS) -c $<

a4x: a4x.o libfragments.a
	$(CC) -o $@ $(AUG_LDFLAGS) a4x.o libfragments.a

# Dependencies detected with "gcc -MM", one per line, sorted by both target
# and dependency, using the collation sequence of locale C.
a4x.o: \
 a4x.c \
 include/dim_sdbrke8ae851uitgzm4nv3ea2.h \
 include/getopt_nh7lll77vb62ycgwzwf30zlln.h \
 include/r4g/r4g_u0ywydbuiziuzssqsi5l0mdid.h \
 include/r4g/stdio_tsq4kr2x9qnr3sh1jgkaaf8qg.h \
 local_config.h \
 xl8.h

fflush_c1.o: \
 fflush_c1.c \
 include/r4g/stdio_tsq4kr2x9qnr3sh1jgkaaf8qg.h

fputs_c1.o: \
 fputs_c1.c \
 include/r4g/stdio_tsq4kr2x9qnr3sh1jgkaaf8qg.h

getopt_simplest.o: \
 getopt_simplest.c \
 include/getopt_nh7lll77vb62ycgwzwf30zlln.h

putc_c1.o: \
 include/r4g/stdio_tsq4kr2x9qnr3sh1jgkaaf8qg.h \
 putc_c1.c

putchar_c1.o: \
 include/r4g/stdio_tsq4kr2x9qnr3sh1jgkaaf8qg.h \
 putchar_c1.c

puts_c1.o: \
 include/r4g/stdio_tsq4kr2x9qnr3sh1jgkaaf8qg.h \
 puts_c1.c

r4g.o: \
 include/r4g/r4g_u0ywydbuiziuzssqsi5l0mdid.h \
 local_config.h \
 r4g.c

raise_write_error_c1.o: \
 include/r4g/r4g_u0ywydbuiziuzssqsi5l0mdid.h \
 local_config.h \
 raise_write_error_c1.c \
 xl8.h
