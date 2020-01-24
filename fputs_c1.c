#include <r4g/stdio_tsq4kr2x9qnr3sh1jgkaaf8qg.h>
#include <stdio.h>

extern void fputs_c1(char const *s, FILE *stream) {
   if (fputs(s, stream) < 0) raise_write_error_c1();
}
