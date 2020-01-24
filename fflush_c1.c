#include <r4g/stdio_tsq4kr2x9qnr3sh1jgkaaf8qg.h>
#include <stdio.h>

extern void fflush_c1(FILE *stream) {
   if (fflush(stream)) raise_write_error_c1();
}
