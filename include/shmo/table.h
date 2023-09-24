//
// Created by scott on 9/9/2023.
//

#ifndef SHMOCODEBASE_TABLE_H
#define SHMOCODEBASE_TABLE_H

#include "allocator.h"

typedef struct TablePair {
    Bytes key;
    Bytes val;
} TablePair;
typedef struct TableBucket TableBucket;
typedef struct Table Table;

Table *table_create(HeapAllocator *allocator);
void table_destroy(Table *tb);
bool table_put(Table *tb, Bytes key, Bytes val);
Bytes table_get(const Table *tb, Bytes key);
bool table_remove(Table *tb, Bytes key);
void table_clear(Table *tb);
bool table_shrink(Table *tb);
usize table_size(const Table *tb);
bool table_empty(const Table *tb);
bool table_contains(const Table *tb, Bytes key);

typedef struct TableItr {
    Table *table;
    usize slot;
    TableBucket *bucket;
} TableItr;

TableItr table_itr(Table *tb);
TableItr table_itr_next(TableItr itr);
bool table_itr_end(TableItr itr);
TablePair table_itr_get(TableItr itr);

#endif //SHMOCODEBASE_TABLE_H
