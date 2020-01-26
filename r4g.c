/* Version 2020.26
 * Copyright (c) 2013-2020 Guenther Brunthaler. All rights reserved.
 *
 * This source file is free software.
 * Distribution is permitted under the terms of the LGPLv3. */

#include <r4g/r4g_u0ywydbuiziuzssqsi5l0mdid.h>
#include <assert.h>
#include <stdlib.h>

#ifdef PER_THREAD_QUALIFIER_0VC35WCXXRR0DWZDPTP8CUC8S
   /* Fall-back multi-threading support is *not* being used. Just define the
    * global <r4g> variable. Note that the macro
    * PER_THREAD_QUALIFIER_0VC35WCXXRR0DWZDPTP8CUC8S may expand to nothing, in
    * which case a simple global variable will be defined. */
   PER_THREAD_QUALIFIER_0VC35WCXXRR0DWZDPTP8CUC8S
      struct resource_context_4th_generation r4g
   ;
#endif

extern void release_c1(void) {
   while (r4g.rlist) (*r4g.rlist)();
}

extern void error_c1(char const *static_message) {
   if (r4g.errors) {
      if (r4g.errors > 0 && ++r4g.errors >= 250) r4g.errors= -r4g.errors;
   } else {
      if (!r4g.static_error_message) r4g.static_error_message= static_message;
      r4g.errors= 1;
   }
   release_c1();
   assert(!r4g.rlist);
   exit(EXIT_FAILURE);
}
