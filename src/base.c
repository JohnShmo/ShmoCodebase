//
// Created by scott on 9/8/2023.
//

#define DONT_USE_MATH_MACROS 1
#include "shmo/base.h"

f32 inf_f32(void) {
    union { f32 f; u32 u; } r;
    r.u = 0x7F800000;
    return r.f;
}

f32 neg_inf_f32(void) {
    union { f32 f; u32 u; } r;
    r.u = 0xFF800000;
    return r.f;
}

f64 inf_f64(void) {
    union { f64 f; u64 u; } r;
    r.u = 0x7FF0000000000000;
    return r.f;
}

f64 neg_inf_f64(void) {
    union { f64 f; u64 u; } r;
    r.u = 0xFFF0000000000000;
    return r.f;
}

f32 fabs_f32(f32 x) {
    union { f32 f; u32 u; } r;
    r.f = x;
    r.u &= 0x7FFFFFFF;
    return r.f;
}

f64 fabs_f64(f64 x) {
    union { f64 f; u64 u; } r;
    r.f = x;
    r.u &= 0x7FFFFFFFFFFFFFFF;
    return r.f;
}

#include <math.h>

f32 sqrt_f32(f32 x) {
    return sqrtf(x);
}

f32 sin_f32(f32 x) {
    return sinf(x);
}

f32 cos_f32(f32 x) {
    return cosf(x);
}

f32 tan_f32(f32 x) {
    return tanf(x);
}

f64 sqrt_f64(f64 x) {
    return sqrt(x);
}

f64 sin_f64(f64 x) {
    return sin(x);
}

f64 cos_f64(f64 x) {
    return cos(x);
}

f64 tan_f64(f64 x) {
    return tan(x);
}

f32 lerp_f32(f32 a, f32 b, f32 t) {
    return a + (b - a) * t;
}

f32 inv_lerp_f32(f32 a, f32 b, f32 x) {
    f32 t = 0;
    if (a != b) {
        t = (x - a) / (b - a);
    }
    return t;
}

f64 lerp_f64(f64 a, f64 b, f64 t) {
    return a + (b - a) * t;
}

f64 inv_lerp_f64(f64 a, f64 b, f64 x) {
    f64 t = 0;
    if (a != b) {
        t = (x - a) / (b - a);
    }
    return t;
}

i32 round_up_i32(i32 num, i32 multiple) {
    assert(multiple);
    return ((num + multiple - 1) / multiple) * multiple;
}

i64 round_up_i64(i64 num, i64 multiple) {
    assert(multiple);
    return ((num + multiple - 1) / multiple) * multiple;
}

u32 round_up_u32(u32 num, u32 multiple) {
    assert(multiple);
    return ((num + multiple - 1) / multiple) * multiple;
}

u64 round_up_u64(u64 num, u64 multiple) {
    assert(multiple);
    return ((num + multiple - 1) / multiple) * multiple;
}

v2i_t v2i(i32 x, i32 y) {
    return (v2i_t){ .x = x, .y = y };
}

v2f_t v2f(f32 x, f32 y) {
    return (v2f_t){ .x = x, .y = y };
}

v3f_t v3f(f32 x, f32 y, f32 z) {
    return (v3f_t){ .x = x, .y = y, .z = z };
}

v4f_t v4f(f32 x, f32 y, f32 z, f32 w) {
    return (v4f_t){ .x = x, .y = y, .z = z, .w = w };
}

r1u_t r1u(u64 min, u64 max) {
    return (r1u_t) { .min = min, .max = max };
}

r1f_t r1f(f32 min, f32 max) {
    return (r1f_t) { .min = min, .max = max };
}

r2i_t r2i(v2i_t min, v2i_t max) {
    return (r2i_t) { .min = min, .max = max };
}

r2f_t r2f(v2f_t min, v2f_t max) {
    return (r2f_t) { .min = min, .max = max };
}

u64 hash_cstr(const void *v) {
    assert(v);
    const char *str = *(const char **)v;
    size_t len = strlen(str);
    const u64 p = 16777619; // magic number 1
    u64 hash = 2166136261;  // magic number 2
    for (size_t i = 0; i < len; ++i) {
        hash = hash ^ (str[i]) * p;
    }
    return hash;
}

i32 compare_cstr(const void *lhs, const void *rhs) {
    assert(lhs);
    assert(rhs);
    return strcmp(*(const char **)lhs, *(const char **)rhs);
}