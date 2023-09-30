//
// Created by scott on 9/9/2023.
//

#include "shmo/table.h"

typedef struct TablePairInternal {
    void *key;
    usize key_size;
    void *val;
    usize val_size;
} TablePairInternal;

struct TableBucket {
    TablePairInternal pair;
    struct TableBucket *next;
};

struct Table {
    usize size;
    usize slot_count;

    TableBucket **slots;
    TableBucket *free_buckets;

    Allocator *allocator;
};

local_fn bool table_bucket_create(TableBucket *bucket,
                                  const void *key,
                                  usize key_size,
                                  const void *val,
                                  usize val_size,
                                  Allocator *allocator) {
    assert(bucket);
    assert(key);
    assert(key_size);
    assert(val);
    assert(val_size);
    assert(allocator);

    u8 *key_copy = nullptr;
    u8 *val_copy = nullptr;

    key_copy = allocator_malloc(allocator, key_size);
    if (!key_copy)
        goto error;
    val_copy = allocator_malloc(allocator, val_size);
    if (!val_copy)
        goto error;

    memcpy(key_copy, key, key_size);
    memcpy(val_copy, val, val_size);

    bucket->next = nullptr;
    bucket->pair.key = key_copy;
    bucket->pair.key_size = key_size;
    bucket->pair.val = val_copy;
    bucket->pair.val_size = val_size;

    return true;
    
    error:
        if (key_copy)
            allocator_free(allocator, key_copy);
        if (val_copy)
            allocator_free(allocator, val_copy);
    return false;
}

local_fn bool table_bucket_put_val(TableBucket *bucket, const void *val, usize val_size, Allocator *allocator) {
    assert(bucket);
    assert(val);
    assert(val_size);
    assert(allocator);

    u8 *new_mem = nullptr;
    new_mem = allocator_realloc(allocator, bucket->pair.val, val_size);
    if (!new_mem)
        goto error;

    bucket->pair.val = new_mem;
    memory_copy(bucket->pair.val, val, val_size);
    bucket->pair.val_size = val_size;

    return true;

    error:
    return false;
}

local_fn void table_bucket_destroy(TableBucket *bucket, Allocator *allocator) {
    assert(bucket);
    assert(allocator);

    allocator_free(allocator, bucket->pair.key);
    allocator_free(allocator, bucket->pair.val);

    bucket->pair.key = nullptr;
    bucket->pair.val = nullptr;
    bucket->next = nullptr;
}

local_fn bool table_rehash(Table *self, usize new_count) {
    assert(self);

    if (new_count == 0) {
        allocator_free(self->allocator, self->slots);
        self->slots = nullptr;
        self->slot_count = 0;
        return true;
    }

    TableBucket **old_slots = self->slots;
    usize old_count = self->slot_count;

    TableBucket **new_slots = nullptr;

    new_slots = allocator_calloc(self->allocator, new_count, sizeof(TableBucket *));
    if (!new_slots)
        goto error;
    self->slots = new_slots;
    self->slot_count = new_count;

    for (usize i = 0; i < old_count; ++i) {
        TableBucket *bucket = old_slots[i];
        while (bucket) {
            TableBucket *next = bucket->next;
            usize hash = hash_bytes((Bytes) { .p = bucket->pair.key, .size = bucket->pair.key_size } ) % new_count;
            bucket->next = self->slots[hash];
            self->slots[hash] = bucket;
            bucket = next;
        }
    }

    if (old_slots) {
        allocator_free(self->allocator, old_slots);
    }

    return true;

    error:
    return false;
}

local_fn TableBucket *table_lookup(const Table *self, Bytes key) {
    assert(self);
    assert(key.p);

    if (table_empty(self)) {
        return nullptr;
    }

    usize hash = hash_bytes(key) % self->slot_count;
    TableBucket *bucket = self->slots[hash];
    while(bucket) {
        bool match = compare_bytes((Bytes) { .p = bucket->pair.key, .size = bucket->pair.key_size }, key) == 0;
        if (match) {
            break;
        }
        bucket = bucket->next;
    }
    return bucket;
}

Table *table_create(Allocator *allocator) {
    if (!allocator)
        allocator = stdalloc;

    Table *dest = allocator_malloc(allocator, sizeof(Table));
    if (!dest)
        return nullptr;

    dest->size = 0;
    dest->slot_count = 0;
    dest->slots = nullptr;
    dest->free_buckets = nullptr;
    dest->allocator = allocator;
    return dest;
}

void table_destroy(Table *self) {
    if (!self) {
        return;
    }

    table_clear(self);
    table_shrink(self);

    allocator_free(self->allocator, self);
}

