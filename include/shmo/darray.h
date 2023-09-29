//
// Created by scott on 9/9/2023.
//

#ifndef SHMOCODEBASE_DARRAY_H
#define SHMOCODEBASE_DARRAY_H

#include "allocator.h"

typedef struct Darray Darray;

Darray *darray_create(usize element_size, Allocator *allocator);
void darray_destroy(Darray *self);
bool darray_reserve(Darray *self, usize n);
bool darray_resize(Darray *self, usize n, Bytes fillval);
bool darray_shrink(Darray *self);
void darray_clear(Darray *self);
bool darray_pushb(Darray *self, Bytes val);
bool darray_popb(Darray *self);
bool darray_pushf(Darray *self, Bytes val);
bool darray_popf(Darray *self);
bool darray_insert(Darray *self, usize index, Bytes val);
bool darray_remove(Darray *self, usize index);
Bytes darray_back(const Darray *self);
Bytes darray_front(const Darray *self);
Bytes darray_at(const Darray *self, usize index);
bool darray_setb(Darray *self, Bytes val);
bool darray_setf(Darray *self, Bytes val);
bool darray_set(Darray *self, usize index, Bytes val);
bool darray_empty(const Darray *self);
usize darray_size(const Darray *self);
usize darray_capacity(const Darray *self);
usize darray_elm_size(const Darray *self);
void *darray_data(Darray *self);
const void *darray_const_data(const Darray *self);

#endif //SHMOCODEBASE_DARRAY_H
