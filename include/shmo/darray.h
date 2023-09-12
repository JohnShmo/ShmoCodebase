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

#endif //SHMOCODEBASE_DARRAY_H
