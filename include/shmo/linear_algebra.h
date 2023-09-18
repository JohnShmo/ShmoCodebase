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

void vec_add(size_t n, f32 dest[n], const f32 a[n], const f32 b[n]);
void vec_sub(size_t n, f32 dest[n], const f32 a[n], const f32 b[n]);
f32 vec_dot(size_t n, const f32 a[n], const f32 b[n]);
void vec_mul(size_t n, f32 dest[n], const f32 a[n], f32 b);
void vec_div(size_t n, f32 dest[n], const f32 a[n], f32 b);
f32 vec_len(size_t n, const f32 a[n]);
void vec_norm(size_t n, f32 dest[n], const f32 a[n]);

v2f_t v2f_add(const v2f_t *a, const v2f_t *b);
v2f_t v2f_sub(const v2f_t *a, const v2f_t *b);
f32 v2f_dot(const v2f_t *a, const v2f_t *b);
f32 v2f_cross(const v2f_t *a, const v2f_t *b);
v2f_t v2f_mul(const v2f_t *a, f32 b);
v2f_t v2f_div(const v2f_t *a, f32 b);
f32 v2f_len(const v2f_t *a);
v2f_t v2f_norm(const v2f_t *a);

v3f_t v3f_add(const v3f_t *a, const v3f_t *b);
v3f_t v3f_sub(const v3f_t *a, const v3f_t *b);
f32 v3f_dot(const v3f_t *a, const v3f_t *b);
v3f_t v3f_cross(const v3f_t *a, const v3f_t *b);
v3f_t v3f_mul(const v3f_t *a, f32 b);
v3f_t v3f_div(const v3f_t *a, f32 b);
f32 v3f_len(const v3f_t *a);
v3f_t v3f_norm(const v3f_t *a);

v4f_t v4f_add(const v4f_t *a, const v4f_t *b);
v4f_t v4f_sub(const v4f_t *a, const v4f_t *b);
f32 v4f_dot(const v4f_t *a, const v4f_t *b);
v4f_t v4f_mul(const v4f_t *a, f32 b);
v4f_t v4f_div(const v4f_t *a, f32 b);
f32 v4f_len(const v4f_t *a);
v4f_t v4f_norm(const v4f_t *a);

m3x3f_t m3x3f_identity(void);
m3x3f_t m3x3f_translation(const v2f_t *v);
m3x3f_t m3x3f_scale(const v2f_t *v);
m3x3f_t m3x3f_reflection(void);
m3x3f_t m3x3f_rotation(f32 degrees);
m3x3f_t m3x3f_shear(const v2f_t *v);
m3x3f_t m3x3f_mul_m3x3f(const m3x3f_t *m1, const m3x3f_t *m2);
v2f_t m3x3f_mul_v2f(const m3x3f_t *m, const v2f_t *v);

#endif //SHMOCODEBASE_LINEAR_ALGEBRA_H
