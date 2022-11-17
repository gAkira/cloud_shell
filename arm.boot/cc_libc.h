#ifndef CC_LIBC_H
#define CC_LIBC_H

#include <stddef.h>

void  *cc_memset(void *b, int c, size_t len);
void  cc_bzero(void *s, size_t n);
void  *cc_memcpy(void *dst, const void *src, size_t n);
void  *cc_memmove(void *dst, const void *src, size_t len);
size_t  cc_strlen(const char *s);
int cc_strncmp(const char *s1, const char *s2, size_t n);

#endif
