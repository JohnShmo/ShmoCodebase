//
// Created by scott on 9/9/2023.
//

#ifndef SHMOCODEBASE_DARRAY_H
#define SHMOCODEBASE_DARRAY_H

#include "allocator.h"

typedef struct Darray Darray;

Darray *darray_create(usize element_size, Allocator *allocator);
void darray_destroy(Darray *da);
bool darray_reserve(Darray *da, usize n);
bool darray_resize(Darray *da, usize n, Bytes fillval);
bool darray_shrink(Darray *da);
void darray_clear(Darray *da);
bool darray_pushb(Darray *da, Bytes val);
bool darray_popb(Darray *da);
bool darray_pushf(Darray *da, Bytes val);
bool darray_popf(Darray *da);
bool darray_insert(Darray *da, usize index, Bytes val);
bool darray_remove(Darray *da, usize index);
Bytes darray_back(const Darray *da);
Bytes darray_front(const Darray *da);
Bytes darray_at(const Darray *da, usize index);
bool darray_setb(Darray *da, Bytes val);
bool darray_setf(Darray *da, Bytes val);
bool darray_set(Darray *da, usize index, Bytes val);
bool darray_empty(const Darray *da);
usize darray_size(const Darray *da);
usize darray_capacity(const Darray *da);
usize darray_elm_size(const Darray *da);
void *darray_data(Darray *da);
const void *darray_const_data(const Darray *da);

#endif //SHMOCODEBASE_DARRAY_H
