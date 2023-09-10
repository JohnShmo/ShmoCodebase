//
// Created by scott on 9/9/2023.
//

#include "shmo/table.h"
#include "shmo/darray.h"

struct table_bucket_t {
    darray_t pairs;
};

local_fn void table_free_bucket(table_t *tb, table_bucket_t *b) {
    for (size_t i = 0; i < darray_size(&b->pairs); ++i) {
        table_pair_t *pair = darray_at(&b->pairs, i);
        heap_free(tb->allocator, pair->key.data);
        heap_free(tb->allocator, pair->val.data);
    }
    darray_destroy(&b->pairs);
}

local_fn u64 table_hash(table_t *tb, bytes_t key) {
    assert(tb);
    assert(key.data);
    assert(key.size);
    return tb->hash_func(key);
}

local_fn f64 table_load_factor(table_t *tb) {
    assert(tb);
    if (tb->bucket_count == 0)
        return 1.0;
    return (f64)tb->size / (f64)tb->bucket_count;
}

local_fn void table_set_impl(table_t *tb, bytes_t key, bytes_t val) {
    assert(tb);
    assert(key.data);
    assert(key.size);
    assert(val.data);
    assert(val.size);

    u64 hash = table_hash(tb, key);
    table_bucket_t *bucket = &tb->buckets[(size_t)hash % tb->bucket_count];

    table_pair_t *free_pair = nullptr;
    table_pair_t *match_pair = nullptr;

    for (size_t i = 0; i < darray_size(&bucket->pairs); ++i) {
        table_pair_t *pair = darray_at(&bucket->pairs, i);
        if (!pair->key.data && !free_pair) {
            free_pair = pair;
        }
        bool match = tb->compare_func(pair->key, key);
        if (match) {
            match_pair = pair;
            break;
        }
    }

    table_pair_t *final_pair;
    if (match_pair) {
        final_pair = match_pair;
        heap_free(tb->allocator, final_pair->val.data);
    } else {
        if (free_pair) {
            final_pair = free_pair;
        } else {
            darray_pushb(&bucket->pairs, nullptr);
            final_pair = darray_back(&bucket->pairs);
        }
        final_pair->key.data = heap_malloc(tb->allocator, key.size);
        assert(final_pair->key.data);
        memory_copy(final_pair->key.data, key.data, key.size);
        final_pair->key.size = key.size;
        tb->size += 1;
    }

    final_pair->val.data = heap_malloc(tb->allocator, val.size);
    assert(final_pair->val.data);
    memory_copy(final_pair->val.data, val.data, val.size);
    final_pair->val.size = val.size;
}

local_fn void table_grow(table_t *tb) {
    assert(tb);

    if (tb->bucket_count == 0) {
        tb->bucket_count = 16;
        tb->buckets = heap_calloc(tb->allocator, tb->bucket_count, sizeof(table_bucket_t));
        assert(tb->buckets);
        for (size_t i = 0; i < tb->bucket_count; ++i) {
            darray_create(&tb->buckets[i].pairs, sizeof(table_pair_t), tb->allocator);
        }
        return;
    }
    size_t old_count = tb->bucket_count;
    table_bucket_t *old_buckets = tb->buckets;

    tb->bucket_count = tb->bucket_count * 2;
    tb->buckets = heap_calloc(tb->allocator, tb->bucket_count, sizeof(table_bucket_t));
    tb->size = 0;
    assert(tb->buckets);
    for (size_t i = 0; i < tb->bucket_count; ++i) {
        darray_create(&tb->buckets[i].pairs, sizeof(table_pair_t), tb->allocator);
    }

    for (size_t i = 0; i < old_count; ++i) {
        table_bucket_t *bucket = &old_buckets[i];
        for (size_t j = 0; j < darray_size(&bucket->pairs); ++j) {
            table_pair_t *pair = (table_pair_t *)darray_at(&bucket->pairs, j);
            if (pair->key.data) {
                table_set_impl(tb, pair->key, pair->val);
            }
        }
        table_free_bucket(tb, bucket);
    }
    heap_free(tb->allocator, old_buckets);
}

void table_create(table_t *tb, hash_func_t hash_func, compare_func_t compare_func, heap_allocator_t *allocator) {
    assert(tb);

    if (hash_func) {
        tb->hash_func = hash_func;
    } else {
        tb->hash_func = hash_default;
    }

    if (compare_func) {
        tb->compare_func = compare_func;
    } else {
        tb->compare_func = compare_default;
    }

    if (allocator) {
        tb->allocator = allocator;
    } else {
        tb->allocator = stdalloc;
    }

    tb->buckets = nullptr;
    tb->bucket_count = 0;
}

void table_destroy(table_t *tb) {
    assert(tb);

    if (tb->buckets) {
        for (size_t i = 0; i < tb->bucket_count; ++i) {
            table_free_bucket(tb, &tb->buckets[i]);
        }
        heap_free(tb->allocator, tb->buckets);
    }

    tb->size = 0;
    tb->bucket_count = 0;
    tb->buckets = nullptr;
    tb->hash_func = nullptr;
    tb->compare_func = nullptr;
    tb->allocator = nullptr;
}


void table_set(table_t *tb, bytes_t key, bytes_t val) {
    assert(tb);

    f64 load = table_load_factor(tb);
    if (load > 0.75) {
        table_grow(tb);
    }

    table_set_impl(tb, key, val);
}

bytes_t table_get(table_t *tb, bytes_t key) {
    assert(tb);
    assert(key.data);
    assert(key.size);

    bytes_t result = { 0 };

    u64 hash = table_hash(tb, key);
    table_bucket_t *bucket = &tb->buckets[(size_t)hash % tb->bucket_count];

    table_pair_t *match_pair = nullptr;
    for (size_t i = 0; i < darray_size(&bucket->pairs); ++i) {
        table_pair_t *pair = darray_at(&bucket->pairs, i);
        bool match = tb->compare_func(pair->key, key);
        if (match) {
            match_pair = pair;
            break;
        }
    }

    if (match_pair) {
        result.data = match_pair->val.data;
        result.size = match_pair->val.size;
    }

    return result;
}

void table_remove(table_t *tb, bytes_t key) {
    assert(tb);
    assert(key.data);
    assert(key.size);

    if (tb->size == 0) {
        return;
    }

    u64 hash = table_hash(tb, key);
    table_bucket_t *bucket = &tb->buckets[(size_t)hash % tb->bucket_count];

    table_pair_t *match_pair = nullptr;
    for (size_t i = 0; i < darray_size(&bucket->pairs); ++i) {
        table_pair_t *pair = darray_at(&bucket->pairs, i);
        bool match = tb->compare_func(pair->key, key);
        if (match) {
            match_pair = pair;
            break;
        }
    }

    if (match_pair) {
        heap_free(tb->allocator, match_pair->key.data);
        heap_free(tb->allocator, match_pair->val.data);
        match_pair->key.data = nullptr;
        match_pair->key.size = 0;
        match_pair->val.data = nullptr;
        match_pair->val.size = 0;

        tb->size -= 1;
    }
}