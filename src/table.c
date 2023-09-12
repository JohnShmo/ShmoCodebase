//
// Created by scott on 9/9/2023.
//

#include "shmo/table.h"

local_fn void table_bucket_create(table_bucket_t *bucket,
                                  const void *key,
                                  size_t key_size,
                                  const void *val,
                                  size_t val_size,
                                  heap_allocator_t *allocator) {
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

local_fn void table_bucket_put_val(table_bucket_t *bucket, const void *val, size_t val_size) {
    assert(bucket);
    assert(bucket->pair.val);
    assert(val);
    assert(val_size);

    memory_copy(bucket->pair.val, val, val_size);
}

local_fn void table_bucket_destroy(table_bucket_t *bucket, heap_allocator_t *allocator) {
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

local_fn void table_rehash(table_t *tb, size_t new_count) {
    assert(tb);

    if (new_count == 0) {
        if (tb->slots) {
            heap_free(tb->allocator, tb->slots);
        }
        tb->slots = nullptr;
        tb->slot_count = 0;
        return;
    }

    table_bucket_t **old_slots = tb->slots;
    size_t old_count = tb->slot_count;

    tb->slots = heap_calloc(tb->allocator, new_count, sizeof(table_bucket_t *));
    assert(tb->slots);
    tb->slot_count = new_count;

    for (size_t i = 0; i < old_count; ++i) {
        table_bucket_t *bucket = old_slots[i];
        while (bucket) {
            table_bucket_t *next = bucket->next;
            size_t hash = tb->hash_func(bucket->pair.key) % new_count;
            bucket->next = tb->slots[hash];
            tb->slots[hash] = bucket;
            bucket = next;
        }
    }

    if (old_slots) {
        heap_free(tb->allocator, old_slots);
    }
}

local_fn table_bucket_t *table_lookup(const table_t *tb, const void *key) {
    assert(tb);
    assert(key);

    if (table_empty(tb)) {
        return nullptr;
    }

    size_t hash = tb->hash_func(key) % tb->slot_count;
    table_bucket_t *bucket = tb->slots[hash];
    while(bucket) {
        bool match = tb->compare_func(bucket->pair.key, key) == 0;
        if (match) {
            break;
        }
        bucket = bucket->next;
    }
    return bucket;
}

table_t *table_create(size_t key_size,
                  size_t val_size,
                  hash_func_t hash_func,
                  compare_func_t compare_func,
                  heap_allocator_t *allocator) {
    assert(key_size);
    assert(val_size);
    assert(hash_func);
    assert(compare_func);
    if (!allocator)
        allocator = stdalloc;

    table_t *tb = heap_malloc(allocator, sizeof(table_t));
    assert(tb);

    tb->size = 0;
    tb->slot_count = 0;
    tb->slots = nullptr;
    tb->free_buckets = nullptr;
    tb->key_size = key_size;
    tb->val_size = val_size;
    tb->hash_func = hash_func;
    tb->compare_func = compare_func;
    tb->allocator = allocator;
}

void table_destroy(table_t *tb) {
    assert(tb);

    table_clear(tb);
    table_shrink(tb);

    tb->key_size = 0;
    tb->val_size = 0;
    tb->hash_func = nullptr;
    tb->compare_func = nullptr;

    heap_free(tb->allocator, tb);
}

void table_put(table_t *tb, const void *key, const void *val) {
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

    table_bucket_t *bucket = table_lookup(tb, key);

    if (bucket) {
        table_bucket_put_val(bucket, val, tb->val_size);
    } else {
        size_t hash = tb->hash_func(key) % tb->slot_count;
        table_bucket_t *new_bucket = heap_malloc(tb->allocator, sizeof(table_bucket_t));
        assert(new_bucket);
        table_bucket_create(new_bucket, key, tb->key_size, val, tb->val_size, tb->allocator);
        new_bucket->next = tb->slots[hash];
        tb->slots[hash] = new_bucket;
        tb->size++;
    }
}

void *table_get(table_t *tb, const void *key) {
    assert(tb);
    assert(key);
    return table_lookup(tb, key)->pair.val;
}

void table_remove(table_t *tb, const void *key) {
    assert(tb);
    assert(key);

    table_bucket_t *bucket = table_lookup(tb, key);
    if (!bucket) {
        return;
    }

    size_t hash = tb->hash_func(key) % tb->slot_count;
    table_bucket_t *first_bucket = tb->slots[hash];

    table_pair_t temp = first_bucket->pair;
    first_bucket->pair = bucket->pair;
    bucket->pair = temp;

    tb->slots[hash] = first_bucket->next;
    table_bucket_destroy(first_bucket, tb->allocator);

    first_bucket->next = tb->free_buckets;
    tb->free_buckets = first_bucket;
}

void table_clear(table_t *tb) {
    assert(tb);
    if (tb->size == 0) {
        return;
    }

    for (size_t i = 0; i < tb->slot_count; ++i) {
        table_bucket_t *bucket = tb->slots[i];
        while (bucket) {
            table_bucket_t *next = bucket->next;
            tb->slots[i] = next;

            table_bucket_destroy(bucket, tb->allocator);

            bucket->next = tb->free_buckets;
            tb->free_buckets = bucket;

            bucket = next;
        }
    }

    tb->size = 0;
}

void table_shrink(table_t *tb) {
    assert(tb);

    table_rehash(tb, (size_t)((f64)tb->size * 1.25));

    while (tb->free_buckets) {
        table_bucket_t *to_free = tb->free_buckets;
        tb->free_buckets = to_free->next;
        heap_free(tb->allocator, to_free);
    }
}

size_t table_size(const table_t *tb) {
    assert(tb);
    return tb->size;
}

bool table_empty(const table_t *tb) {
    assert(tb);
    return tb->size == 0 || tb->slot_count == 0;
}

bool table_contains(const table_t *tb, const void *key) {
    assert(tb);
    assert(key);
    return table_lookup(tb, key) != nullptr;
}

table_itr_t table_itr(table_t *tb) {
    assert(tb);

    table_itr_t itr;
    itr.table = tb;
    itr.slot = 0;
    itr.bucket = nullptr;

    return table_itr_next(itr);
}

table_itr_t table_itr_next(table_itr_t itr) {
    assert(itr.table);
    if (table_empty(itr.table)) {
        return itr;
    }

    size_t slot = itr.slot;
    table_bucket_t *bucket;

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

bool table_itr_end(table_itr_t itr) {
    assert(itr.table);
    return itr.slot >= itr.table->slot_count;
}

table_pair_t *table_itr_get(table_itr_t itr) {
    assert(itr.table);
    assert(itr.bucket);
    return &(itr.bucket->pair);
}