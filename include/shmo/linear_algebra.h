//
// Created by scott on 9/11/2023.
//

#ifndef SHMOCODEBASE_LINEAR_ALGEBRA_H
#define SHMOCODEBASE_LINEAR_ALGEBRA_H

#include "base.h"

typedef struct m3x3f_t {
    f32 v[3][3];
} m3x3f_t;

typedef struct m4x4f_t {
    f32 v[4][4];
} m4x4f_t;

void vec_add(usize n, f32 dest[n], const f32 a[n], const f32 b[n]);
void vec_sub(usize n, f32 dest[n], const f32 a[n], const f32 b[n]);
f32 vec_dot(usize n, const f32 a[n], const f32 b[n]);
void vec_mul(usize n, f32 dest[n], const f32 a[n], f32 b);
void vec_div(usize n, f32 dest[n], const f32 a[n], f32 b);
f32 vec_len(usize n, const f32 a[n]);
void vec_norm(usize n, f32 dest[n], const f32 a[n]);

v2f_t v2f_add(v2f_t a, v2f_t b);
v2f_t v2f_sub(v2f_t a, v2f_t b);
f32 v2f_dot(v2f_t a, v2f_t b);
f32 v2f_cross(v2f_t a, v2f_t b);
v2f_t v2f_mul(v2f_t a, f32 b);
v2f_t v2f_div(v2f_t a, f32 b);
f32 v2f_len(v2f_t a);
v2f_t v2f_norm(v2f_t a);

v3f_t v3f_add(v3f_t a, v3f_t b);
v3f_t v3f_sub(v3f_t a, v3f_t b);
f32 v3f_dot(v3f_t a, v3f_t b);
v3f_t v3f_cross(v3f_t a, v3f_t b);
v3f_t v3f_mul(v3f_t a, f32 b);
v3f_t v3f_div(v3f_t a, f32 b);
f32 v3f_len(v3f_t a);
v3f_t v3f_norm(v3f_t a);

v4f_t v4f_add(v4f_t a, v4f_t b);
v4f_t v4f_sub(v4f_t a, v4f_t b);
f32 v4f_dot(v4f_t a, v4f_t b);
v4f_t v4f_mul(v4f_t a, f32 b);
v4f_t v4f_div(v4f_t a, f32 b);
f32 v4f_len(v4f_t a);
v4f_t v4f_norm(v4f_t a);

m3x3f_t m3x3f_identity(void);
m3x3f_t m3x3f_translation(v2f_t v);
m3x3f_t m3x3f_scale(v2f_t v);
m3x3f_t m3x3f_reflection(void);
m3x3f_t m3x3f_rotation(f32 degrees);
m3x3f_t m3x3f_shear(v2f_t v);
m3x3f_t m3x3f_mul_m3x3f(const m3x3f_t *m1, const m3x3f_t *m2);
v2f_t m3x3f_mul_v2f(const m3x3f_t *m, v2f_t v);

#endif //SHMOCODEBASE_LINEAR_ALGEBRA_H
