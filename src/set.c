//
// Created by scott on 9/19/2023.
//

#include "shmo/set.h"

struct set_bucket_t {
    void *elm;
    struct set_bucket_t *next;
};

struct set_t {
    usize size;
    usize slot_count;

    set_bucket_t **slots;
    set_bucket_t *free_buckets;

    usize elm_size;
    hash_func_t hash_func;
    compare_func_t compare_func;
    heap_allocator_t *allocator;
};

local_fn void set_bucket_create(set_bucket_t *bucket,
                                  const void *elm,
                                  usize elm_size,
                                  heap_allocator_t *allocator) {
    assert(bucket);
    assert(elm);
    assert(elm_size);
    assert(allocator);

    u8 *elm_copy = heap_malloc(allocator, elm_size);
    assert(elm_copy);

    memory_copy(elm_copy, elm, elm_size);

    bucket->next = nullptr;
    bucket->elm = elm_copy;
}

local_fn void set_bucket_destroy(set_bucket_t *bucket, heap_allocator_t *allocator) {
    assert(bucket);
    assert(allocator);

    if (bucket->elm) {
        heap_free(allocator, bucket->elm);
    }

    bucket->elm = nullptr;
    bucket->next = nullptr;
}

local_fn void set_rehash(set_t *s, usize new_count) {
    assert(s);

    if (new_count == 0) {
        if (s->slots) {
            heap_free(s->allocator, s->slots);
        }
        s->slots = nullptr;
        s->slot_count = 0;
        return;
    }

    set_bucket_t **old_slots = s->slots;
    usize old_count = s->slot_count;

    s->slots = heap_calloc(s->allocator, new_count, sizeof(set_bucket_t *));
    assert(s->slots);
    s->slot_count = new_count;

    for (usize i = 0; i < old_count; ++i) {
        set_bucket_t *bucket = old_slots[i];
        while (bucket) {
            set_bucket_t *next = bucket->next;
            usize hash = s->hash_func(bucket->elm) % new_count;
            bucket->next = s->slots[hash];
            s->slots[hash] = bucket;
            bucket = next;
        }
    }

    if (old_slots) {
        heap_free(s->allocator, old_slots);
    }
}

local_fn set_bucket_t *set_lookup(const set_t *s, const void *elm) {
    assert(s);
    assert(elm);

    if (set_empty(s)) {
        return nullptr;
    }

    usize hash = s->hash_func(elm) % s->slot_count;
    set_bucket_t *bucket = s->slots[hash];
    while(bucket) {
        bool match = s->compare_func(bucket->elm, elm) == 0;
        if (match) {
            break;
        }
        bucket = bucket->next;
    }
    return bucket;
}

set_t *set_create(usize element_size,
                  hash_func_t hash_func,
                  compare_func_t compare_func,
                  heap_allocator_t *allocator) {
    assert(element_size);
    assert(hash_func);
    assert(compare_func);
    if (!allocator)
        allocator = stdalloc;

    set_t *dest = heap_malloc(allocator, sizeof(set_t));
    assert(dest);

    dest->size = 0;
    dest->slot_count = 0;
    dest->slots = nullptr;
    dest->free_buckets = nullptr;
    dest->elm_size = element_size;
    dest->hash_func = hash_func;
    dest->compare_func = compare_func;
    dest->allocator = allocator;
    return dest;
}

void set_destroy(set_t *s) {
    assert(s);

    set_clear(s);
    set_shrink(s);

    s->elm_size = 0;
    s->hash_func = nullptr;
    s->compare_func = nullptr;

    heap_free(s->allocator, s);
}

void set_put(set_t *s, const void *elm) {
    assert(s);
    assert(elm);

    f64 load;
    if (set_empty(s)) {
        load = 1.0;
    } else {
        load = (f64)s->size / (f64)s->slot_count;
    }

    if (load > 0.75) {
        set_rehash(s, s->slot_count * 4 + 16);
    }

    set_bucket_t *bucket = set_lookup(s, elm);

    if (bucket) {
        return;
    } else {
        usize hash = s->hash_func(elm) % s->slot_count;
        set_bucket_t *new_bucket = heap_malloc(s->allocator, sizeof(set_bucket_t));
        assert(new_bucket);
        set_bucket_create(new_bucket, elm, s->elm_size, s->allocator);
        new_bucket->next = s->slots[hash];
        s->slots[hash] = new_bucket;
        s->size++;
    }
}

void set_remove(set_t *s, const void *elm) {
    assert(s);
    assert(elm);

    set_bucket_t *bucket = set_lookup(s, elm);
    if (!bucket) {
        return;
    }

    usize hash = s->hash_func(elm) % s->slot_count;
    set_bucket_t *first_bucket = s->slots[hash];

    void *temp = first_bucket->elm;
    first_bucket->elm = bucket->elm;
    bucket->elm = temp;

    s->slots[hash] = first_bucket->next;
    set_bucket_destroy(first_bucket, s->allocator);

    first_bucket->next = s->free_buckets;
    s->free_buckets = first_bucket;
}

void set_clear(set_t *s) {
    assert(s);
    if (s->size == 0) {
        return;
    }

    for (usize i = 0; i < s->slot_count; ++i) {
        set_bucket_t *bucket = s->slots[i];
        while (bucket) {
            set_bucket_t *next = bucket->next;
            s->slots[i] = next;

            set_bucket_destroy(bucket, s->allocator);

            bucket->next = s->free_buckets;
            s->free_buckets = bucket;

            bucket = next;
        }
    }

    s->size = 0;
}

void set_shrink(set_t *s) {
    assert(s);

    set_rehash(s, (usize)((f64)s->size * 1.25));

    while (s->free_buckets) {
        set_bucket_t *to_free = s->free_buckets;
        s->free_buckets = to_free->next;
        heap_free(s->allocator, to_free);
    }
}

usize set_size(const set_t *s) {
    assert(s);
    return s->size;
}

bool set_empty(const set_t *s) {
    assert(s);
    return s->size == 0 || s->slot_count == 0;
}

bool set_contains(const set_t *s, const void *elm) {
    assert(s);
    assert(elm);
    return set_lookup(s, elm) != nullptr;
}

set_itr_t set_itr(set_t *s) {
    assert(s);

    set_itr_t itr;
    itr.set = s;
    itr.slot = 0;
    itr.bucket = nullptr;

    return set_itr_next(itr);
}

set_itr_t set_itr_next(set_itr_t itr) {
    assert(itr.set);
    if (set_empty(itr.set)) {
        return itr;
    }

    usize slot = itr.slot;
    set_bucket_t *bucket;

    if (itr.bucket) {
        bucket = itr.bucket->next;
    } else {
        bucket = itr.set->slots[slot];
    }

    while (!bucket) {
        slot++;
        if (slot >= itr.set->slot_count) {
            break;
        }
        bucket = itr.set->slots[slot];
    }

    itr.slot = slot;
    itr.bucket = bucket;

    return itr;
}

bool set_itr_end(set_itr_t itr) {
    assert(itr.set);
    return itr.slot >= itr.set->slot_count;
}

void *set_itr_get(set_itr_t itr) {
    assert(itr.set);
    assert(itr.bucket);
    return &(itr.bucket->elm);
}