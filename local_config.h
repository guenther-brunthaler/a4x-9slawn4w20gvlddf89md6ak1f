/* Enable project-specific R4G features.
 *
 * Add -D 'R4G_CONFIG_H_7MPQXSRZEGRNW8FJHR4CYANWJ="local_config.h"' to
 * the C preprocessor flags to enable usage of this file.
 *
 * Version 2020.26.1 */

#if defined __STDC_VERSION__ && __STDC_VERSION__ >= 201000L \
&& !defined __STDC_NO_THREADS__
   /* Prefer C11/C18+ threads. */
   #define ENABLE_OPTIONAL_CTHREADS_LMGBEG53DETVLBEP2J6J4BUKY
#elif defined __GNUC__ \
&& (__GNUC__ > 6 || __GNUC__ == 6 && __GNUC_MINOR__ >= 3)
   /* Otherwise, if using gcc and some older version of it, use GCC's
    * implementation-specific thread support for older gcc/libc versions. I
    * have no idea since when GCC supports this exactly, but I know that at
    * least gcc 6.3 already does. */
   #define PER_THREAD_QUALIFIER_0VC35WCXXRR0DWZDPTP8CUC8S __thread
#else
   /* Otherwise, use fallback multi-threading support. This will require
    * pthreads support in the case of this local project. */
   #define ENABLE_THREADS_8Y802YFBJ3A8H763I3XID022D
#endif
