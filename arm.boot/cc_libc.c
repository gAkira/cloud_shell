// File with some useful functions (like the ones in the libc)
#include "cc_libc.h"

void  *cc_memset(void *b, int c, size_t len)
{
  size_t  i;
  unsigned char u_c;

  i = 0;
  u_c = (unsigned char)c;
  while (i < len)
  {
    *(unsigned char*)(b + i) = u_c;
    i++;
  }
  return (b);
}


void  cc_bzero(void *s, size_t n)
{
  cc_memset(s, 0, n);
}


void  *cc_memcpy(void *dst, const void *src, size_t n)
{
  size_t  i;

  i = 0;
  while (dst != src && i < n)
  {
    *(unsigned char*)(dst + i) = *(unsigned char*)(src + i);
    i++;
  }
  return (dst);
}


void  *cc_memmove(void *dst, const void *src, size_t len)
{
  size_t  i;

  if (len && (unsigned long)dst > (unsigned long)src)
  {
    i = 1;
    while (src != dst && i <= len)
    {
      *(unsigned char*)(dst + len - i) = *(unsigned char*)(src + len - i);
      i++;
    }
    return (dst);
  }
  return (cc_memcpy(dst, src, len));
}


size_t  cc_strlen(const char *s)
{
  size_t  len;

  len = 0;
  while (s[len] != '\0')
    len++;
  return (len);
}


int cc_strncmp(const char *s1, const char *s2, size_t n)
{
  int diff;
  size_t  i;

  i = 0;
  diff = 0;
  while (i < n && !diff && s1[i] != '\0' && s2[i] != '\0')
  {
    diff = (int)((unsigned char)s1[i] - (unsigned char)s2[i]);
    i++;
  }
  if (i < n && !diff && (s1[i] == '\0' || s2[i] == '\0'))
    diff = (int)((unsigned char)s1[i] - (unsigned char)s2[i]);
  return (diff);
}
