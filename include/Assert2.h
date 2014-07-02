#pragma once

#include <stdio.h>

#ifdef _MSC_VER
#define DEBUG_BREAK __debugbreak()
#define FUNCTION_SIGNATURE __FUNCSIG__
#define MSC_PRAGMA_PUSH __pragma(warning(push))
#define MSC_PRAGMA_POP __pragma(warning(pop))
#define MSC_PRAGMA_WARNING_DISABLE(text) __pragma(warning(text))
#else
#include <signal.h>
#define DEBUG_BREAK raise(SIGTRAP)
#define FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#define MSC_PRAGMA_PUSH
#define MSC_PRAGMA_POP
#define MSC_PRAGMA_WARNING_DISABLE(text)
#endif


#ifndef NDEBUG
#define assert2(condition, ...) \
do \
{ \
MSC_PRAGMA_PUSH \
MSC_PRAGMA_WARNING_DISABLE(disable:4127) \
	if (!(condition)) \
	{ \
		printf("Assertion failed: %s\n", #condition); \
		printf(__VA_ARGS__); \
		printf("\nFile: %s:%i\nFunction: %s\n", __FILE__, __LINE__, FUNCTION_SIGNATURE); \
		DEBUG_BREAK; \
	} \
} while (false) \
MSC_PRAGMA_POP

#else
//The void casting prevents warnings about unused variables.
#define assert2(condition, ...) ((void)(condition))
#endif
