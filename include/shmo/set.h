//
// Created by scott on 9/19/2023.
//

#ifndef SHMOCODEBASE_SET_H
#define SHMOCODEBASE_SET_H

#include "allocator.h"

typedef struct SetBucket SetBucket;
typedef struct Set Set;

Set *set_create(Allocator *allocator);
void set_destroy(Set *self);
bool set_put(Set *self, Bytes elm);
bool set_remove(Set *self, Bytes elm);
void set_clear(Set *self);
bool set_shrink(Set *self);
usize set_size(const Set *self);
bool set_empty(const Set *self);
bool set_contains(const Set *self, Bytes elm);

typedef struct SetItr {
    Set *set;
    usize slot;
    SetBucket *bucket;
} SetItr;

SetItr set_itr(Set *s);
SetItr set_itr_next(SetItr itr);
bool set_itr_end(SetItr itr);
Bytes set_itr_get(SetItr itr);

#endif //SHMOCODEBASE_SET_H
