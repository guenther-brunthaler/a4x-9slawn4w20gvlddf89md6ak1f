/* #include <r4g/stdio_tsq4kr2x9qnr3sh1jgkaaf8qg.h>
 *
 * R4G aware ANSI C library stdio support.
 *
 * Version 2020.20
 * Copyright (c) 2013-2020 Guenther Brunthaler. All rights reserved.
 *
 * This source file is free software.
 * Distribution is permitted under the terms of the LGPLv3. */

#ifndef HEADER_TSQ4KR2X9QNR3SH1JGKAAF8QG_INCLUDED
   #define HEADER_TSQ4KR2X9QNR3SH1JGKAAF8QG_INCLUDED

   #include <stdio.h>
   #include <stdarg.h>

   #ifdef __cplusplus
      extern "C" {
   #endif

   void fflush_c1(FILE *stream);
   void fprintf_c1(FILE *stream, char const *format, ...);
   void fputs_c1(char const *s, FILE *stream);
   void printf_c1(char const *format, ...);
   void putc_c1(int c, FILE *stream);
   void putchar_c1(int c);
   void puts_c1(char const *s);
   void raise_read_error_c1(void);
   void raise_write_error_c1(void);
   void vfprintf_c1(FILE *stream, char const *format, va_list args);
   void vprintf_c1(char const *format, va_list args);

   #ifdef __cplusplus
      }
   #endif
#endif /* !HEADER_TSQ4KR2X9QNR3SH1JGKAAF8QG_INCLUDED */
