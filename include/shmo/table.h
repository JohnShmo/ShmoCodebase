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

Table *table_create(Allocator *allocator);
void table_destroy(Table *self);
bool table_put(Table *self, Bytes key, Bytes val);
Bytes table_get(const Table *self, Bytes key);
bool table_remove(Table *self, Bytes key);
void table_clear(Table *self);
bool table_shrink(Table *self);
usize table_size(const Table *self);
bool table_empty(const Table *self);
bool table_contains(const Table *self, Bytes key);

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
