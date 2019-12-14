.POSIX:

TARGETS = a4xa3072

.PHONY: all clean

CFLAGS = -O -s

all: $(TARGETS)

clean:
	-rm $(TARGETS)
