//
// Created by scott on 9/8/2023.
//
#ifndef SHMOCODEBASE_BASE_H
#define SHMOCODEBASE_BASE_H

// ========================================
// Context Cracking

#if defined(_MSC_VER) && !defined(__clang__)
# define COMPILER_CL 1
#elif defined(__clang__)
# define COMPILER_CLANG 1
#elif defined(__GNUC__)
# define COMPILER_GCC 1
#else
# error Unkown compiler. No context cracking.
#endif

#if defined(_WIN32)
# define OS_WINDOWS 1
#elif defined(__gnu_linux__)
# define OS_LINUX 1
#elif defined(__APPLE__) && defined(__MACH__)
# define OS_MAC 1
#else
# error Unkown OS. No context cracking.
#endif

#if defined(_M_AMD64) || defined(__amd64__) || defined(__amd64) || defined(__x86_64) || defined(__x86_64__)
# define ARCH_X64 1
#elif defined(_M_IX86) || defined(__i386) || defined(__i386__)
# define ARCH_X86 1
#elif defined(__arm__) || defined(_M_ARM) || defined(__arm)
# define ARCH_ARM 1
#elif defined(__aarch64__)
# define ARCH_ARM64 1
#else
# error Unkown Architecture. No context cracking.
#endif

// Zero out the missing macros
#if !defined(COMPILER_CL)
# define COMPILER_CL 0
#endif
#if !defined(COMPILER_CLANG)
# define COMPILER_CLANG 0
#endif
#if !defined(COMPILER_GCC)
# define COMPILER_GCC 0
#endif
#if !defined(OS_WINDOWS)
# define OS_WINDOWS 0
#endif
#if !defined(OS_LINUX)
# define OS_LINUX 0
#endif
#if !defined(OS_MAC)
# define OS_MAC 0
#endif
#if !defined(ARCH_X64)
# define ARCH_X64 0
#endif
#if !defined(ARCH_X86)
# define ARCH_X86 0
#endif
#if !defined(ARCH_ARM)
# define ARCH_ARM 0
#endif
#if !defined(ARCH_ARM64)
# define ARCH_ARM64 0
#endif

// ========================================
// Basic Types

#include <stdint.h>
#include <stdbool.h>
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float    f32;
typedef double   f64;

typedef void void_func_t(void);

// ========================================
// Helper Macros

#if !defined(ENABLE_ASSERT)
# define ENABLE_ASSERT
#endif

#define stmnt(S) do { S } while(0)

#if !defined(assert_break)
# if COMPILER_GCC
#  define assert_break() (__builtin_trap())
# else
#  define assert_break() (*(int*)0 = 0)
# endif
#endif

#if ENABLE_ASSERT
# define assert(C) stmnt( if (!(C)) { assert_break(); } )
#else
# define assert(C)
#endif

#define local_fn static inline

#define STRINGIFY_(S) #S
#define STRINGIFY(S) STRINGIFY_(S)
#define GLUE_(A, B) A##B
#define GLUE(A, B) GLUE_(A, B)

#define array_count(A) (sizeof(A)/sizeof(*(A)))

#define int_from_ptr(P) (unsigned long long)((char *)P - (char *)0)
#define ptr_from_int(N) (void *)((char *)0 + (N))

#define member(T, M) (((T*)0)->M)
#define offsetof_member(T, M) int_from_ptr(&member(T, M))

#define MIN(A, B) (((A)<(B))?(A):(B))
#define MAX(A, B) (((A)>(B))?(A):(B))
#define CLAMP(A, X, B) (((X)<(A))?(A):\
                       ((B)<(X))?(B):(X))
#define CLAMP_TOP(A, B) MIN(A, B)
#define CLAMP_BOT(A, B) MAX(A, B)

#include <string.h>
#define memory_zero(Dest, Size) memset((Dest), 0, (Size))
#define memory_zero_struct(Dest) memory_zero((Dest), sizeof(*(Dest)))
#define memory_zero_array(Dest) memory_zero((Dest), sizeof(Dest))
#define memory_zero_typed(Dest, Count) memory_zero((Dest), sizeof(*(Dest)) * (Count))

#define memory_equals(A, B, Size) (memcmp((A), (B), (Size)) == 0)

#define memory_copy(Dest, Src, Size) memmove((Dest), (Src), (Size))
#define memory_copy_struct(Dest, Src) memmove((Dest), (Src), min(sizeof(*(Dest)), sizeof(*(Src))))
#define memory_copy_array(Dest, Src) memmove((Dest), (Src), min(sizeof(Dest), sizeof(Src)))
#define memory_copy_typed(Dest, Src, Count) memmove((Dest), (Src), min(sizeof(*(Dest)), sizeof(*(Src))) * (Count))

