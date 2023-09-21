//
// Created by scott on 9/19/2023.
//

#ifndef SHMOCODEBASE_SET_H
#define SHMOCODEBASE_SET_H

#include "allocator.h"

typedef struct SetBucket SetBucket;
typedef struct Set Set;

Set *set_create(usize element_size,
                HashFunc hash_func,
                CompareFunc compare_func,
                HeapAllocator *allocator);

void set_destroy(Set *s);
void set_put(Set *s, const void *elm);
const void *set_get(const Set *s, const void *elm);
void set_remove(Set *s, const void *elm);
void set_clear(Set *s);
void set_shrink(Set *s);
usize set_size(const Set *s);
bool set_empty(const Set *s);
bool set_contains(const Set *s, const void *elm);

typedef struct SetItr {
    Set *set;
    usize slot;
    SetBucket *bucket;
} SetItr;

SetItr set_itr(Set *s);
SetItr set_itr_next(SetItr itr);
bool set_itr_end(SetItr itr);
void *set_itr_get(SetItr itr);

#define SET_DEF(Name, Prefix, T, HashFunc, CompareFunc) \
typedef void Name;                                      \
typedef struct Name##_ReleasedElement { T elm; bool released; } Name##_ReleasedElement; \
local_fn Name *Prefix##_create(HeapAllocator *allocator) { return set_create(sizeof(T), HashFunc, CompareFunc, allocator); } \
local_fn void Prefix##_destroy(Name *s) { set_destroy((Set *)s); }                      \
local_fn void Prefix##_put(Name *s, const T elm) { set_put((Set *)s, &elm); }           \
local_fn const T *Prefix##_get(const Name *s, const T elm) { return (const T *)set_get((const Set *)s, &elm); }              \
local_fn Name##_ReleasedElement Prefix##_remove(Name *s, const T elm) {                 \
    const T *e = Prefix##_get(s, elm);                  \
    Name##_ReleasedElement result = { 0 };              \
    if (e) {                                            \
        result.elm = *(T *)e;                           \
        result.released = true;                         \
    }                                                   \
    set_remove((Set *)s, &elm);                         \
    return result;                                      \
}                                                       \
local_fn void Prefix##_clear(Name *s) { set_clear((Set *)s); }                                                               \
local_fn void Prefix##_shrink(Name *s) { set_shrink((Set *)s); }                                                             \
local_fn usize Prefix##_size(const Name *s) { return set_size((const Set *)s); }                                             \
local_fn bool Prefix##_empty(const Name *s) { return set_empty((const Set *)s); }                                            \
local_fn bool Prefix##_contains(const Name *s, const T elm) { return set_contains((const Set *)s, &elm); }                   \
local_fn SetItr Prefix##_itr(Name *s) { return set_itr((Set *)s); }                                                          \
local_fn SetItr Prefix##_itr_next(SetItr itr) { return set_itr_next(itr); }                                                  \
local_fn bool Prefix##_itr_end(SetItr itr) { return set_itr_end(itr); }                                                      \
local_fn T *Prefix##_itr_get(SetItr itr) { return (T *)set_itr_get(itr); }

#endif //SHMOCODEBASE_SET_H
