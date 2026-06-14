#ifndef STDLIB_H
#define STDLIB_H

#include <stdint.h>
#include <stddef.h>
#include <errno.h>

#define NULL 0

void *malloc(unsigned int size);
void free(void *ptr);
void *calloc(unsigned int num, unsigned int size);
void *realloc(void *ptr, unsigned int size);

int atoi(const char *str);
void exit(int status);

long atol(const char *str);
long long atoll (const char *str);

char * __itoa (int value, char *str, int base);
char * itoa (int value, char *str, int base);


char *  utoa (unsigned value, char *str,  int base);
char * __utoa (unsigned value, char *str,  int base);

long strtol(const char *nptr, char **endptr, int base);
long long strtoll(const char *str, char **endptr, int base);
uintmax_t strntoumax(const unsigned char *nptr, char **endptr, int base, unsigned int n);

char isspace(char c);
char isdigit(char c);
char isalpha(char c);
char isupper(char c);
char islower(char c);

int strcmp(const char *s1, const char *s2);
void *memcpy(void *dest, const void *src, int n);
void memset(void *s, int c, int n);
int memcmp(const char *s1, const char *s2, int n);
char *strcat(char *dest, const char *src);
const char *strchr(const char *str, int ch);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, int n);
char *strcpy(char *dest, const char *src);
size_t strcspn(const char *str1, const char *str2);
int strlen(const char *str);
char *strncat(char *dest, const char *src, unsigned long max);
int strncmp(const char *str1, const char *str2, int max);
char *strncpy(char *dest, const char *src, int n);
size_t strnlen(const char *str, size_t maxlen);
const char *strrchr(const char *str, int ch);
size_t strspn(const char *str1, const char *str2);
char *strstr(const char *str1, const char *str2);


#endif
