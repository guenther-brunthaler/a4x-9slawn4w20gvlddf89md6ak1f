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

int main(int argc, char **argv) {
   r4g *rc= r4g_c1();
   int optind= 0;
   {
      struct minimal_resource r;
      r.dtor= &error_reporter_dtor;
      (void)fprintf(stderr, "DTOR = %p\n", r.dtor);
      r.saved= rc->rlist;
      (void)fprintf(stderr, "SAVED = %p\n", r.saved);
      rc->rlist= &r.dtor;
   }
   {
      int opt, optpos= 0;
      while (opt= getopt_simplest(&optind, &optpos, argc, argv)) {
         switch (opt) {
            case 'e':
            case 'd':
            default: error_c1(rc, "Unrecognized command line option!");
         }
      }
   }
   if (fflush(0)) error_c1(rc, "Write error!");
   release_c1(rc);
   return EXIT_SUCCESS;
}
