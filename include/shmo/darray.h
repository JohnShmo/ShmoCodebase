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

#endif //SHMOCODEBASE_DARRAY_H
