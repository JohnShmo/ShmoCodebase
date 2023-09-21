//
// Created by scott on 9/11/2023.
//

#ifndef SHMOCODEBASE_LINEAR_ALGEBRA_H
#define SHMOCODEBASE_LINEAR_ALGEBRA_H

#include "base.h"

typedef struct M3x3f {
    f32 v[3][3];
} M3x3f;

typedef struct M4x4f {
    f32 v[4][4];
} M4x4f;

void vec_add(usize n, f32 dest[n], const f32 a[n], const f32 b[n]);
void vec_sub(usize n, f32 dest[n], const f32 a[n], const f32 b[n]);
f32 vec_dot(usize n, const f32 a[n], const f32 b[n]);
void vec_mul(usize n, f32 dest[n], const f32 a[n], f32 b);
void vec_div(usize n, f32 dest[n], const f32 a[n], f32 b);
f32 vec_len(usize n, const f32 a[n]);
void vec_norm(usize n, f32 dest[n], const f32 a[n]);

V2f v2f_add(V2f a, V2f b);
V2f v2f_sub(V2f a, V2f b);
f32 v2f_dot(V2f a, V2f b);
f32 v2f_cross(V2f a, V2f b);
V2f v2f_mul(V2f a, f32 b);
V2f v2f_div(V2f a, f32 b);
f32 v2f_len(V2f a);
V2f v2f_norm(V2f a);

V3f v3f_add(V3f a, V3f b);
V3f v3f_sub(V3f a, V3f b);
f32 v3f_dot(V3f a, V3f b);
V3f v3f_cross(V3f a, V3f b);
V3f v3f_mul(V3f a, f32 b);
V3f v3f_div(V3f a, f32 b);
f32 v3f_len(V3f a);
V3f v3f_norm(V3f a);

V4f v4f_add(V4f a, V4f b);
V4f v4f_sub(V4f a, V4f b);
f32 v4f_dot(V4f a, V4f b);
V4f v4f_mul(V4f a, f32 b);
V4f v4f_div(V4f a, f32 b);
f32 v4f_len(V4f a);
V4f v4f_norm(V4f a);

M3x3f m3x3f_identity(void);
M3x3f m3x3f_translation(V2f v);
M3x3f m3x3f_scale(V2f v);
M3x3f m3x3f_reflection(void);
M3x3f m3x3f_rotation(f32 degrees);
M3x3f m3x3f_shear(V2f v);
M3x3f m3x3f_mul_m3x3f(const M3x3f *m1, const M3x3f *m2);
V2f m3x3f_mul_v2f(const M3x3f *m, V2f v);

#endif //SHMOCODEBASE_LINEAR_ALGEBRA_H