// ========================================
// Basic Constants

#define MIN_i8  ((i8)  0x80)
#define MIN_i16 ((i16) 0x8000)
#define MIN_i32 ((i32) 0x80000000)
#define MIN_i64 ((i64) 0x8000000000000000LLU)

#define MAX_i8  ((i8)  0x7F)
#define MAX_i16 ((i16) 0x7FFF)
#define MAX_i32 ((i32) 0x7FFFFFFF)
#define MAX_i64 ((i64) 0x7FFFFFFFFFFFFFFFLLU)

#define MIN_u8  ((u8)  0)
#define MIN_u16 ((u16) 0)
#define MIN_u32 ((u32) 0)
#define MIN_u64 ((u64) 0)

#define MAX_u8  ((u8)  0xFF)
#define MAX_u16 ((u16) 0xFFFF)
#define MAX_u32 ((u32) 0xFFFFFFFF)
#define MAX_u64 ((u64) 0xFFFFFFFFFFFFFFFFLLU)

#define MACHINE_EPSILON_f32 ((f32) 1.1920929e-7f)
#define E_f32               ((f32) 2.71828182845904523536f)
#define LOG2E_f32           ((f32) 1.44269504088896340736f)
#define LOG10E_f32          ((f32) 0.434294481903251827651f)
#define LN2_f32             ((f32) 0.693147180559945309417f)
#define LN10_f32            ((f32) 2.30258509299404568402f)
#define PI_f32              ((f32) 3.14159265358979323846f)
#define PI_2_f32            ((f32) 1.57079632679489661923f)
#define PI_4_f32            ((f32) 0.785398163397448309616f)
#define O1_PI_f32           ((f32) 0.318309886183790671538f)
#define O2_PI_f32           ((f32) 0.636619772367581343076f)
#define O2_SQRT_PI_f32      ((f32) 1.12837916709551257390f)
#define SQRT_2_f32          ((f32) 1.41421356237309504880f)
#define O1_SQRT_2_f32       ((f32) 0.707106781186547524401f)

#define MACHINE_EPSILON_f64 ((f64) 2.220446e-16)
#define E_f64               ((f64) 2.71828182845904523536)
#define LOG2E_f64           ((f64) 1.44269504088896340736)
#define LOG10E_f64          ((f64) 0.434294481903251827651)
#define LN2_f64             ((f64) 0.693147180559945309417)
#define LN10_f64            ((f64) 2.30258509299404568402)
#define PI_f64              ((f64) 3.14159265358979323846)
#define PI_2_f64            ((f64) 1.57079632679489661923)
#define PI_4_f64            ((f64) 0.785398163397448309616)
#define O1_PI_f64           ((f64) 0.318309886183790671538)
#define O2_PI_f64           ((f64) 0.636619772367581343076)
#define O2_SQRT_PI_f64      ((f64) 1.12837916709551257390)
#define SQRT_2_f64          ((f64) 1.41421356237309504880)
#define O1_SQRT_2_f64       ((f64) 0.707106781186547524401)

// ========================================
// Math Functions

f32 inf_f32(void);
f32 neg_inf_f32(void);
f64 inf_f64(void);
f64 neg_inf_f64(void);

f32 fabs_f32(f32 x);
f64 fabs_f64(f64 x);

f32 sqrt_f32(f32 x);
f32 sin_f32(f32 x);
f32 cos_f32(f32 x);
f32 tan_f32(f32 x);

f64 sqrt_f64(f64 x);
f64 sin_f64(f64 x);
f64 cos_f64(f64 x);
f64 tan_f64(f64 x);

f32 lerp_f32(f32 a, f32 b, f32 t);
f32 inv_lerp_f32(f32 a, f32 b, f32 x);

f64 lerp_f64(f64 a, f64 b, f64 t);
f64 inv_lerp_f64(f64 a, f64 b, f64 x);

i32 round_up_i32(i32 num, i32 multiple);
i64 round_up_i64(i64 num, i64 multiple);
u32 round_up_u32(u32 num, u32 multiple);
u64 round_up_u64(u64 num, u64 multiple);

#if !defined(DONT_USE_MATH_MACROS)
# define DONT_USE_MATH_MACROS 0
#endif

#if DONT_USE_MATH_MACROS == 0

# define fabs(X) _Generic((X), \
    f32 : fabs_f32(X),         \
    f64 : fabs_f64(X))

# define sqrt(X) _Generic((X), \
    f32 : sqrt_f32(X),         \
    f64 : sqrt_f64(X))