bool table_put(Table *self, Bytes key, Bytes val) {
    assert(self);
    if (!key.p || !val.p)
        return false;

    TableBucket *new_bucket = nullptr;

    f64 load;
    if (table_empty(self)) {
        load = 1.0;
    } else {
        load = (f64)self->size / (f64)self->slot_count;
    }

    if (load > 0.75) {
        if (!table_rehash(self, self->slot_count * 4 + 16))
            goto error;
    }

    TableBucket *bucket = table_lookup(self, key);
    
    if (bucket) {
        table_bucket_put_val(bucket, val.p, val.size, self->allocator);
    } else {
        usize hash = hash_bytes(key) % self->slot_count;
        new_bucket = allocator_malloc(self->allocator, sizeof(TableBucket));
        if (!new_bucket)
            goto error;
        if (!table_bucket_create(new_bucket, key.p, key.size, val.p, val.size, self->allocator))
            goto error;
        new_bucket->next = self->slots[hash];
        self->slots[hash] = new_bucket;
        self->size++;        
    }

    return true;

    error:
    if (new_bucket) {
        allocator_free(self->allocator, new_bucket);
    }
    return false;
}

Bytes table_get(const Table *self, Bytes key) {
    assert(self);
    if (!key.p)
        return nullbytes;

    TableBucket *bucket = table_lookup(self, key);
    if (bucket) {
        return (Bytes) { .p = bucket->pair.val, .size = bucket->pair.val_size };
    }

    return nullbytes;
}

bool table_remove(Table *self, Bytes key) {
    assert(self);
    if (!key.p)
        return false;

    TableBucket *bucket = table_lookup(self, key);
    if (!bucket) {
        return false;
    }

    usize hash = hash_bytes(key) % self->slot_count;
    TableBucket *first_bucket = self->slots[hash];

    TablePairInternal temp = first_bucket->pair;
    first_bucket->pair = bucket->pair;
    bucket->pair = temp;

    self->slots[hash] = first_bucket->next;
    table_bucket_destroy(first_bucket, self->allocator);

    first_bucket->next = self->free_buckets;
    self->free_buckets = first_bucket;

    return true;
}

void table_clear(Table *self) {
    assert(self);
    if (self->size == 0) {
        return;
    }

    for (usize i = 0; i < self->slot_count; ++i) {
        TableBucket *bucket = self->slots[i];
        while (bucket) {
            TableBucket *next = bucket->next;
            self->slots[i] = next;

            table_bucket_destroy(bucket, self->allocator);

            bucket->next = self->free_buckets;
            self->free_buckets = bucket;

            bucket = next;
        }
    }

    self->size = 0;
}

bool table_shrink(Table *self) {
    assert(self);

    if (!table_rehash(self, (usize)((f64)self->size * 1.25))) {
        return false;
    }

    while (self->free_buckets) {
        TableBucket *to_free = self->free_buckets;
        self->free_buckets = to_free->next;
        allocator_free(self->allocator, to_free);
    }

    return true;
}

usize table_size(const Table *self) {
    assert(self);
    return self->size;
}

bool table_empty(const Table *self) {
    assert(self);
    return self->size == 0 || self->slot_count == 0;
}

bool table_contains(const Table *self, Bytes key) {
    assert(self);
    if (!key.p)
        return false;
    return table_lookup(self, key) != nullptr;
}

TableItr table_itr(Table *self) {
    TableItr itr;
    itr.table = self;
    itr.slot = 0;
    itr.bucket = nullptr;

    return table_itr_next(itr);
}

TableItr table_itr_next(TableItr itr) {
    if (!itr.table || table_empty(itr.table)) {
        return itr;
    }

    usize slot = itr.slot;
    TableBucket *bucket;

    if (itr.bucket) {
        bucket = itr.bucket->next;
    } else {
        bucket = itr.table->slots[slot];
    }

    while (!bucket) {
        slot++;
        if (slot >= itr.table->slot_count) {
            break;
        }
        bucket = itr.table->slots[slot];
    }

    itr.slot = slot;
    itr.bucket = bucket;

    return itr;
}

bool table_itr_end(TableItr itr) {
    return !itr.table || itr.slot >= itr.table->slot_count;
}

TablePair table_itr_get(TableItr itr) {
    if (!itr.table || !itr.bucket)
        return (TablePair) { .key = nullbytes, .val = nullbytes };
    return (TablePair) {
        .key = bytes(itr.bucket->pair.key, itr.bucket->pair.key_size),
        .val = bytes(itr.bucket->pair.val, itr.bucket->pair.val_size)
    };
}
