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

vec2i32_t vec2i32(i32 x, i32 y) {
    return (vec2i32_t){ x, y };
}

vec2f32_t vec2f32(f32 x, f32 y) {
    return (vec2f32_t){ x, y };
}

vec3f32_t vec3f32(f32 x, f32 y, f32 z) {
    return (vec3f32_t){ x, y, z };
}

vec4f32_t vec4f32(f32 x, f32 y, f32 z, f32 w) {
    return (vec4f32_t){ x, y, z, w };
}