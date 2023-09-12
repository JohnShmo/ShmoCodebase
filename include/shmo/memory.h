//
// Created by scott on 9/12/2023.
//

#ifndef SHMOCODEBASE_MEMORY_H
#define SHMOCODEBASE_MEMORY_H

#include "allocator.h"

typedef struct ref_t {
    void *p;
    size_t count;
    destructor_func_t destructor_func;
    heap_allocator_t *allocator;
} ref_t;

ref_t *ref_create(size_t n, destructor_func_t destructor_func, heap_allocator_t *allocator);
void ref_inc(ref_t *r);
void ref_dec(ref_t *r);
void *ref_get(ref_t *r);

#endif //SHMOCODEBASE_MEMORY_H
