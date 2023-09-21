//
// Created by scott on 9/9/2023.
//

#ifndef SHMOCODEBASE_DARRAY_H
#define SHMOCODEBASE_DARRAY_H

#include "allocator.h"

typedef struct Darray Darray;

Darray *darray_create(usize element_size, HeapAllocator *allocator);
void darray_destroy(Darray *da);
void darray_reserve(Darray *da, usize n);
void darray_resize(Darray *da, usize n, const void *fillval);
void darray_shrink(Darray *da);
void darray_clear(Darray *da);
void darray_pushb(Darray *da, const void *val);
void darray_popb(Darray *da);
void darray_pushf(Darray *da, const void *val);
void darray_popf(Darray *da);
void darray_insert(Darray *da, usize index, const void *val);
void darray_remove(Darray *da, usize index);
void *darray_back(Darray *da);
const void *darray_const_back(const Darray *da);
void *darray_front(Darray *da);
const void *darray_const_front(const Darray *da);
void *darray_at(Darray *da, usize index);
const void *darray_const_at(const Darray *da, usize index);
void darray_set(Darray *da, usize index, const void *val);
bool darray_empty(const Darray *da);
usize darray_size(const Darray *da);
usize darray_capacity(const Darray *da);
void *darray_data(Darray *da);
const void *darray_const_data(const Darray *da);

#define DARRAY_DEF(Name, Prefix, T) \
typedef void Name;      \
local_fn Name *Prefix##_create(HeapAllocator *allocator) { return darray_create(sizeof(T), allocator); } \
local_fn void Prefix##_destroy(Name *da) { darray_destroy((Darray *)da); }                                \
local_fn void Prefix##_reserve(Name *da, usize n) { darray_reserve((Darray *)da, n); }                   \
local_fn void Prefix##_resize(Name *da, usize n, const T fillval) { darray_resize((Darray *)da, n, &fillval); } \
local_fn void Prefix##_shrink(Name *da) { darray_shrink((Darray *)da); }                                  \
local_fn void Prefix##_clear(Name *da) { darray_clear((Darray *)da); }                                    \
local_fn void Prefix##_pushb(Name *da, const T val) { darray_pushb((Darray *)da, &val); }                 \
local_fn void Prefix##_popb(Name *da) { darray_popb((Darray *)da); }                                      \
local_fn void Prefix##_pushf(Name *da, const T val) { darray_pushf((Darray *)da, &val); }                 \
local_fn void Prefix##_popf(Name *da) { darray_popf((Darray *)da); }                                      \
local_fn void Prefix##_insert(Name *da, usize index, const T val) { darray_insert((Darray *)da, index, &val); } \
local_fn void Prefix##_remove(Name *da, usize index) { darray_remove((Darray *)da, index); }             \
local_fn T *Prefix##_back(Name *da) { return darray_back((Darray *)da); }                                 \
local_fn const T *Prefix##_const_back(const Name *da) { return darray_const_back((const Darray *)da); }   \
local_fn T *Prefix##_front(Name *da) { return darray_front((Darray *)da); }                               \
local_fn const T *Prefix##_const_front(const Name *da) { return darray_const_front((const Darray *)da); } \
local_fn T *Prefix##_at(Name *da, usize index) { return darray_at((Darray *)da, index); }                \
local_fn const T *Prefix##_const_at(const Name *da, usize index) { return darray_const_at((const Darray *)da, index); } \
local_fn void Prefix##_set(Name *da, usize index, const T val) { darray_set((Darray *)da, index, &val); }\
local_fn bool Prefix##_empty(const Name *da) { return darray_empty((const Darray *)da); }                 \
local_fn usize Prefix##_size(const Name *da) { return darray_size((const Darray *)da); }                 \
local_fn usize Prefix##_capacity(const Name *da) { return darray_capacity((const Darray *)da); }         \
local_fn T *Prefix##_data(Name *da) { return darray_data((Darray *)da); }                                 \
local_fn const T *Prefix##_const_data(const Name *da) { return darray_const_data((const Darray *)da); }

#endif //SHMOCODEBASE_DARRAY_H
