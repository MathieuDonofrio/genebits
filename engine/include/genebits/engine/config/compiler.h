#ifndef GENEBITS_ENGINE_CONFIG_COMPILER_H
#define GENEBITS_ENGINE_CONFIG_COMPILER_H

#include <cstdint>

// Platform

#if defined(__linux__)
#define PLATFORM_LINUX 1
#define PLATFORM_WINDOWS 0
#elif defined(_WIN32) || defined(_WIN64)
#define PLATFORM_LINUX 0
#define PLATFORM_WINDOWS 1
#else
#error Platform not supported
#endif

// Compiler

#if defined(__clang__)
#define COMPILER_CLANG 1
#define COMPILER_GCC 0
#define COMPILER_MSVC 0
#elif defined(__GNUC__) || defined(__GNUG__)
#define COMPILER_CLANG 0
#define COMPILER_GCC 1
#define COMPILER_MSVC 0
#elif defined(_MSC_VER)
#define COMPILER_CLANG 0
#define COMPILER_GCC 0
#define COMPILER_MSVC 1
#else
#error Compiler not supported
#endif

// Environment

#if INTPTR_MAX == INT64_MAX
#define ENV_64
#elif INTPTR_MAX == INT32_MAX
#define ENV_32
#else
#error Could not detect environment (32/64 bit) using macros.
#endif

#if defined(ENV_64)
static_assert(sizeof(void*) == 8, "In 64 bit environment, size of pointer should be 8 bytes");
#elif defined(ENV_32)
static_assert(sizeof(void*) == 4, "In 32 bit environment, size of pointer should be 4 bytes");
#endif

#ifndef ENV_64
#error "Environment must be 64 bit"
#endif

// SIMD

#ifndef __AVX2__
#error AVX2 is required.
#endif

// C++ Standard

#if defined(__cplusplus)
#if COMPILER_MSVC
#define CPP_STANDARD_VERSION _MSVC_LANG
#else
#define CPP_STANDARD_VERSION __cplusplus
#endif
#else
#error Non standard C++ not supported
#endif

#if CPP_STANDARD_VERSION >= 202002L
#define CPP_STANDARD_20 1
#else
#error Pre C++20 not supported
#endif

// C++ Feature test

#ifndef __cpp_lib_format
#error C++ formatting must be supported
#endif

#ifndef __cpp_lib_source_location
#error C++ source location must be supported
#endif

#endif