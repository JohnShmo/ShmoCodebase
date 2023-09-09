//
// Created by scott on 9/9/2023.
//

#ifndef SHMOCODEBASE_TABLE_H
#define SHMOCODEBASE_TABLE_H

#include "allocator.h"

typedef struct table_pair_t {
    bytes_t key;
    bytes_t val;
} table_pair_t;

typedef struct table_bucket_t table_bucket_t;

typedef struct table_t {
    size_t size;
    size_t bucket_count;
    table_bucket_t *buckets;
    hash_func_t hash_func;
    compare_func_t compare_func;
    heap_allocator_t *allocator;
} table_t;

void table_create(table_t *tb, hash_func_t hash_func, compare_func_t compare_func, heap_allocator_t *allocator);
void table_destroy(table_t *tb);
void table_set(table_t *tb, bytes_t key, bytes_t val);
bytes_t table_get(table_t *tb, bytes_t key);
void table_remove(table_t *tb, bytes_t key);

#endif //SHMOCODEBASE_TABLE_H
