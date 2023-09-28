//
// Created by scott on 9/19/2023.
//

#ifndef SHMOCODEBASE_SET_H
#define SHMOCODEBASE_SET_H

#include "allocator.h"

typedef struct SetBucket SetBucket;
typedef struct Set Set;

Set *set_create(Allocator *allocator);
void set_destroy(Set *s);
bool set_put(Set *s, Bytes elm);
bool set_remove(Set *s, Bytes elm);
void set_clear(Set *s);
bool set_shrink(Set *s);
usize set_size(const Set *s);
bool set_empty(const Set *s);
bool set_contains(const Set *s, Bytes elm);

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
