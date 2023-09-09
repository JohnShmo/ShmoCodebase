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

void darray_create(darray_t *da, size_t element_size, heap_allocator_t *allocator);
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
bool darray_empty(darray_t *da);
size_t darray_size(darray_t *da);
size_t darray_capacity(darray_t *da);
size_t darray_element_size(darray_t *da);
void *darray_data(darray_t *da);

#endif //SHMOCODEBASE_DARRAY_H
