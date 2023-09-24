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

V2i v2i(i32 x, i32 y) {
    return (V2i){ .x = x, .y = y };
}

V2f v2f(f32 x, f32 y) {
    return (V2f){ .x = x, .y = y };
}

V3f v3f(f32 x, f32 y, f32 z) {
    return (V3f){ .x = x, .y = y, .z = z };
}

V4f v4f(f32 x, f32 y, f32 z, f32 w) {
    return (V4f){ .x = x, .y = y, .z = z, .w = w };
}

R1u r1u(u64 begin, u64 end) {
    return (R1u) { .begin = begin, .end = end };
}

R1f r1f(f32 begin, f32 end) {
    return (R1f) { .begin = begin, .end = end };
}

R2i r2i(V2i begin, V2i end) {
    return (R2i) { .begin = begin, .end = end };
}

R2f r2f(V2f begin, V2f end) {
    return (R2f) { .begin = begin, .end = end };
}

Bytes bytes(const u8 *p, usize size) {
    return (Bytes) { .p = p, .size = size };
}

u64 hash_bytes(Bytes v) {
    assert(v.p);
    size_t len = v.size;
    const u64 p = 16777619; // magic number 1
    u64 hash = 2166136261;  // magic number 2
    for (size_t i = 0; i < len; ++i) {
        hash = hash ^ (v.p[i]) * p;
    }
    return hash;
}

i32 compare_bytes(Bytes lhs, Bytes rhs) {
    if (lhs.p == nullptr && rhs.p == nullptr) {
        return 0;
    } else if (lhs.p == nullptr) {
        return -1;
    } else if (rhs.p == nullptr) {
        return 1;
    }

    i32 result = memcmp(lhs.p, rhs.p, min(lhs.size, rhs.size));
    if (result != 0 || lhs.size == rhs.size) {
        return result;
    }
    return lhs.size < rhs.size ? -1 : 1;
}
