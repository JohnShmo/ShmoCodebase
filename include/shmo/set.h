//
// Created by scott on 9/19/2023.
//

#ifndef SHMOCODEBASE_SET_H
#define SHMOCODEBASE_SET_H

#include "allocator.h"

typedef struct set_bucket_t set_bucket_t;
typedef struct set_t set_t;

set_t *set_create(size_t element_size,
                  hash_func_t hash_func,
                  compare_func_t compare_func,
                  heap_allocator_t *allocator);

void set_destroy(set_t *s);
void set_put(set_t *s, const void *elm);
void set_remove(set_t *s, const void *elm);
void set_clear(set_t *s);
void set_shrink(set_t *s);
size_t set_size(const set_t *s);
bool set_empty(const set_t *s);
bool set_contains(const set_t *s, const void *elm);

typedef struct set_itr_t {
    set_t *set;
    size_t slot;
    set_bucket_t *bucket;
} set_itr_t;

set_itr_t set_itr(set_t *s);
set_itr_t set_itr_next(set_itr_t itr);
bool set_itr_end(set_itr_t itr);
void *set_itr_get(set_itr_t itr);

#endif //SHMOCODEBASE_SET_H
