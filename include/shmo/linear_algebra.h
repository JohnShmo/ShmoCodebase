//
// Created by scott on 9/11/2023.
//

#ifndef SHMOCODEBASE_LINEAR_ALGEBRA_H
#define SHMOCODEBASE_LINEAR_ALGEBRA_H

#include "base.h"

void vec_add(f32 *dest, const f32 *a, const f32 *b, size_t n);
void vec_sub(f32 *dest, const f32 *a, const f32 *b, size_t n);
f32 vec_dot(const f32 *a, const f32 *b, size_t n);
void vec_mul(f32 *dest, const f32 *a, f32 b, size_t n);
void vec_div(f32 *dest, const f32 *a, f32 b, size_t n);
f32 vec_len(const f32 *a, size_t n);
void vec_norm(f32 *dest, const f32 *a, size_t n);

v2f_t v2f_add(v2f_t a, v2f_t b);
v2f_t v2f_sub(v2f_t a, v2f_t b);
f32 v2f_dot(v2f_t a, v2f_t b);
v2f_t v2f_mul(v2f_t a, f32 b);
v2f_t v2f_div(v2f_t a, f32 b);
f32 v2f_len(v2f_t a);
v2f_t v2f_norm(v2f_t a);

v3f_t v3f_add(v3f_t a, v3f_t b);
v3f_t v3f_sub(v3f_t a, v3f_t b);
f32 v3f_dot(v3f_t a, v3f_t b);
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

#endif //SHMOCODEBASE_LINEAR_ALGEBRA_H
