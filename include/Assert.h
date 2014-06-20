#pragma once

#include <stdio.h>

#ifdef _MSC_VER
#define DEBUG_BREAK __debugbreak()
#define FUNCTION_SIGNATURE __FUNCSIG__
#else
#include <signal.h>
#define DEBUG_BREAK raise(SIGTRAP)
#define FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#endif


#ifndef NDEBUG
#define assert2(condition, ...) \
do \
{ \
	if (!(condition)) \
	{ \
		printf("Assertion failed: %s\n", #condition); \
		printf(__VA_ARGS__); \
		printf("\nFile: %s:%i\nFunction: %s\n", __FILE__, __LINE__, FUNCTION_SIGNATURE); \
		DEBUG_BREAK; \
	} \
} while (false)

#else
//The void casting prevents warnings about unused variables.
#define assert2(condition, ...) ((void)(condition))
#endif
