//
// Created by scott on 9/9/2023.
//

#ifndef SHMOCODEBASE_TABLE_H
#define SHMOCODEBASE_TABLE_H

#include "allocator.h"

typedef struct table_pair_t {
    void *key;
    void *val;
} table_pair_t;

typedef struct table_bucket_t {
    table_pair_t pair;
    struct table_bucket_t *next;
} table_bucket_t;

typedef struct table_t {
    size_t size;
    size_t slot_count;

    table_bucket_t **slots;
    table_bucket_t *free_buckets;

    size_t key_size;
    size_t val_size;
    hash_func_t hash_func;
    compare_func_t compare_func;
    heap_allocator_t *allocator;
} table_t;

table_t table_create(size_t key_size,
                     size_t val_size,
                     hash_func_t hash_func,
                     compare_func_t compare_func,
                     heap_allocator_t *allocator);

void table_destroy(table_t *tb);
void table_put(table_t *tb, const void *key, const void *val);
void *table_get(table_t *tb, const void *key);
void table_remove(table_t *tb, const void *key);
void table_clear(table_t *tb);
void table_shrink(table_t *tb);
size_t table_size(const table_t *tb);
bool table_empty(const table_t *tb);
bool table_contains(const table_t *tb, const void *key);

typedef struct table_itr_t {
    table_t *table;
    size_t slot;
    table_bucket_t *bucket;
} table_itr_t;

table_itr_t table_itr(table_t *tb);
table_itr_t table_itr_next(table_itr_t itr);
bool table_itr_end(table_itr_t itr);
table_pair_t *table_itr_get(table_itr_t itr);

#define TABLE_DEF(Name, KeyT, ValT, HashFunc, CompareFunc) \
typedef struct Name##_pair_t { KeyT *key; ValT *val; } Name##_pair_t; \
typedef struct Name##_t { table_t impl; } Name##_t;        \
local_fn Name##_t Name##_create(heap_allocator_t *allocator) { return (Name##_t) { table_create(sizeof(KeyT), sizeof(ValT), HashFunc, CompareFunc, allocator) }; } \
local_fn void Name##_destroy(Name##_t *tb) { table_destroy((table_t *)tb); }                                                                                     \
local_fn void Name##_put(Name##_t *tb, const KeyT key, const ValT val) { table_put((table_t *)tb, &key, &val); }                                                 \
local_fn ValT *Name##_get(Name##_t *tb, const KeyT key) { return table_get((table_t *)tb, &key); }                                                               \
local_fn void Name##_remove(Name##_t *tb, const KeyT key) { table_remove((table_t *)tb, &key); }                                                                 \
local_fn void Name##_clear(Name##_t *tb) { table_clear((table_t *)tb); }                                                                                         \
local_fn void Name##_shrink(Name##_t *tb) { table_shrink((table_t *)tb); }                                                                                       \
local_fn size_t Name##_size(const Name##_t *tb) { return table_size((const table_t *)tb); }                                                                      \
local_fn bool Name##_empty(const Name##_t *tb) { return table_empty((const table_t *)tb); }                                                                      \
local_fn bool Name##_contains(const Name##_t *tb, const KeyT key) { return table_contains((const table_t *)tb, &key); }                                          \
local_fn table_itr_t Name##_itr(Name##_t *tb) { return table_itr((table_t *)tb); }                                                                               \
local_fn table_itr_t Name##_itr_next(table_itr_t itr) { return table_itr_next(itr); }                                                                            \
local_fn bool Name##_itr_end(table_itr_t itr) { return table_itr_end(itr); }                                                                                     \
local_fn Name##_pair_t *Name##_itr_get(table_itr_t itr) { return (Name##_pair_t *)table_itr_get(itr); }

#endif //SHMOCODEBASE_TABLE_H
