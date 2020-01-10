.POSIX:

TARGETS = a4xa

.PHONY: all clean

CFLAGS = -D NDEBUG -O -s

all: $(TARGETS)

clean:
	-rm $(TARGETS)