# define sin(X) _Generic((X), \
    f32 : sin_f32(X),         \
    f64 : sin_f64(X))

# define cos(X) _Generic((X), \
    f32 : cos_f32(X),         \
    f64 : cos_f64(X))

# define tan(X) _Generic((X), \
    f32 : tan_f32(X),         \
    f64 : tan_f64(X))

# define lerp(A, B, T) _Generic((A), \
    f32 : lerp_f32(A, B, T),         \
    f64 : lerp_f64(A, B, T))

# define inv_lerp(A, B, X) _Generic((A), \
    f32 : inv_lerp_f32(A, B, X),         \
    f64 : inv_lerp_f64(A, B, X))

#endif

// ========================================
// Compound Types

typedef union vec2i32_t {
    struct {
        i32 x;
        i32 y;
    };
    i32 v[2];
} vec2i32_t;

typedef union vec2f32_t {
    struct {
        f32 x;
        f32 y;
    };
    f32 v[2];
} vec2f32_t;

typedef union vec3f32_t {
    struct {
        f32 x;
        f32 y;
        f32 z;
    };
    f32 v[3];
} vec3f32_t;

typedef union vec4f32_t {
    struct {
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    };
    f32 v[4];
} vec4f32_t;

typedef union itv1f32_t {
    struct {
        f32 min;
        f32 max;
    };
    f32 v[2];
} itv1f32_t;

typedef union itv1u64_t {
    struct {
        u64 min;
        u64 max;
    };
    struct {
        u64 first;
        u64 opl;
    };
    u64 v[2];
} itv1u64_t;

typedef union itv2f32_t {
    struct {
        vec2f32_t min;
        vec2f32_t max;
    };
    struct {
        vec2f32_t p0;
        vec2f32_t p1;
    };
    struct {
        f32 x0;
        f32 y0;
        f32 x1;
        f32 y1;
    };
    vec2f32_t p[2];
    f32 v[4];
} itv2f32_t;

typedef union itv2i32_t {
    struct {
        vec2i32_t min;
        vec2i32_t max;
    };
    struct {
        vec2i32_t p0;
        vec2i32_t p1;
    };
    struct {
        i32 x0;
        i32 y0;
        i32 x1;
        i32 y1;
    };
    vec2i32_t p[2];
    i32 v[4];
} itv2i32_t;

// ========================================
// Compound Type Functions

vec2i32_t vec2i32(i32 x, i32 y);
vec2f32_t vec2f32(f32 x, f32 y);
vec3f32_t vec3f32(f32 x, f32 y, f32 z);
vec4f32_t vec4f32(f32 x, f32 y, f32 z, f32 w);
itv1f32_t itv1f32(f32 min, f32 max);
itv1u64_t itv1u64(u64 min, u64 max);
itv2f32_t itv2f32(vec2f32_t min, vec2f32_t max);
itv2i32_t itv2i32(vec2i32_t min, vec2i32_t max);

// ========================================
// Compound Type Redefinitions

typedef vec2i32_t point_t;
typedef vec2f32_t vec2_t;
typedef vec3f32_t vec3_t;
typedef vec4f32_t vec4_t;
typedef itv2f32_t rect_t;
typedef itv2i32_t box_t;
typedef itv1f32_t range_t;
typedef itv1u64_t interval_t;

local_fn point_t point(i32 x, i32 y) { return vec2i32(x, y); }
local_fn vec2_t vec2(f32 x, f32 y) { return vec2f32(x, y); }
local_fn vec3_t vec3(f32 x, f32 y, f32 z) { return vec3f32(x, y, z); }
local_fn vec4_t vec4(f32 x, f32 y, f32 z, f32 w) { return vec4f32(x, y, z, w); }
local_fn rect_t rect(vec2_t min, vec2_t max) { return itv2f32(min, max); }
local_fn box_t box(point_t min, point_t max) { return itv2i32(min, max); }
local_fn range_t range(f32 min, f32 max) { return itv1f32(min, max); }
local_fn interval_t interval(u64 min, u64 max) { return itv1u64(min, max); }

// ========================================
// Bytes Type

typedef struct bytes_t {
    void *data;
    size_t size;
} bytes_t;

typedef bool (*compare_func_t)(bytes_t, bytes_t);
typedef u64 (*hash_func_t)(bytes_t);

#define bytes_of(X) ((bytes_t) { .data = &(X), .size = sizeof(X) })
#define bytes_of_str(X) ((bytes_t) { .data = (X), .size = strlen(X) })
#define bytes_to(T, Bytes) *((T *)(Bytes).data)
#define bytes_to_str(Bytes) ((char *)(Bytes).data)

#endif //SHMOCODEBASE_BASE_H
