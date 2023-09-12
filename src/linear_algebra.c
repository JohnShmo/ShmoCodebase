//
// Created by scott on 9/11/2023.
//

#include "shmo/linear_algebra.h"

void vec_add(size_t n, f32 dest[n], const f32 a[n], const f32 b[n]) {
    for (size_t i = 0; i < n; ++i) {
        dest[i] = a[i] + b[i];
    }
}

void vec_sub(size_t n, f32 dest[n], const f32 a[n], const f32 b[n]) {
    for (size_t i = 0; i < n; ++i) {
        dest[i] = a[i] - b[i];
    }
}

f32 vec_dot(size_t n, const f32 a[n], const f32 b[n]) {
    f32 accum = 0;
    for (size_t i = 0; i < n; ++i) {
        accum += a[i] * b[i];
    }
    return accum;
}

void vec_mul(size_t n, f32 dest[n], const f32 a[n], f32 b) {
    for (size_t i = 0; i < n; ++i) {
        dest[i] = a[i] * b;
    }
}

void vec_div(size_t n, f32 dest[n], const f32 a[n], f32 b) {
    for (size_t i = 0; i < n; ++i) {
        dest[i] = a[i] / b;
    }
}

f32 vec_len(size_t n, const f32 a[n]) {
    f32 dot = vec_dot(n, a, a);
    return sqrt_f32(dot);
}

void vec_norm(size_t n, f32 dest[n], const f32 a[n]) {
    f32 len = vec_len(n, a);
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
    vec_add(2, result.v, a.v, b.v);
    return result;
}

v2f_t v2f_sub(v2f_t a, v2f_t b) {
    v2f_t result;
    vec_sub(2, result.v, a.v, b.v);
    return result;
}

f32 v2f_dot(v2f_t a, v2f_t b) {
    return vec_dot(2, a.v, b.v);
}

v2f_t v2f_mul(v2f_t a, f32 b) {
    v2f_t result;
    vec_mul(2, result.v, a.v, b);
    return result;
}

v2f_t v2f_div(v2f_t a, f32 b) {
    v2f_t result;
    vec_div(2, result.v, a.v, b);
    return result;
}

f32 v2f_len(v2f_t a) {
    return vec_len(2, a.v);
}

v2f_t v2f_norm(v2f_t a) {
    v2f_t result;
    vec_norm(2, result.v, a.v);
    return result;
}

v3f_t v3f_add(v3f_t a, v3f_t b) {
    v3f_t result;
    vec_add(3, result.v, a.v, b.v);
    return result;
}

v3f_t v3f_sub(v3f_t a, v3f_t b) {
    v3f_t result;
    vec_sub(3, result.v, a.v, b.v);
    return result;
}

f32 v3f_dot(v3f_t a, v3f_t b) {
    return vec_dot(3, a.v, b.v);
}

v3f_t v3f_mul(v3f_t a, f32 b) {
    v3f_t result;
    vec_mul(3, result.v, a.v, b);
    return result;
}

v3f_t v3f_div(v3f_t a, f32 b) {
    v3f_t result;
    vec_div(3, result.v, a.v, b);
    return result;
}

f32 v3f_len(v3f_t a) {
    return vec_len(3, a.v);
}

v3f_t v3f_norm(v3f_t a) {
    v3f_t result;
    vec_norm(3, result.v, a.v);
    return result;
}

v4f_t v4f_add(v4f_t a, v4f_t b) {
    v4f_t result;
    vec_add(4, result.v, a.v, b.v);
    return result;
}

v4f_t v4f_sub(v4f_t a, v4f_t b) {
    v4f_t result;
    vec_sub(4, result.v, a.v, b.v);
    return result;
}

f32 v4f_dot(v4f_t a, v4f_t b) {
    return vec_dot(4, a.v, b.v);
}

v4f_t v4f_mul(v4f_t a, f32 b) {
    v4f_t result;
    vec_mul(4, result.v, a.v, b);
    return result;
}

v4f_t v4f_div(v4f_t a, f32 b) {
    v4f_t result;
    vec_div(4, result.v, a.v, b);
    return result;
}

f32 v4f_len(v4f_t a) {
    return vec_len(4, a.v);
}

v4f_t v4f_norm(v4f_t a) {
    v4f_t result;
    vec_norm(4, result.v, a.v);
    return result;
}