//
// Created by scott on 9/19/2023.
//

#include "shmo/set.h"

struct SetBucket {
    void *elm;
    struct SetBucket *next;
};

struct Set {
    usize size;
    usize slot_count;

    SetBucket **slots;
    SetBucket *free_buckets;

    usize elm_size;
    HashFunc hash_func;
    CompareFunc compare_func;
    HeapAllocator *allocator;
};

local_fn void set_bucket_create(SetBucket *bucket,
                                const void *elm,
                                usize elm_size,
                                HeapAllocator *allocator) {
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

local_fn void set_bucket_destroy(SetBucket *bucket, HeapAllocator *allocator) {
    assert(bucket);
    assert(allocator);

    if (bucket->elm) {
        heap_free(allocator, bucket->elm);
    }

    bucket->elm = nullptr;
    bucket->next = nullptr;
}

local_fn void set_rehash(Set *s, usize new_count) {
    assert(s);

    if (new_count == 0) {
        if (s->slots) {
            heap_free(s->allocator, s->slots);
        }
        s->slots = nullptr;
        s->slot_count = 0;
        return;
    }

    SetBucket **old_slots = s->slots;
    usize old_count = s->slot_count;

    s->slots = heap_calloc(s->allocator, new_count, sizeof(SetBucket *));
    assert(s->slots);
    s->slot_count = new_count;

    for (usize i = 0; i < old_count; ++i) {
        SetBucket *bucket = old_slots[i];
        while (bucket) {
            SetBucket *next = bucket->next;
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

local_fn SetBucket *set_lookup(const Set *s, const void *elm) {
    assert(s);
    assert(elm);

    if (set_empty(s)) {
        return nullptr;
    }

    usize hash = s->hash_func(elm) % s->slot_count;
    SetBucket *bucket = s->slots[hash];
    while(bucket) {
        bool match = s->compare_func(bucket->elm, elm) == 0;
        if (match) {
            break;
        }
        bucket = bucket->next;
    }
    return bucket;
}

Set *set_create(usize element_size,
                HashFunc hash_func,
                CompareFunc compare_func,
                HeapAllocator *allocator) {
    assert(element_size);
    assert(hash_func);
    assert(compare_func);
    if (!allocator)
        allocator = stdalloc;

    Set *dest = heap_malloc(allocator, sizeof(Set));
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

void set_destroy(Set *s) {
    assert(s);

    set_clear(s);
    set_shrink(s);

    s->elm_size = 0;
    s->hash_func = nullptr;
    s->compare_func = nullptr;

    heap_free(s->allocator, s);
}

void set_put(Set *s, const void *elm) {
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

    SetBucket *bucket = set_lookup(s, elm);

    if (bucket) {
        return;
    } else {
        usize hash = s->hash_func(elm) % s->slot_count;
        SetBucket *new_bucket = heap_malloc(s->allocator, sizeof(SetBucket));
        assert(new_bucket);
        set_bucket_create(new_bucket, elm, s->elm_size, s->allocator);
        new_bucket->next = s->slots[hash];
        s->slots[hash] = new_bucket;
        s->size++;
    }
}

const void *set_get(const Set *s, const void *elm) {
    assert(s);
    assert(elm);

    SetBucket *bucket = set_lookup(s, elm);
    if (bucket) {
        return bucket->elm;
    }

    return nullptr;
}

void set_remove(Set *s, const void *elm) {
    assert(s);
    assert(elm);

    SetBucket *bucket = set_lookup(s, elm);
    if (!bucket) {
        return;
    }

    usize hash = s->hash_func(elm) % s->slot_count;
    SetBucket *first_bucket = s->slots[hash];

    void *temp = first_bucket->elm;
    first_bucket->elm = bucket->elm;
    bucket->elm = temp;

    s->slots[hash] = first_bucket->next;
    set_bucket_destroy(first_bucket, s->allocator);

    first_bucket->next = s->free_buckets;
    s->free_buckets = first_bucket;
}

void set_clear(Set *s) {
    assert(s);
    if (s->size == 0) {
        return;
    }

    for (usize i = 0; i < s->slot_count; ++i) {
        SetBucket *bucket = s->slots[i];
        while (bucket) {
            SetBucket *next = bucket->next;
            s->slots[i] = next;

            set_bucket_destroy(bucket, s->allocator);

            bucket->next = s->free_buckets;
            s->free_buckets = bucket;

            bucket = next;
        }
    }

    s->size = 0;
}

void set_shrink(Set *s) {
    assert(s);

    set_rehash(s, (usize)((f64)s->size * 1.25));

    while (s->free_buckets) {
        SetBucket *to_free = s->free_buckets;
        s->free_buckets = to_free->next;
        heap_free(s->allocator, to_free);
    }
}

usize set_size(const Set *s) {
    assert(s);
    return s->size;
}

bool set_empty(const Set *s) {
    assert(s);
    return s->size == 0 || s->slot_count == 0;
}

bool set_contains(const Set *s, const void *elm) {
    assert(s);
    assert(elm);
    return set_lookup(s, elm) != nullptr;
}

SetItr set_itr(Set *s) {
    assert(s);

    SetItr itr;
    itr.set = s;
    itr.slot = 0;
    itr.bucket = nullptr;

    return set_itr_next(itr);
}

SetItr set_itr_next(SetItr itr) {
    assert(itr.set);
    if (set_empty(itr.set)) {
        return itr;
    }

    usize slot = itr.slot;
    SetBucket *bucket;

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

bool set_itr_end(SetItr itr) {
    assert(itr.set);
    return itr.slot >= itr.set->slot_count;
}

void *set_itr_get(SetItr itr) {
    assert(itr.set);
    assert(itr.bucket);
    return &(itr.bucket->elm);
}
