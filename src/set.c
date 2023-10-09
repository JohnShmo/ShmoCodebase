//
// Created by scott on 9/19/2023.
//

#include "shmo/set.h"

struct SetBucket {
    byte *elm;
    usize elm_size;
    struct SetBucket *next;
};

struct Set {
    usize size;
    usize slot_count;

    SetBucket **slots;
    SetBucket *free_buckets;

    Allocator *allocator;
};

local_fn bool set_bucket_create(SetBucket *bucket,
                                const void *elm,
                                usize elm_size,
                                Allocator *allocator) {
    assert(bucket);
    assert(elm);
    assert(elm_size);
    assert(allocator);

    byte *elm_copy = allocator_malloc(allocator, elm_size);
    if (!elm_copy) {
        return false;
    }

    memory_copy(elm_copy, elm, elm_size);

    bucket->next = nullptr;
    bucket->elm = elm_copy;
    bucket->elm_size = elm_size;

    return true;
}

local_fn void set_bucket_destroy(SetBucket *bucket, Allocator *allocator) {
    assert(bucket);
    assert(allocator);

    allocator_free(allocator, bucket->elm);

    bucket->elm = nullptr;
    bucket->next = nullptr;
}

local_fn bool set_rehash(Set *s, usize new_count) {
    assert(s);

    if (new_count == 0) {
        allocator_free(s->allocator, s->slots);
        s->slots = nullptr;
        s->slot_count = 0;
        return true;
    }

    SetBucket **old_slots = s->slots;
    usize old_count = s->slot_count;

    SetBucket **new_slots = allocator_calloc(s->allocator, new_count, sizeof(SetBucket *));
    if (!new_slots)
        return false;
    s->slot_count = new_count;
    s->slots = new_slots;

    for (usize i = 0; i < old_count; ++i) {
        SetBucket *bucket = old_slots[i];
        while (bucket) {
            SetBucket *next = bucket->next;
            usize hash = hash_bytes((Bytes) { .p = bucket->elm, .size = bucket->elm_size }) % new_count;
            bucket->next = s->slots[hash];
            s->slots[hash] = bucket;
            bucket = next;
        }
    }

    if (old_slots) {
        allocator_free(s->allocator, old_slots);
    }
    return true;
}

local_fn SetBucket *set_lookup(const Set *s, Bytes elm) {
    assert(s);
    assert(elm.p);

    if (set_empty(s)) {
        return nullptr;
    }

    usize hash = hash_bytes(elm) % s->slot_count;
    SetBucket *bucket = s->slots[hash];
    while(bucket) {
        bool match = compare_bytes((Bytes) { .p = bucket->elm, .size = bucket->elm_size }, elm) == 0;
        if (match) {
            break;
        }
        bucket = bucket->next;
    }
    return bucket;
}

Set *set_create(Allocator *allocator) {
    if (!allocator)
        allocator = stdalloc;

    Set *dest = allocator_malloc(allocator, sizeof(Set));
    if (!dest) {
        return nullptr;
    }

    dest->size = 0;
    dest->slot_count = 0;
    dest->slots = nullptr;
    dest->free_buckets = nullptr;
    dest->allocator = allocator;
    return dest;
}

void set_destroy(Set *s) {
    if (!s) {
        return;
    }

    set_clear(s);
    set_shrink(s);

    allocator_free(s->allocator, s);
}

bool set_put(Set *s, Bytes elm) {
    assert(s);

    if (bytes_is_null(elm))
        return false;

    f64 load;
    if (set_empty(s)) {
        load = 1.0;
    } else {
        load = (f64)s->size / (f64)s->slot_count;
    }

    if (load > 0.75) {
        if (!set_rehash(s, s->slot_count * 4 + 16))
            return false;
    }

    SetBucket *bucket = set_lookup(s, elm);

    if (bucket) {
        return true;
    } else {
        usize hash = hash_bytes(elm) % s->slot_count;
        SetBucket *new_bucket = allocator_malloc(s->allocator, sizeof(SetBucket));
        if (!new_bucket)
            return false;
        set_bucket_create(new_bucket, elm.p, elm.size, s->allocator);
        new_bucket->next = s->slots[hash];
        s->slots[hash] = new_bucket;
        s->size++;
    }

    return true;
}

bool set_remove(Set *s, Bytes elm) {
    assert(s);
    if (bytes_is_null(elm))
        return false;

    SetBucket *bucket = set_lookup(s, elm);
    if (!bucket) {
        return false;
    }

    usize hash = hash_bytes(elm) % s->slot_count;
    SetBucket *first_bucket = s->slots[hash];

    void *temp = first_bucket->elm;
    first_bucket->elm = bucket->elm;
    bucket->elm = temp;

    s->slots[hash] = first_bucket->next;
    set_bucket_destroy(first_bucket, s->allocator);

    first_bucket->next = s->free_buckets;
    s->free_buckets = first_bucket;

    return true;
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

bool set_shrink(Set *s) {
    assert(s);

    if (!set_rehash(s, (usize)((f64)s->size * 1.25))) {
        return false;
    }

    while (s->free_buckets) {
        SetBucket *to_free = s->free_buckets;
        s->free_buckets = to_free->next;
        allocator_free(s->allocator, to_free);
    }

    return true;
}

usize set_size(const Set *s) {
    assert(s);
    return s->size;
}

bool set_empty(const Set *s) {
    assert(s);
    return s->size == 0 || s->slot_count == 0;
}

bool set_contains(const Set *s, Bytes elm) {
    assert(s);
    if (bytes_is_null(elm))
        return false;
    return set_lookup(s, elm) != nullptr;
}

SetItr set_itr(Set *s) {
    SetItr itr;
    itr.set = s;
    itr.slot = 0;
    itr.bucket = nullptr;

    return set_itr_next(itr);
}

SetItr set_itr_next(SetItr itr) {
    if (!itr.set || set_empty(itr.set)) {
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
    return !itr.set || itr.slot >= itr.set->slot_count;
}

Bytes set_itr_get(SetItr itr) {
    if (!itr.set || !itr.bucket)
        return nullbytes;
    return (Bytes) { .p = itr.bucket->elm, .size = itr.bucket->elm_size };
}
