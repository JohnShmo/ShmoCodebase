//
// Created by scott on 9/11/2023.
//

#include "shmo/linear_algebra.h"

void vec_add(f32 *dest, const f32 *a, const f32 *b, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        dest[i] = a[i] + b[i];
    }
}

void vec_sub(f32 *dest, const f32 *a, const f32 *b, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        dest[i] = a[i] - b[i];
    }
}

f32 vec_dot(const f32 *a, const f32 *b, size_t n) {
    f32 accum = 0;
    for (size_t i = 0; i < n; ++i) {
        accum += a[i] * b[i];
    }
    return accum;
}

void vec_mul(f32 *dest, const f32 *a, f32 b, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        dest[i] = a[i] * b;
    }
}

void vec_div(f32 *dest, const f32 *a, f32 b, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        dest[i] = a[i] / b;
    }
}

f32 vec_len(const f32 *a, size_t n) {
    f32 dot = vec_dot(a, a, n);
    return sqrt_f32(dot);
}

void vec_norm(f32 *dest, const f32 *a, size_t n) {
    f32 len = vec_len(a, n);
    if (len == 0.0f) {
        for (size_t i = 0; i < n; ++i) {
            dest[i] = a[i];
        }
        return;
    }
    for (size_t i = 0; i < n; ++i) {
        dest[i] = a[i] / len;
    }
}

v2f_t v2f_add(v2f_t a, v2f_t b) {
    v2f_t result;
    vec_add(result.v, a.v, b.v, 2);
    return result;
}

v2f_t v2f_sub(v2f_t a, v2f_t b) {
    v2f_t result;
    vec_sub(result.v, a.v, b.v, 2);
    return result;
}

f32 v2f_dot(v2f_t a, v2f_t b) {
    return vec_dot(a.v, b.v, 2);
}

v2f_t v2f_mul(v2f_t a, f32 b) {
    v2f_t result;
    vec_mul(result.v, a.v, b, 2);
    return result;
}

v2f_t v2f_div(v2f_t a, f32 b) {
    v2f_t result;
    vec_div(result.v, a.v, b, 2);
    return result;
}

f32 v2f_len(v2f_t a) {
    return vec_len(a.v, 2);
}

v2f_t v2f_norm(v2f_t a) {
    v2f_t result;
    vec_norm(result.v, a.v, 2);
    return result;
}

v3f_t v3f_add(v3f_t a, v3f_t b) {
    v3f_t result;
    vec_add(result.v, a.v, b.v, 3);
    return result;
}

v3f_t v3f_sub(v3f_t a, v3f_t b) {
    v3f_t result;
    vec_sub(result.v, a.v, b.v, 3);
    return result;
}

f32 v3f_dot(v3f_t a, v3f_t b) {
    return vec_dot(a.v, b.v, 3);
}

v3f_t v3f_mul(v3f_t a, f32 b) {
    v3f_t result;
    vec_mul(result.v, a.v, b, 3);
    return result;
}

v3f_t v3f_div(v3f_t a, f32 b) {
    v3f_t result;
    vec_div(result.v, a.v, b, 3);
    return result;
}

f32 v3f_len(v3f_t a) {
    return vec_len(a.v, 3);
}

v3f_t v3f_norm(v3f_t a) {
    v3f_t result;
    vec_norm(result.v, a.v, 3);
    return result;
}

v4f_t v4f_add(v4f_t a, v4f_t b) {
    v4f_t result;
    vec_add(result.v, a.v, b.v, 4);
    return result;
}

v4f_t v4f_sub(v4f_t a, v4f_t b) {
    v4f_t result;
    vec_sub(result.v, a.v, b.v, 4);
    return result;
}

f32 v4f_dot(v4f_t a, v4f_t b) {
    return vec_dot(a.v, b.v, 4);
}

v4f_t v4f_mul(v4f_t a, f32 b) {
    v4f_t result;
    vec_mul(result.v, a.v, b, 4);
    return result;
}

v4f_t v4f_div(v4f_t a, f32 b) {
    v4f_t result;
    vec_div(result.v, a.v, b, 4);
    return result;
}

f32 v4f_len(v4f_t a) {
    return vec_len(a.v, 4);
}

v4f_t v4f_norm(v4f_t a) {
    v4f_t result;
    vec_norm(result.v, a.v, 4);
    return result;
}