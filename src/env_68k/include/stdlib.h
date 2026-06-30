#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include <stddef.h>
#include <errno.h>

#define NULL 0

extern char * itoa (int value, char *str, int base); __attribute__((nonnull (1,2)));
extern char * __itoa (int value, char *str, int base); __attribute__((nonnull (1,2)));
extern char *  utoa (unsigned value, char *str,  int base) __attribute__((nonnull (1,2)));
extern char * __utoa (unsigned value, char *str,  int base) __attribute__((nonnull (1,2)));


extern double atof (const char *__nptr) __attribute__((nonnull (1)));
extern int atoi (const char *__nptr)    __attribute__((nonnull (1)));
extern long int atol (const char *__nptr) __attribute__((nonnull (1)));
extern long long int atoll (const char *__nptr) __attribute__((nonnull (1)));
extern double strtod (const char *__restrict __nptr, char **__restrict __endptr) __attribute__((nonnull (1)));
extern float strtof (const char *__restrict __nptr, char **__restrict __endptr) __attribute__((nonnull (1)));

extern long double strtold (const char *__restrict __nptr, char **__restrict __endptr); __attribute__((nonnull (1)));
extern long int strtol (const char *__restrict __nptr,char **__restrict __endptr, int __base) __attribute__((nonnull (1)));
extern unsigned long int strtoul (const char *__restrict __nptr, char **__restrict __endptr, int __base) __attribute__((nonnull (1)));
extern long long int strtoll (const char *__restrict __nptr, char **__restrict __endptr, int __base) __attribute__((nonnull (1)));
extern unsigned long long int strtoull (const char *__restrict __nptr, char **__restrict __endptr, int __base); __attribute__((nonnull (1)));

extern uintmax_t strntoumax(const char *nptr, char **endptr, int base, unsigned int n) __attribute__((nonnull (1)));

/* Allocate SIZE bytes of memory.  */
extern void *malloc (size_t __size)__attribute__ ((alloc_size (1)));
extern void *calloc (size_t __nmemb, size_t __size) __attribute__ ((alloc_size (1,2)));
extern void *realloc (void *__ptr, size_t __size) __attribute__ ((alloc_size (2)));
extern void free (void *__ptr);

extern int atoi (const char *__nptr) __attribute__((nonnull (1)));
extern long int atol (const char *__nptr) __attribute__((nonnull (1)));
extern long long int atoll (const char *__nptr) __attribute__((nonnull (1)));

extern char isspace(char c);
extern char isdigit(char c);
extern char isalpha(char c);
extern char isupper(char c);
extern char islower(char c);


extern void exit(int status);
extern int atexit (void (*__func) (void)) __attribute__((nonnull (1)));








#ifdef __DDRAIG_DOS
#ifndef NULL
#define NULL ((void*) 0)
#endif

#include <klibc/extern.h>
#include <klibc/inline.h>
#include <stddef.h>

__extern_inline int abs(int __n)
{
	return (__n < 0) ? -__n : __n;
}

__extern int atoi(const char *);
__extern long atol(const char *);
__extern long long atoll(const char *);

__extern_inline long labs(long __n)
{
	return (__n < 0L) ? -__n : __n;
}
__extern_inline long long llabs(long long __n)
{
	return (__n < 0LL) ? -__n : __n;
}

__extern long strtol(const char *, char **, int);
__extern long long strtoll(const char *, char **, int);
__extern unsigned long strtoul(const char *, char **, int);
__extern unsigned long long strtoull(const char *, char **, int);

typedef int (*__comparefunc_t) (const void *, const void *);
__extern void *bsearch(const void *, const void *, size_t, size_t, __comparefunc_t);
__extern void qsort(void *, size_t, size_t, __comparefunc_t);

__extern long jrand48(unsigned short  xsubi[3]);
__extern long mrand48(void);
__extern long nrand48(unsigned short  xsubi[3]);
__extern long lrand48(void);
__extern unsigned short *seed48(const unsigned short *);
__extern void srand48(long);

#define RAND_MAX 0x7fffffff
__extern_inline int rand(void)
{
	return (int)lrand48();
}
__extern_inline void srand(unsigned int __s)
{
	srand48(__s);
}
__extern_inline long random(void)
{
	return lrand48();
}
__extern_inline void srandom(unsigned int __s)
{
	srand48(__s);
}

#endif

#endif
