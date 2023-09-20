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

r1u_t r1u(u64 begin, u64 end) {
    return (r1u_t) { .begin = begin, .end = end };
}

r1f_t r1f(f32 begin, f32 end) {
    return (r1f_t) { .begin = begin, .end = end };
}

r2i_t r2i(v2i_t begin, v2i_t end) {
    return (r2i_t) { .begin = begin, .end = end };
}

r2f_t r2f(v2f_t begin, v2f_t end) {
    return (r2f_t) { .begin = begin, .end = end };
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

i32 compare_i8(const void *lhs, const void *rhs) {
    assert(lhs);
    assert(rhs);
    const i8 *a = lhs, *b = rhs;
    return (i32)(*a > *b) - (i32)(*a < *b);
}

u64 hash_i8(const void *v) {
    assert(v);
    union {
        i8 i8_;
        u64 u64_;
    } convert = {
            .i8_ = *(i8 *)v
    };
    return convert.u64_;
}

i32 compare_i16(const void *lhs, const void *rhs) {
    assert(lhs);
    assert(rhs);
    const i16 *a = lhs, *b = rhs;
    return (i32)(*a > *b) - (i32)(*a < *b);
}

u64 hash_i16(const void *v) {
    assert(v);
    union {
        i16 i16_;
        u64 u64_;
    } convert = {
            .i16_ = *(i16 *)v
    };
    return convert.u64_;
}

i32 compare_i32(const void *lhs, const void *rhs) {
    assert(lhs);
    assert(rhs);
    const i32 *a = lhs, *b = rhs;
    return (i32)(*a > *b) - (i32)(*a < *b);
}

u64 hash_i32(const void *v) {
    assert(v);
    union {
        i32 i32_;
        u64 u64_;
    } convert = {
            .i32_ = *(i32 *)v
    };
    return convert.u64_;
}

i32 compare_i64(const void *lhs, const void *rhs) {
    assert(lhs);
    assert(rhs);
    const i64 *a = lhs, *b = rhs;
    return (i32)(*a > *b) - (i32)(*a < *b);
}

u64 hash_i64(const void *v) {
    assert(v);
    union {
        i64 i64_;
        u64 u64_;
    } convert = {
            .i64_ = *(i64 *)v
    };
    return convert.u64_;
}

i32 compare_isize(const void *lhs, const void *rhs) {
    assert(lhs);
    assert(rhs);
    const isize *a = lhs, *b = rhs;
    return (i32)(*a > *b) - (i32)(*a < *b);
}

u64 hash_isize(const void *v) {
    assert(v);
    union {
        isize isize_;
        u64 u64_;
    } convert = {
            .isize_ = *(isize *)v
    };
    return convert.u64_;
}

i32 compare_u8(const void *lhs, const void *rhs) {
    assert(lhs);
    assert(rhs);
    const u8 *a = lhs, *b = rhs;
    return (i32)(*a > *b) - (i32)(*a < *b);
}

u64 hash_u8(const void *v) {
    assert(v);
    union {
        u8 u8_;
        u64 u64_;
    } convert = {
            .u8_ = *(u8 *)v
    };
    return convert.u64_;
}

i32 compare_u16(const void *lhs, const void *rhs) {
    assert(lhs);
    assert(rhs);
    const u16 *a = lhs, *b = rhs;
    return (i32)(*a > *b) - (i32)(*a < *b);
}

u64 hash_u16(const void *v) {
    assert(v);
    union {
        u16 u16_;
        u64 u64_;
    } convert = {
            .u16_ = *(u16 *)v
    };
    return convert.u64_;
}

i32 compare_u32(const void *lhs, const void *rhs) {
    assert(lhs);
    assert(rhs);
    const u32 *a = lhs, *b = rhs;
    return (i32)(*a > *b) - (i32)(*a < *b);
}

u64 hash_u32(const void *v) {
    assert(v);
    union {
        u32 u32_;
        u64 u64_;
    } convert = {
            .u32_ = *(u32 *)v
    };
    return convert.u64_;
}

i32 compare_u64(const void *lhs, const void *rhs) {
    assert(lhs);
    assert(rhs);
    const u64 *a = lhs, *b = rhs;
    return (i32)(*a > *b) - (i32)(*a < *b);
}

u64 hash_u64(const void *v) {
    assert(v);
    return *(u64 *)v;
}

i32 compare_usize(const void *lhs, const void *rhs) {
    assert(lhs);
    assert(rhs);
    const usize *a = lhs, *b = rhs;
    return (i32)(*a > *b) - (i32)(*a < *b);
}

u64 hash_usize(const void *v) {
    assert(v);
    union {
        usize usize_;
        u64 u64_;
    } convert = {
            .usize_ = *(isize *)v
    };
    return convert.u64_;
}