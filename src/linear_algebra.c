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

V2f v2f_add(V2f a, V2f b) {
    V2f result;
    vec_add(2, result.v, a.v, b.v);
    return result;
}

V2f v2f_sub(V2f a, V2f b) {
    V2f result;
    vec_sub(2, result.v, a.v, b.v);
    return result;
}

f32 v2f_dot(V2f a, V2f b) {
    return vec_dot(2, a.v, b.v);
}

f32 v2f_cross(V2f a, V2f b) {
    return a.x * b.y - a.y * b.x;
}

V2f v2f_mul(V2f a, f32 b) {
    V2f result;
    vec_mul(2, result.v, a.v, b);
    return result;
}

V2f v2f_div(V2f a, f32 b) {
    V2f result;
    vec_div(2, result.v, a.v, b);
    return result;
}

f32 v2f_len(V2f a) {
    return vec_len(2, a.v);
}

V2f v2f_norm(V2f a) {
    V2f result;
    vec_norm(2, result.v, a.v);
    return result;
}

V3f v3f_add(V3f a, V3f b) {
    V3f result;
    vec_add(3, result.v, a.v, b.v);
    return result;
}

V3f v3f_sub(V3f a, V3f b) {
    V3f result;
    vec_sub(3, result.v, a.v, b.v);
    return result;
}

f32 v3f_dot(V3f a, V3f b) {
    return vec_dot(3, a.v, b.v);
}

V3f v3f_cross(V3f a, V3f b) {
    return (V3f) {
        .x = a.y * b.z - a.z * b.y,
        .y = a.x * b.z - a.z * b.x,
        .z = a.x * b.y - a.y * b.x
    };
}

V3f v3f_mul(V3f a, f32 b) {
    V3f result;
    vec_mul(3, result.v, a.v, b);
    return result;
}

V3f v3f_div(V3f a, f32 b) {
    V3f result;
    vec_div(3, result.v, a.v, b);
    return result;
}

f32 v3f_len(V3f a) {
    return vec_len(3, a.v);
}

V3f v3f_norm(V3f a) {
    V3f result;
    vec_norm(3, result.v, a.v);
    return result;
}

V4f v4f_add(V4f a, V4f b) {
    V4f result;
    vec_add(4, result.v, a.v, b.v);
    return result;
}

V4f v4f_sub(V4f a, V4f b) {
    V4f result;
    vec_sub(4, result.v, a.v, b.v);
    return result;
}

f32 v4f_dot(V4f a, V4f b) {
    return vec_dot(4, a.v, b.v);
}

V4f v4f_mul(V4f a, f32 b) {
    V4f result;
    vec_mul(4, result.v, a.v, b);
    return result;
}

V4f v4f_div(V4f a, f32 b) {
    V4f result;
    vec_div(4, result.v, a.v, b);
    return result;
}

f32 v4f_len(V4f a) {
    return vec_len(4, a.v);
}

V4f v4f_norm(V4f a) {
    V4f result;
    vec_norm(4, result.v, a.v);
    return result;
}

M3x3f m3x3f_identity(void) {
    return (M3x3f) {
        {{ 1, 0, 0 },
         { 0, 1, 0 },
         { 0, 0, 1 }},
    };
}

M3x3f m3x3f_translation(V2f v) {
    return (M3x3f) {
        {{ 1, 0, v.x },
         { 0, 1, v.y },
         { 0, 0, 1 }},
    };
}

M3x3f m3x3f_scale(V2f v) {
    return (M3x3f) {
        {{ v.x, 0,    0 },
         { 0,    v.y, 0 },
         { 0,    0,    1 }},
    };
}

M3x3f m3x3f_reflection(void) {
    return (M3x3f) {
        {{ -1, 0, 0 },
         {  0, 1, 0 },
         {  0, 0, 1 }},
    };
}

M3x3f m3x3f_rotation(f32 degrees) {
    f32 t = degrees * DEG_TO_RAD_f32;
    f32 ct = cos_f32(t);
    f32 st = sin_f32(t);
    return (M3x3f) {
        {{ ct, -st, 0 },
         { st,  ct, 0 },
         { 0,   0,  1 }},
    };
}

M3x3f m3x3f_shear(V2f v) {
    return (M3x3f) {
        {{ 1,    v.x, 0 },
         { v.y, 1,    0 },
         { 0,    0,    1 }},
    };
}

M3x3f m3x3f_mul_m3x3f(const M3x3f *m1, const M3x3f *m2) {
    M3x3f result;
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

V2f m3x3f_mul_v2f(const M3x3f *m, V2f v) {
    V2f result;

    // Multiply using homogeneous coordinates
    float w = m->v[0][2] * v.x + m->v[1][2] * v.y + m->v[2][2]; // This is the homogeneous coordinate

    result.x = (m->v[0][0] * v.x + m->v[1][0] * v.y + m->v[2][0]) / w;
    result.y = (m->v[0][1] * v.x + m->v[1][1] * v.y + m->v[2][1]) / w;

    return result;
}