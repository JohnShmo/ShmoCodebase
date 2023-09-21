//
// Created by scott on 9/9/2023.
//

#include "shmo/table.h"

struct TableBucket {
    TablePair pair;
    struct TableBucket *next;
};

struct Table {
    usize size;
    usize slot_count;

    TableBucket **slots;
    TableBucket *free_buckets;

    usize key_size;
    usize val_size;
    HashFunc hash_func;
    CompareFunc compare_func;
    HeapAllocator *allocator;
};

local_fn void table_bucket_create(TableBucket *bucket,
                                  const void *key,
                                  usize key_size,
                                  const void *val,
                                  usize val_size,
                                  HeapAllocator *allocator) {
    assert(bucket);
    assert(key);
    assert(key_size);
    assert(val);
    assert(val_size);
    assert(allocator);

    u8 *key_copy = heap_malloc(allocator, key_size);
    assert(key_copy);
    u8 *val_copy = heap_malloc(allocator, val_size);
    assert(val_copy);

    memory_copy(key_copy, key, key_size);
    memory_copy(val_copy, val, val_size);

    bucket->next = nullptr;
    bucket->pair.key = key_copy;
    bucket->pair.val = val_copy;
}

local_fn void table_bucket_put_val(TableBucket *bucket, const void *val, usize val_size) {
    assert(bucket);
    assert(bucket->pair.val);
    assert(val);
    assert(val_size);

    memory_copy(bucket->pair.val, val, val_size);
}

local_fn void table_bucket_destroy(TableBucket *bucket, HeapAllocator *allocator) {
    assert(bucket);
    assert(allocator);

    if (bucket->pair.key) {
        heap_free(allocator, bucket->pair.key);
    }
    if (bucket->pair.val) {
        heap_free(allocator, bucket->pair.val);
    }

    bucket->pair.key = nullptr;
    bucket->pair.val = nullptr;
    bucket->next = nullptr;
}

local_fn void table_rehash(Table *tb, usize new_count) {
    assert(tb);

    if (new_count == 0) {
        if (tb->slots) {
            heap_free(tb->allocator, tb->slots);
        }
        tb->slots = nullptr;
        tb->slot_count = 0;
        return;
    }

    TableBucket **old_slots = tb->slots;
    usize old_count = tb->slot_count;

    tb->slots = heap_calloc(tb->allocator, new_count, sizeof(TableBucket *));
    assert(tb->slots);
    tb->slot_count = new_count;

    for (usize i = 0; i < old_count; ++i) {
        TableBucket *bucket = old_slots[i];
        while (bucket) {
            TableBucket *next = bucket->next;
            usize hash = tb->hash_func(bucket->pair.key) % new_count;
            bucket->next = tb->slots[hash];
            tb->slots[hash] = bucket;
            bucket = next;
        }
    }

    if (old_slots) {
        heap_free(tb->allocator, old_slots);
    }
}

local_fn TableBucket *table_lookup(const Table *tb, const void *key) {
    assert(tb);
    assert(key);

    if (table_empty(tb)) {
        return nullptr;
    }

    usize hash = tb->hash_func(key) % tb->slot_count;
    TableBucket *bucket = tb->slots[hash];
    while(bucket) {
        bool match = tb->compare_func(bucket->pair.key, key) == 0;
        if (match) {
            break;
        }
        bucket = bucket->next;
    }
    return bucket;
}

Table *table_create(usize key_size,
                    usize val_size,
                    HashFunc hash_func,
                    CompareFunc compare_func,
                    HeapAllocator *allocator) {
    assert(key_size);
    assert(val_size);
    assert(hash_func);
    assert(compare_func);
    if (!allocator)
        allocator = stdalloc;

    Table *dest = heap_malloc(allocator, sizeof(Table));
    assert(dest);

    dest->size = 0;
    dest->slot_count = 0;
    dest->slots = nullptr;
    dest->free_buckets = nullptr;
    dest->key_size = key_size;
    dest->val_size = val_size;
    dest->hash_func = hash_func;
    dest->compare_func = compare_func;
    dest->allocator = allocator;
    return dest;
}

