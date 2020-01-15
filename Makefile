.POSIX:

TARGETS = a4x

.PHONY: all clean

CFLAGS = -D NDEBUG -O -s

all: $(TARGETS)

clean:
	-rm $(TARGETS)
