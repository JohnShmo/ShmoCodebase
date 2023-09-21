//
// Created by scott on 9/9/2023.
//

#ifndef SHMOCODEBASE_TABLE_H
#define SHMOCODEBASE_TABLE_H

#include "allocator.h"

typedef struct TablePair {
    void *key;
    void *val;
} TablePair;
typedef struct TableBucket TableBucket;
typedef struct Table Table;

Table *table_create(usize key_size,
                    usize val_size,
                    HashFunc hash_func,
                    CompareFunc compare_func,
                    HeapAllocator *allocator);

void table_destroy(Table *tb);
void table_put(Table *tb, const void *key, const void *val);
void *table_get(Table *tb, const void *key);
void table_remove(Table *tb, const void *key);
void table_clear(Table *tb);
void table_shrink(Table *tb);
usize table_size(const Table *tb);
bool table_empty(const Table *tb);
bool table_contains(const Table *tb, const void *key);

typedef struct TableItr {
    Table *table;
    usize slot;
    TableBucket *bucket;
} TableItr;

TableItr table_itr(Table *tb);
TableItr table_itr_next(TableItr itr);
bool table_itr_end(TableItr itr);
TablePair *table_itr_get(TableItr itr);

#define TABLE_DEF(Name, Prefix, KeyT, ValT, HashFunc, CompareFunc) \
typedef struct Name##_TablePair { KeyT *key; ValT *val; } Name##_TablePair; \
typedef void Name;                                     \
local_fn Name *Prefix##_create(HeapAllocator *allocator) { return table_create(sizeof(KeyT), sizeof(ValT), HashFunc, CompareFunc, allocator); } \
local_fn void Prefix##_destroy(Name *tb) { table_destroy((Table *)tb); }                                                                         \
local_fn void Prefix##_put(Name *tb, const KeyT key, const ValT val) { table_put((Table *)tb, &key, &val); }                                     \
local_fn ValT *Prefix##_get(Name *tb, const KeyT key) { return table_get((Table *)tb, &key); }                                                   \
local_fn void Prefix##_remove(Name *tb, const KeyT key) { table_remove((Table *)tb, &key); }                                                     \
local_fn void Prefix##_clear(Name *tb) { table_clear((Table *)tb); }                                                                             \
local_fn void Prefix##_shrink(Name *tb) { table_shrink((Table *)tb); }                                                                           \
local_fn usize Prefix##_size(const Name *tb) { return table_size((const Table *)tb); }                                                          \
local_fn bool Prefix##_empty(const Name *tb) { return table_empty((const Table *)tb); }                                                          \
local_fn bool Prefix##_contains(const Name *tb, const KeyT key) { return table_contains((const Table *)tb, &key); }                              \
local_fn TableItr Prefix##_itr(Name *tb) { return table_itr((Table *)tb); }                                                                   \
local_fn TableItr Prefix##_itr_next(TableItr itr) { return table_itr_next(itr); }                                                                \
local_fn bool Prefix##_itr_end(TableItr itr) { return table_itr_end(itr); }                                                                         \
local_fn Name##_TablePair *Name##_itr_get(TableItr itr) { return (Name##_TablePair *)table_itr_get(itr); }

#endif //SHMOCODEBASE_TABLE_H
