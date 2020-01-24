/* #include <r4g/cbuffer_p1yk4fgjdiljnafrva9hz5800.h>
 *
 * Version 2020.24
 * Copyright (c) 2020 Guenther Brunthaler. All rights reserved.
 *
 * This header file and its associated implementations are free software.
 * Distribution is permitted under the terms of the LGPLv3. */

#ifndef HEADER_P1YK4FGJDILJNAFRVA9HZ5800_INCLUDED
#define HEADER_P1YK4FGJDILJNAFRVA9HZ5800_INCLUDED

#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
   extern "C" {
#endif

/* Descriptor for a malloc()-based dynamically allocated 'char' buffer that
 * can grow or shrink. A buffer will often contain text, but can also contain
 * arbitrary binary bytes. If it does contain text, the first unused reserved
 * byte will normally be set to a zero value, making the buffer a
 * null-terminated "C" string. Note that this requires the allocation size to
 * be at least one byte larger than the string length. */
typedef struct {
   char *start; /* First byte of data or null if not allocated. */
   size_t length; /* Buffer bytes currently in use, up to <reserved>. */
   size_t reserved; /* Current allocation size or 0 if not allocated. */
} cbuffer;

/* <cbuffer> object wrapped into a resource object. Use this as a local
 * variable to ensure the buffer will be deallocated in case of an error. */
typedef struct {
   cbuffer b; /* The wrapped buffer object. */
   void (*dtor)(void); /* Cleanup function for this resource. */
   void (**saved)(void); /* Saved <r4g.rlist> pointer of previous resource. */
} cbrsc;

/* Initializes <b> to be a zero-sized buffer with no reserved bytes which has
 * not been allocated allocated yet as a consequence. */
void cbuffer_init_c0(cbuffer *b);

/* Initializes r->b and link <b> into the r4g.rlist resource list. */
void cbrsc_init_c4(cbrsc *r);

/* Set the first unused buffer byte to a zero value and return a pointer to
 * the resulting buffer as a null-terminated string. Will grow the buffer if
 * no unused reserved bytes are available. */
char *cstr_c1(cbuffer *s);

/* Reallocates the buffer to <required> bytes, rounded up to some
 * internally-defined allocation quantum which might also be size-dependent.
 * Does nothing if the rounded-up size is already the same as <reserved>. If
 * <required> is zero, the buffer will be deallocated instead. If the buffer
 * was not allocated before, it will be allocated rather than being
 * reallocated. This call may change <reserved> but not <length>. <required>
 * must not be smaller than <length>. Note that this call may also shrink the
 * buffer, so don't call it if you only want to grow the buffer. */
void cbuffer_resize_c1(cbuffer *b, size_t required);

/* Reallocates the buffer so that <reserved> becomes exactly the same
 * as <length> without the usual internal rounding-up of <reserved>. If
 * <length> is 0, the buffer will become deallocated. */
void cbuffer_pack_c1(cbuffer *b);

/* Reallocates the buffer so that <reserved> becomes exactly one more than
 * <length> and set the unused byte to a zero value, making the whole buffer a
 * null-terminated string with no further reserved bytes after it. */
char *cstrfinal_c1(cbuffer *s);

/* Append the binary contents of a buffer to another buffer. */
void cbuffer_append_c1(cbuffer *dst, cbuffer const *src);

/* Append a string to a string buffer. Does not copy the null terminator. */
void cstrcat_c1(cbuffer *dst, char const *src);

/* Expand a simple format string containing string insertion sequences and
 * append it to the current contents of a dynamically allocated string.
 *
 * The variable arguments consist of one or more (int key, const char
 * *expansion)-pairs.
 *
 * Insertion sequences come first in arbitrary order and define a "char" key
 * and an expansion string associated with that key. It the same key occurs
 * multiple times, only the last such argument-pair will be found when
 * referenced from within the format string. Argument-pairs which are never
 * referenced from within the format string are allowed and will have no
 * effect.
 *
 * A key of 0 is special and its associated expansion string then actually
 * represents the format string. The format string is usually also the last
 * argument, because any additional arguments will be ignored.
 *
 * Within the format string, all characters are copied literally to the output
 * buffer except sequences of the form "%" followed by some character. This
 * represents an insertion sequence, and the character following the "%" will
 * be used as a lookup key for finding the expansion string to insert.
 *
 * All possible "char" values (including '%') are valid as keys, not just
 * printable characters. However, a NUL value will not work as key, because
 * then the argument-pair defining its expansion value would be
 * mis-interpreted as the format string.
 *
 * There is no built-in way to output a literal "%". If required it is
 * recommended to include an argument-pair '%', "%" which will copy "%" into
 * the result buffer whenever "%%" occurs within the format string.
 *
 * Argument-pairs referenced by "%"-sequences will be located using a linear
 * search backwards through the pairs starting immediately before the format
 * string. Even though the referenced pairs will be found in any case, for
 * maximum performance more frequently referenced pairs should be placed after
 * less frequently referenced pairs. If all pairs are only referenced exactly
 * once, the order does not affect performance.
 *
 * The resulting augmented string will not have been null-terminated yet. */
void cstrf_c1(cbuffer *append, ...);

/* Same a cstrf_c1() but takes the variable arguments as a va_list instead of
 * directly. */
void vcstrf_c1(cbuffer *append, va_list args);

#ifdef __cplusplus
   }
#endif
#endif /* !HEADER_P1YK4FGJDILJNAFRVA9HZ5800_INCLUDED */
