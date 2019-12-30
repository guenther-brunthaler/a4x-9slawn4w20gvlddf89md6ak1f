/*
 * Version 2019.348
 * Copyright (c) 2019 Guenther Brunthaler. All rights reserved.
 *
 * This source file is free software.
 * Distribution is permitted under the terms of the GPLv3.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

#define DIM(array) (sizeof(array) / sizeof *(array))

typedef struct resource_context_4th_generation r4g;
typedef void (*r4g_dtor)(r4g *rc);
struct resource_context_4th_generation {
   int errors; /* Error count (negative for saturated). */
   char const *static_error_message; /* First error or null. */
   r4g_dtor *rlist; /* Address of pointer to dtor in last resource. */
};

struct minimal_resource {
   r4g_dtor dtor, *saved;
};

/* Example: R4G_DEFINE_INIT_RPTR(struct my_resource, *r=, rc, dtor); */
#define R4G_DEFINE_INIT_RPTR(resource_t, var_eq, r4g_rc, dtor_member) \
   resource_t var_eq (void *)( \
      (char *)(r4g_rc)->rlist - offsetof(resource_t, dtor_member) \
   )

static r4g *r4g_c1(void) {
   static r4g single_thread_context;
   return &single_thread_context;
}

static void release_c1(r4g *rc) {
   while (rc->rlist) (*rc->rlist)(rc);
}

static void error_c1(r4g *rc, char const *static_message) {
   if (rc->errors) {
      if (rc->errors > 0 && ++rc->errors >= 250) rc->errors= -rc->errors;
   } else {
      rc->static_error_message= static_message;
      rc->errors= 1;
   }
   release_c1(rc);
   exit(EXIT_FAILURE);
}

static int getopt_simplest(
   int *optind_ref, int *optpos_ref, int argc, char **argv
) {
   int c, i= *optpos_ref, optind= *optind_ref;
   if (argc <= 1) {
      assert(optind == 0);
      assert(i == 0);
      end_of_options:
      c= 0;
      goto done;
   }
   if (optind == 0) optind= 1; /* Start parsing after argv[0]. */
   for (;;) {
      assert(optind < argc);
      assert((size_t)i <= strlen(argv[optind]));
      switch (c= argv[optind][i]) {
         case 0:
            if (i == 0 || ++optind == argc) goto end_of_options;
            i= 0;
            continue;
         case '-':
            if (i == 0 && argv[optind][1] == '\0') goto end_of_options;
            if (i == 1 && argv[optind][i + 1] == '\0') {
               ++optind; goto end_of_options;
            }
            ++i;
            continue;
      }
      break;
   }
   if (i == 0) goto end_of_options;
   ++i;
   done:
   *optind_ref= optind;
   *optpos_ref= i;
   return c;
}

static void error_reporter_dtor(r4g *rc) {
   R4G_DEFINE_INIT_RPTR(struct minimal_resource, *r=, rc, dtor);
   rc->rlist= r->saved;
   if (!rc->errors) return;
   (void)fprintf(
      stderr, "%s\n", rc->static_error_message
         ?  rc->static_error_message
         :  "Internal error!"
   );
   if (rc->errors != 1) {
      (void)fprintf(
            stderr, "(%d%s follow-up errors also occurred.)\n"
         ,  rc->errors < 0 ? -rc->errors : rc->errors
         ,  rc->errors < 0 ? " or more" : ""
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
   r4g *rc= r4g_c1();
   struct minimal_resource error_reporter;
   int decrypt= -1;
   error_reporter.dtor= &error_reporter_dtor;
   error_reporter.saved= rc->rlist;
   rc->rlist= &error_reporter.dtor;
   {
      int optind= 0;
      {
         int opt, optpos= 0;
         while (opt= getopt_simplest(&optind, &optpos, argc, argv)) {
            switch (opt) {
               case 'e': decrypt= 0; break;
               case 'd': decrypt= 1; break;
               default: error_c1(rc, "Unrecognized command line option!");
            }
         }
      }
      if (optind != argc - 1) {
         error_c1(
               rc
            ,  "Please specify the path to the binary key file"
               " as the only non-option argument!"
         );
      }
   }
   if (decrypt < 0) {
      error_c1(
         rc, "Please select encryption with -e or decryption with -d!"
      );
   }
   if (fflush(0)) error_c1(rc, "Write error!");
   release_c1(rc);
   return EXIT_SUCCESS;
}
