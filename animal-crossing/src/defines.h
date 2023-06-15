#pragma once

#include <stdint.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#define global static
#define internal static
#define persist static

// Unsigned integers numbers
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// Signed integers numbers
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

// Floating point numbers
typedef float f32;
typedef double f64;

// Boolean types
typedef u8 b8;
typedef u32 b32;
