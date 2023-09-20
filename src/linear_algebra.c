//
// Created by scott on 9/11/2023.
//

#include "shmo/linear_algebra.h"

void vec_add(usize n, f32 dest[n], const f32 a[n], const f32 b[n]) {
    for (usize i = 0; i < n; ++i) {
        dest[i] = a[i] + b[i];
    }
}

void vec_sub(usize n, f32 dest[n], const f32 a[n], const f32 b[n]) {
    for (usize i = 0; i < n; ++i) {
        dest[i] = a[i] - b[i];
    }
}

f32 vec_dot(usize n, const f32 a[n], const f32 b[n]) {
    f32 accum = 0;
    for (usize i = 0; i < n; ++i) {
        accum += a[i] * b[i];
    }
    return accum;
}

void vec_mul(usize n, f32 dest[n], const f32 a[n], f32 b) {
    for (usize i = 0; i < n; ++i) {
        dest[i] = a[i] * b;
    }
}

void vec_div(usize n, f32 dest[n], const f32 a[n], f32 b) {
    for (usize i = 0; i < n; ++i) {
        dest[i] = a[i] / b;
    }
}

f32 vec_len(usize n, const f32 a[n]) {
    f32 dot = vec_dot(n, a, a);
    return sqrt_f32(dot);
}

void vec_norm(usize n, f32 dest[n], const f32 a[n]) {
    f32 len = vec_len(n, a);
    if (len == 0.0f) {
        for (usize i = 0; i < n; ++i) {
            dest[i] = a[i];
        }
        return;
    }
    for (usize i = 0; i < n; ++i) {
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

f32 v2f_cross(v2f_t a, v2f_t b) {
    return a.x * b.y - a.y * b.x;
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

v3f_t v3f_cross(v3f_t a, v3f_t b) {
    return (v3f_t) {
        .x = a.y * b.z - a.z * b.y,
        .y = a.x * b.z - a.z * b.x,
        .z = a.x * b.y - a.y * b.x
    };
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

m3x3f_t m3x3f_identity(void) {
    return (m3x3f_t) {
        {{ 1, 0, 0 },
         { 0, 1, 0 },
         { 0, 0, 1 }},
    };
}

m3x3f_t m3x3f_translation(v2f_t v) {
    return (m3x3f_t) {
        {{ 1, 0, v.x },
         { 0, 1, v.y },
         { 0, 0, 1 }},
    };
}

m3x3f_t m3x3f_scale(v2f_t v) {
    return (m3x3f_t) {
        {{ v.x, 0,    0 },
         { 0,    v.y, 0 },
         { 0,    0,    1 }},
    };
}

m3x3f_t m3x3f_reflection(void) {
    return (m3x3f_t) {
        {{ -1, 0, 0 },
         {  0, 1, 0 },
         {  0, 0, 1 }},
    };
}

m3x3f_t m3x3f_rotation(f32 degrees) {
    f32 t = degrees * DEG_TO_RAD_f32;
    f32 ct = cos_f32(t);
    f32 st = sin_f32(t);
    return (m3x3f_t) {
        {{ ct, -st, 0 },
         { st,  ct, 0 },
         { 0,   0,  1 }},
    };
}

m3x3f_t m3x3f_shear(v2f_t v) {
    return (m3x3f_t) {
        {{ 1,    v.x, 0 },
         { v.y, 1,    0 },
         { 0,    0,    1 }},
    };
}

m3x3f_t m3x3f_mul_m3x3f(const m3x3f_t *m1, const m3x3f_t *m2) {
    m3x3f_t result;
    const i32 n = 3;
    for (i32 i = 0; i < n; ++i) {
        for (i32 j = 0; j < n; ++j) {
            f32 sum = 0;
            for (i32 k = 0; k < n; ++k) {
                sum += m1->v[k][i] * m2->v[j][k];
            }
            result.v[j][i] = sum;
        }
    }
    return result;
}

v2f_t m3x3f_mul_v2f(const m3x3f_t *m, v2f_t v) {
    v2f_t result;

    // Multiply using homogeneous coordinates
    float w = m->v[0][2] * v.x + m->v[1][2] * v.y + m->v[2][2]; // This is the homogeneous coordinate

    result.x = (m->v[0][0] * v.x + m->v[1][0] * v.y + m->v[2][0]) / w;
    result.y = (m->v[0][1] * v.x + m->v[1][1] * v.y + m->v[2][1]) / w;

    return result;
}