void table_destroy(Table *tb) {
    assert(tb);

    table_clear(tb);
    table_shrink(tb);

    tb->key_size = 0;
    tb->val_size = 0;
    tb->hash_func = nullptr;
    tb->compare_func = nullptr;

    heap_free(tb->allocator, tb);
}

void table_put(Table *tb, const void *key, const void *val) {
    assert(tb);
    assert(key);
    assert(val);

    f64 load;
    if (table_empty(tb)) {
        load = 1.0;
    } else {
        load = (f64)tb->size / (f64)tb->slot_count;
    }

    if (load > 0.75) {
        table_rehash(tb, tb->slot_count * 4 + 16);
    }

    TableBucket *bucket = table_lookup(tb, key);

    if (bucket) {
        table_bucket_put_val(bucket, val, tb->val_size);
    } else {
        usize hash = tb->hash_func(key) % tb->slot_count;
        TableBucket *new_bucket = heap_malloc(tb->allocator, sizeof(TableBucket));
        assert(new_bucket);
        table_bucket_create(new_bucket, key, tb->key_size, val, tb->val_size, tb->allocator);
        new_bucket->next = tb->slots[hash];
        tb->slots[hash] = new_bucket;
        tb->size++;
    }
}

void *table_get(Table *tb, const void *key) {
    assert(tb);
    assert(key);
    return table_lookup(tb, key)->pair.val;
}

void table_remove(Table *tb, const void *key) {
    assert(tb);
    assert(key);

    TableBucket *bucket = table_lookup(tb, key);
    if (!bucket) {
        return;
    }

    usize hash = tb->hash_func(key) % tb->slot_count;
    TableBucket *first_bucket = tb->slots[hash];

    TablePair temp = first_bucket->pair;
    first_bucket->pair = bucket->pair;
    bucket->pair = temp;

    tb->slots[hash] = first_bucket->next;
    table_bucket_destroy(first_bucket, tb->allocator);

    first_bucket->next = tb->free_buckets;
    tb->free_buckets = first_bucket;
}

void table_clear(Table *tb) {
    assert(tb);
    if (tb->size == 0) {
        return;
    }

    for (usize i = 0; i < tb->slot_count; ++i) {
        TableBucket *bucket = tb->slots[i];
        while (bucket) {
            TableBucket *next = bucket->next;
            tb->slots[i] = next;

            table_bucket_destroy(bucket, tb->allocator);

            bucket->next = tb->free_buckets;
            tb->free_buckets = bucket;

            bucket = next;
        }
    }

    tb->size = 0;
}

void table_shrink(Table *tb) {
    assert(tb);

    table_rehash(tb, (usize)((f64)tb->size * 1.25));

    while (tb->free_buckets) {
        TableBucket *to_free = tb->free_buckets;
        tb->free_buckets = to_free->next;
        heap_free(tb->allocator, to_free);
    }
}

usize table_size(const Table *tb) {
    assert(tb);
    return tb->size;
}

bool table_empty(const Table *tb) {
    assert(tb);
    return tb->size == 0 || tb->slot_count == 0;
}

bool table_contains(const Table *tb, const void *key) {
    assert(tb);
    assert(key);
    return table_lookup(tb, key) != nullptr;
}

TableItr table_itr(Table *tb) {
    assert(tb);

    TableItr itr;
    itr.table = tb;
    itr.slot = 0;
    itr.bucket = nullptr;

    return table_itr_next(itr);
}

TableItr table_itr_next(TableItr itr) {
    assert(itr.table);
    if (table_empty(itr.table)) {
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
    assert(itr.table);
    return itr.slot >= itr.table->slot_count;
}

TablePair *table_itr_get(TableItr itr) {
    assert(itr.table);
    assert(itr.bucket);
    return &(itr.bucket->pair);
}
