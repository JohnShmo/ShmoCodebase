//
// Created by scott on 9/9/2023.
//

#ifndef SHMOCODEBASE_DARRAY_H
#define SHMOCODEBASE_DARRAY_H

#include "allocator.h"

typedef struct darray_t {
    size_t size;
    size_t capacity;
    size_t element_size;
    void *data;
    heap_allocator_t *allocator;
} darray_t;

darray_t *darray_create(size_t element_size, heap_allocator_t *allocator);
void darray_destroy(darray_t *da);
void darray_reserve(darray_t *da, size_t n);
void darray_resize(darray_t *da, size_t n, const void *fillval);
void darray_shrink(darray_t *da);
void darray_clear(darray_t *da);
void darray_pushb(darray_t *da, const void *val);
void darray_popb(darray_t *da);
void darray_pushf(darray_t *da, const void *val);
void darray_popf(darray_t *da);
void darray_insert(darray_t *da, size_t index, const void *val);
void darray_remove(darray_t *da, size_t index);
void *darray_back(darray_t *da);
void *darray_front(darray_t *da);
void *darray_at(darray_t *da, size_t index);
void darray_set(darray_t *da, size_t index, const void *val);
bool darray_empty(const darray_t *da);
size_t darray_size(const darray_t *da);
size_t darray_capacity(const darray_t *da);
size_t darray_element_size(const darray_t *da);
const void *darray_data(const darray_t *da);

#define DARRAY_DEF(Name, T) \
typedef struct Name##_t {   \
    darray_t impl;          \
} Name##_t;                 \
local_fn Name##_t *Name##_create(heap_allocator_t *allocator) { return (Name##_t *)darray_create(sizeof(T), allocator); }\
local_fn void Name##_destroy(Name##_t *da) { darray_destroy((darray_t *)da); }                                           \
local_fn void Name##_reserve(Name##_t *da, size_t n) { darray_reserve((darray_t *)da, n); }                              \
local_fn void Name##_resize(Name##_t *da, size_t n, T fillval) { darray_resize((darray_t *)da, n, &fillval); }           \
local_fn void Name##_shrink(Name##_t *da) { darray_shrink((darray_t *)da); }                                             \
local_fn void Name##_clear(Name##_t *da) { darray_clear((darray_t *)da); }                                               \
local_fn void Name##_pushb(Name##_t *da, T val) { darray_pushb((darray_t *)da, &val); }                                  \
local_fn void Name##_popb(Name##_t *da) { darray_popb((darray_t *)da); }                                                 \
local_fn void Name##_pushf(Name##_t *da, T val) { darray_pushf((darray_t *)da, &val); }                                  \
local_fn void Name##_popf(Name##_t *da) { darray_popf((darray_t *)da); }                                                 \
local_fn void Name##_insert(Name##_t *da, size_t index, T val) { darray_insert((darray_t *)da, index, &val); }           \
local_fn void Name##_remove(Name##_t *da, size_t index) { darray_remove((darray_t *)da, index); }                        \
local_fn T *Name##_back(Name##_t *da) { return darray_back((darray_t *)da); }                                            \
local_fn T *Name##_front(Name##_t *da) { return darray_front((darray_t *)da); }                                          \
local_fn T *Name##_at(Name##_t *da, size_t index) { return darray_at((darray_t *)da, index); }                           \
local_fn void Name##_set(Name##_t *da, size_t index, T val) { darray_set((darray_t *)da, index, &val); }                 \
local_fn bool Name##_empty(Name##_t *da) { return darray_empty((darray_t *)da); }                                        \
local_fn size_t Name##_size(Name##_t *da) { return darray_size((darray_t *)da); }                                        \
local_fn size_t Name##_capacity(Name##_t *da) { return darray_capacity((darray_t *)da); }                                \
local_fn const T *Name##_data(Name##_t *da) { return darray_data((darray_t *)da); }

#endif //SHMOCODEBASE_DARRAY_H

