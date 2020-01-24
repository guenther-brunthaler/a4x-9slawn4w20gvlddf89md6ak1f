#include <r4g/stdio_tsq4kr2x9qnr3sh1jgkaaf8qg.h>
#include <stdio.h>

extern void putc_c1(int c, FILE *stream) {
   if (fputc(c, stream) != c) raise_write_error_c1();
}
