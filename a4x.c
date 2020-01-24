char const SHORT_INFO[]=
"extended ARCFOUR application\n"
"\n"
"Provides several utility functions such as encryption or hashing,\n"
"all based on the original ARCFOUR core algorithm, augmented with\n"
"additional mechanisms based on my own ideas.\n"
;
char const VERSION_INFO[]=
"Version 2020.24.1\n"
"Copyright (c) 2019-2020 Guenther Brunthaler. All rights reserved.\n"
"\n"
"This source file is free software.\n"
"Distribution is permitted under the terms of the GPLv3.\n"
;

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <r4g/r4g_u0ywydbuiziuzssqsi5l0mdid.h>
#include <r4g/stdio_tsq4kr2x9qnr3sh1jgkaaf8qg.h>
#include <dim_sdbrke8ae851uitgzm4nv3ea2.h>
#include <getopt_nh7lll77vb62ycgwzwf30zlln.h>
#include "xl8.h"

static void error_reporter_dtor(void) {
   R4G_DEFINE_INIT_RPTR(struct minimal_resource, *r=, dtor);
   r4g.rlist= r->saved;
   if (!r4g.errors) return;
   (void)fprintf(
      stderr, "%s\n", r4g.static_error_message
         ?  r4g.static_error_message
         :  T("Internal error!")
   );
   if (r4g.errors != 1) {
      (void)fprintf(
            stderr
         ,  r4g.errors < 0
            ? T("(%d or more follow-up errors also occurred.)\n")
            : T("(%d follow-up errors also occurred.)\n")
         ,  r4g.errors < 0 ? -r4g.errors : r4g.errors
      );
   }
}

typedef struct {
   unsigned char sbox[1 << 8];
   unsigned i, j;
} a4;

static void a4_init(a4 *ac) {
   unsigned i;
   unsigned char (*s)[DIM(ac->sbox)]= &ac->sbox;
   for (i= DIM(*s); i--; ) (*s)[i]= (unsigned char)i;
   ac->i= ac->j= 0;
}

static void a4_feed(a4 *ac, char const *key_segment, size_t bytes) {
   unsigned i= ac->i, j= ac->j;
   size_t k;
   unsigned char (*s)[DIM(ac->sbox)]= &ac->sbox;
   for (k= 0; k < bytes; ++k) {
      j= j + (*s)[i] + (unsigned char)key_segment[k] & DIM(*s) - 1;
      if (++i == DIM(*s)) i= 0;
      {
         unsigned char t;
         t= (*s)[i]; (*s)[i]= (*s)[j]; (*s)[j]= t;
      }
   }
   ac->i= i; ac->j= j;
}

static void a4_warmup(a4 *ac) {
   unsigned i, j, k= 4 * 3 * DIM(ac->sbox);
   unsigned char (*s)[DIM(ac->sbox)]= &ac->sbox;
   i= j= 0;
   while (k--) {
      if (++i == DIM(*s)) i= 0;
      j= j + (*s)[i] & DIM(*s) - 1;
      {
         unsigned char t;
         t= (*s)[i]; (*s)[i]= (*s)[j]; (*s)[j]= t;
      }
   }
   ac->i= i; ac->j= j;
}

static void a4_fill(a4 *ac, void *data, size_t bytes) {
   unsigned char *d= data;
   unsigned i= ac->i, j= ac->j;
   size_t k;
   unsigned char (*s)[DIM(ac->sbox)]= &ac->sbox;
   for (k= 0; k < bytes; ++k) {
      if (++i == DIM(*s)) i= 0;
      j= j + (*s)[i] & DIM(*s) - 1;
      {
         unsigned char t;
         t= (*s)[i]; (*s)[i]= (*s)[j]; (*s)[j]= t;
      }
      d[k]= (*s)[((*s)[i] + (*s)[j]) & DIM(*s) - 1];
   }
   ac->i= i; ac->j= j;
}

static void a4_encrypt_xa(a4 *ac, void *data, size_t bytes) {
   unsigned char *d= data;
   unsigned i= ac->i, j= ac->j;
   size_t k;
   unsigned char (*s)[DIM(ac->sbox)]= &ac->sbox;
   for (k= 0; k < bytes; ++k) {
      if (++i == DIM(*s)) i= 0;
      j= j + (*s)[i] & DIM(*s) - 1;
      {
         unsigned char t;
         t= (*s)[i]; (*s)[i]= (*s)[j]; (*s)[j]= t;
      }
      d[k]^= (*s)[((*s)[i] + (*s)[j]) & DIM(*s) - 1];
      if (++i == DIM(*s)) i= 0;
      j= j + (*s)[i] & DIM(*s) - 1;
      {
         unsigned char t;
         t= (*s)[i]; (*s)[i]= (*s)[j]; (*s)[j]= t;
      }
      d[k]+= (*s)[((*s)[i] + (*s)[j]) & DIM(*s) - 1];
   }
   ac->i= i; ac->j= j;
}

static void a4_decrypt_xa(a4 *ac, void *data, size_t bytes) {
   unsigned char *d= data;
   unsigned i= ac->i, j= ac->j;
   size_t k;
   unsigned char (*s)[DIM(ac->sbox)]= &ac->sbox;
   for (k= 0; k < bytes; ++k) {
      if (++i == DIM(*s)) i= 0;
      j= j + (*s)[i] & DIM(*s) - 1;
      {
         unsigned char t;
         t= (*s)[i]; (*s)[i]= (*s)[j]; (*s)[j]= t;
      }
      d[k]-= (*s)[((*s)[i] + (*s)[j]) & DIM(*s) - 1];
      if (++i == DIM(*s)) i= 0;
      j= j + (*s)[i] & DIM(*s) - 1;
      {
         unsigned char t;
         t= (*s)[i]; (*s)[i]= (*s)[j]; (*s)[j]= t;
      }
      d[k]^= (*s)[((*s)[i] + (*s)[j]) & DIM(*s) - 1];
   }
   ac->i= i; ac->j= j;
}

int main(int argc, char **argv) {
   struct minimal_resource error_reporter;
   int decrypt= -1;
   error_reporter.dtor= &error_reporter_dtor;
   error_reporter.saved= r4g.rlist;
   r4g.rlist= &error_reporter.dtor;
   {
      int optind= 0;
      {
         int opt, optpos= 0;
         while (opt= getopt_simplest(&optind, &optpos, argc, argv)) {
            switch (opt) {
               case 'e': decrypt= 0; break;
               case 'd': decrypt= 1; break;
               case 'h':
                  fputs_c1(argc ? argv[0] : T("(unnamed program)"), stdout);
                  fputs_c1(" - ", stdout); puts_c1(SHORT_INFO);
                  /* Fall through. */
               case 'V': fputs_c1(VERSION_INFO, stdout); goto done;
               default: error_c1(T("Unrecognized command line option!"));
            }
         }
      }
      if (optind != argc - 1) {
         error_c1(
            T( \
               "Please specify the path to the binary key file" \
               " as the only non-option argument!" \
            )
         );
      }
   }
   if (decrypt < 0) {
      error_c1(T("Please select encryption with -e or decryption with -d!"));
   }
   done:
   fflush_c1(0);
   release_c1();
   return EXIT_SUCCESS;
}
