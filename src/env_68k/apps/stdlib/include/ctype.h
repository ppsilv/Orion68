#ifndef DDRAIG_OS_CTYPE_H
#define DDRAIG_OS_CTYPE_H

#include <klibc/extern.h>
#include <klibc/inline.h>


__extern_inline char isupper(char __c)
{
	return __c >= 'A' && __c <= 'Z';
}

__extern_inline char islower(char __c)
{
	return __c >= 'a' && __c <= 'z';
}

__extern_inline char isalpha(char __c)
{
	return islower(__c) || isupper(__c);
}

__extern_inline char isdigit(char __c)
{
	return ((unsigned)__c - '0') <= 9;
}

__extern_inline char isalnum(char __c)
{
	return isalpha(__c) || isdigit(__c);
}

__extern_inline char isascii(char __c)
{
	return !(__c & ~0x7f);
}

__extern_inline char isblank(char __c)
{
	return (__c == '\t') || (__c == ' ');
}

__extern_inline char iscntrl(char __c)
{
	return __c < 0x20;
}

__extern_inline char isspace(char __c)
{
	return __c == ' ' || __c == '\n' || __c == '\t' || __c == '\r';
}

__extern_inline char isxdigit(char __c)
{
	return isdigit(__c) || (__c >= 'a' && __c <= 'f') || (__c >= 'A' && __c <= 'F');
}

__extern_inline char toupper(char __c)
{
	return islower(__c) ? (__c & ~32) : __c;
}

__extern_inline char tolower(char __c)
{
	return isupper(__c) ? (__c | 32) : __c;
}


#endif

