//
// Created by scott on 9/9/2023.
//

#ifndef SHMOCODEBASE_DARRAY_H
#define SHMOCODEBASE_DARRAY_H

#include "allocator.h"

typedef struct darray_t darray_t;

darray_t *darray_create(usize element_size, heap_allocator_t *allocator);
void darray_destroy(darray_t *da);
void darray_reserve(darray_t *da, usize n);
void darray_resize(darray_t *da, usize n, const void *fillval);
void darray_shrink(darray_t *da);
void darray_clear(darray_t *da);
void darray_pushb(darray_t *da, const void *val);
void darray_popb(darray_t *da);
void darray_pushf(darray_t *da, const void *val);
void darray_popf(darray_t *da);
void darray_insert(darray_t *da, usize index, const void *val);
void darray_remove(darray_t *da, usize index);
void *darray_back(darray_t *da);
const void *darray_const_back(const darray_t *da);
void *darray_front(darray_t *da);
const void *darray_const_front(const darray_t *da);
void *darray_at(darray_t *da, usize index);
const void *darray_const_at(const darray_t *da, usize index);
void darray_set(darray_t *da, usize index, const void *val);
bool darray_empty(const darray_t *da);
usize darray_size(const darray_t *da);
usize darray_capacity(const darray_t *da);
void *darray_data(darray_t *da);
const void *darray_const_data(const darray_t *da);

#define DARRAY_DEF(Name, T) \
typedef void Name##_t;      \
local_fn Name##_t *Name##_create(heap_allocator_t *allocator) { return darray_create(sizeof(T), allocator); } \
local_fn void Name##_destroy(Name##_t *da) { darray_destroy((darray_t *)da); }                                \
local_fn void Name##_reserve(Name##_t *da, usize n) { darray_reserve((darray_t *)da, n); }                   \
local_fn void Name##_resize(Name##_t *da, usize n, const T fillval) { darray_resize((darray_t *)da, n, &fillval); } \
local_fn void Name##_shrink(Name##_t *da) { darray_shrink((darray_t *)da); }                                  \
local_fn void Name##_clear(Name##_t *da) { darray_clear((darray_t *)da); }                                    \
local_fn void Name##_pushb(Name##_t *da, const T val) { darray_pushb((darray_t *)da, &val); }                 \
local_fn void Name##_popb(Name##_t *da) { darray_popb((darray_t *)da); }                                      \
local_fn void Name##_pushf(Name##_t *da, const T val) { darray_pushf((darray_t *)da, &val); }                 \
local_fn void Name##_popf(Name##_t *da) { darray_popf((darray_t *)da); }                                      \
local_fn void Name##_insert(Name##_t *da, usize index, const T val) { darray_insert((darray_t *)da, index, &val); } \
local_fn void Name##_remove(Name##_t *da, usize index) { darray_remove((darray_t *)da, index); }             \
local_fn T *Name##_back(Name##_t *da) { return darray_back((darray_t *)da); }                                 \
local_fn const T *Name##_const_back(const Name##_t *da) { return darray_const_back((const darray_t *)da); }   \
local_fn T *Name##_front(Name##_t *da) { return darray_front((darray_t *)da); }                               \
local_fn const T *Name##_const_front(const Name##_t *da) { return darray_const_front((const darray_t *)da); } \
local_fn T *Name##_at(Name##_t *da, usize index) { return darray_at((darray_t *)da, index); }                \
local_fn const T *Name##_const_at(const Name##_t *da, usize index) { return darray_const_at((const darray_t *)da, index); } \
local_fn void Name##_set(Name##_t *da, usize index, const T val) { darray_set((darray_t *)da, index, &val); }\
local_fn bool Name##_empty(const Name##_t *da) { return darray_empty((const darray_t *)da); }                 \
local_fn usize Name##_size(const Name##_t *da) { return darray_size((const darray_t *)da); }                 \
local_fn usize Name##_capacity(const Name##_t *da) { return darray_capacity((const darray_t *)da); }         \
local_fn T *Name##_data(Name##_t *da) { return darray_data((darray_t *)da); }                                 \
local_fn const T *Name##_const_data(const Name##_t *da) { return darray_const_data((const darray_t *)da); }

#endif //SHMOCODEBASE_DARRAY_H