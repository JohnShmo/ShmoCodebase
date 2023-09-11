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

typedef struct table_bucket_t table_bucket_t;

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

void table_create(table_t *tb,
                  size_t key_size,
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

#endif //SHMOCODEBASE_TABLE_H
