//
// Created by scott on 9/19/2023.
//

#ifndef SHMOCODEBASE_SET_H
#define SHMOCODEBASE_SET_H

#include "allocator.h"

typedef struct set_bucket_t set_bucket_t;
typedef struct set_t set_t;

set_t *set_create(usize element_size,
                  hash_func_t hash_func,
                  compare_func_t compare_func,
                  heap_allocator_t *allocator);

void set_destroy(set_t *s);
void set_put(set_t *s, const void *elm);
void set_remove(set_t *s, const void *elm);
void set_clear(set_t *s);
void set_shrink(set_t *s);
usize set_size(const set_t *s);
bool set_empty(const set_t *s);
bool set_contains(const set_t *s, const void *elm);

typedef struct set_itr_t {
    set_t *set;
    usize slot;
    set_bucket_t *bucket;
} set_itr_t;

set_itr_t set_itr(set_t *s);
set_itr_t set_itr_next(set_itr_t itr);
bool set_itr_end(set_itr_t itr);
void *set_itr_get(set_itr_t itr);

#define SET_DEF(Name, T, HashFunc, CompareFunc) \
typedef void Name##_t;                          \
local_fn Name##_t *Name##_create(heap_allocator_t *allocator) { return set_create(sizeof(T), HashFunc, CompareFunc, allocator); } \
local_fn void Name##_destroy(Name##_t *s) { set_destroy((set_t *)s); }                                                            \
local_fn void Name##_put(Name##_t *s, const T elm) { set_put((set_t *)s, &elm); }                                                 \
local_fn void Name##_remove(Name##_t *s, const T elm) { set_remove((set_t *)s, &elm); }                                           \
local_fn void Name##_clear(Name##_t *s) { set_clear((set_t *)s); }                                                                \
local_fn void Name##_shrink(Name##_t *s) { set_shrink((set_t *)s); }                                                              \
local_fn usize Name##_size(const Name##_t *s) { return set_size((const set_t *)s); }                                             \
local_fn bool Name##_empty(const Name##_t *s) { return set_empty((const set_t *)s); }                                             \
local_fn bool Name##_contains(const Name##_t *s, const T elm) { return set_contains((const set_t *)s, &elm); }                    \
local_fn set_itr_t Name##_itr(Name##_t *s) { return set_itr((set_t *)s); }                                                        \
local_fn set_itr_t Name##_itr_next(set_itr_t itr) { return set_itr_next(itr); }                                                   \
local_fn bool Name##_itr_end(set_itr_t itr) { return set_itr_end(itr); }                                                          \
local_fn T *Name##_itr_get(set_itr_t itr) { return (T *)set_itr_get(itr); }

#endif //SHMOCODEBASE_SET_H
