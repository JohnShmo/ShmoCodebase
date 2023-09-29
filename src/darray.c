//
// Created by scott on 9/9/2023.
//

#include "shmo/darray.h"

struct Darray {
    usize size;
    usize capacity;
    usize element_size;
    u8 *data;
    Allocator *allocator;
};

local_fn bool darray_reserve_if_full(Darray *da) {
    if (da->size == da->capacity) {
        return darray_reserve(da, da->capacity * 4 + 1);
    }
    return true;
}

Darray *darray_create(usize element_size, Allocator *allocator) {
    if (element_size == 0)
        return nullptr;
    if (!allocator)
        allocator = stdalloc;

    Darray *dest = allocator_malloc(allocator, sizeof(Darray));
    if (!dest)
        return nullptr;

    dest->size = 0;
    dest->capacity = 0;
    dest->element_size = element_size;
    dest->data = nullptr;
    dest->allocator = allocator;

    return dest;
}

void darray_destroy(Darray *da) {
    if (!da)
        return;

    allocator_free(da->allocator, da->data);
    da->size = 0;
    da->capacity = 0;
    da->element_size = 0;
    da->data = nullptr;

    allocator_free(da->allocator, da);
}

bool darray_reserve(Darray *da, usize n) {
    assert(da);

    if (n <= da->capacity) {
        return true;
    }

    void *new_data = allocator_realloc(da->allocator, da->data, n * da->element_size);
    if (!new_data)
        return false;

    da->data = new_data;
    da->capacity = n;

    return true;
}

bool darray_resize(Darray *da, usize n, Bytes fillval) {
    assert(da);

    if (!bytes_is_null(fillval) && fillval.size != da->element_size)
        return false;

    if (!darray_reserve(da, n))
        return false;

    if (n > da->size) {
        usize count = n - da->size;
        if (!bytes_is_null(fillval)) {
            for (usize i = 0; i < count; ++i) {
                usize index = da->size + i;
                index *= da->element_size;
                memory_copy((da->data) + index, fillval.p, da->element_size);
            }
        } else {
            memory_zero((da->data) + (da->size * da->element_size), count * da->element_size);
        }
    }

    da->size = n;

    return true;
}

bool darray_shrink(Darray *da) {
    assert(da);

    if (da->size == da->capacity)
        return true;

    void *old_data = da->data;
    void *new_data = allocator_malloc(da->allocator, da->size * da->element_size);
    if (!new_data)
        return false;

    da->data = new_data;
    da->capacity = da->size;

    if (old_data) {
        memory_copy(da->data, old_data, da->size * da->element_size);
        allocator_free(da->allocator, old_data);
    }

    return true;
}

void darray_clear(Darray *da) {
    assert(da);
    da->size = 0;
}

bool darray_pushb(Darray *da, Bytes val) {
    assert(da);
    if (bytes_is_null(val))
        return false;

    if (val.size != da->element_size)
        return false;

    if (!darray_reserve_if_full(da))
        return false;

    usize index = da->size * da->element_size;
    memory_copy((da->data) + index, val.p, da->element_size);

    da->size += 1;
    return true;
}

bool darray_popb(Darray *da) {
    assert(da);
    if (da->size == 0)
        return false;

    da->size -= 1;
    return true;
}

bool darray_pushf(Darray *da, Bytes val) {
    assert(da);
    if (bytes_is_null(val))
        return false;

    if (val.size != da->element_size)
        return false;

    if (!darray_reserve_if_full(da))
        return false;

    if (da->size > 0)
        memory_copy(((u8 *)da->data) + da->element_size, da->data, da->size * da->element_size);

    memory_copy((da->data), val.p, da->element_size);

    da->size += 1;
    return true;
}

bool darray_popf(Darray *da) {
    assert(da);
    if (da->size == 0)
        return false;

    if (da->size > 1) {
        memory_copy(da->data, (da->data) + da->element_size, (da->size - 1) * da->element_size);
    }
    da->size -= 1;
    return true;
}

bool darray_insert(Darray *da, usize index, Bytes val) {
    assert(da);
    if (index > da->size || bytes_is_null(val))
        return false;

    if (index == da->size)
        return darray_pushb(da, val);

    if (index == 0)
        return darray_pushf(da, val);

    if (val.size != da->element_size)
        return false;

    if (!darray_reserve_if_full(da))
        return false;

    index = index * da->element_size;
    if (da->size > 0) {
        memory_copy((da->data) + index + da->element_size, da->data + index, (da->size * da->element_size) - index);
    }

    memory_copy((da->data) + index, val.p, da->element_size);
    da->size += 1;
    return true;
}

bool darray_remove(Darray *da, usize index) {
    assert(da);
    if (index >= da->size)
        return false;

    if (index == da->size - 1)
        return darray_popb(da);

    if (index == 0)
        return darray_popf(da);

    index = index * da->element_size;
    memory_copy((da->data) + index, (da->data) + index + da->element_size, (da->size - 1) * da->element_size - index);
    da->size -= 1;

    return true;
}

Bytes darray_back(const Darray *da) {
    assert(da);
    if (da->size == 0)
        return nullbytes;

    usize index = (da->size - 1) * da->element_size;
    return (Bytes) { .p = (da->data) + index, .size = da->element_size };
}

Bytes darray_front(const Darray *da) {
    assert(da);
    if (da->size == 0)
        return nullbytes;

    return (Bytes) { .p = da->data, .size = da->element_size };
}

Bytes darray_at(const Darray *da, usize index) {
    assert(da);
    if (da->size == 0 || index >= da->size)
        return nullbytes;

    index = index * da->element_size;
    return (Bytes) { .p = (da->data) + index, .size = da->element_size };
}

bool darray_setb(Darray *da, Bytes val) {
    assert(da);
    return darray_set(da, da->size - 1, val);
}

bool darray_setf(Darray *da, Bytes val) {
    assert(da);
    return darray_set(da, 0, val);
}

bool darray_set(Darray *da, usize index, Bytes val) {
    assert(da);
    if (da->size == 0 || index >= da->size)
        return false;

    if (val.size != da->element_size)
        return false;

    index = index * da->element_size;
    memory_copy((da->data) + index, val.p, da->element_size);
    return true;
}

bool darray_empty(const Darray *da) {
    assert(da);
    return da->size == 0;
}

usize darray_size(const Darray *da) {
    assert(da);
    return da->size;
}

usize darray_elm_size(const Darray *da) {
    assert(da);
    return da->element_size;
}

usize darray_capacity(const Darray *da) {
    assert(da);
    return da->capacity;
}

void *darray_data(Darray *da) {
    assert(da);
    return da->data;
}

const void *darray_const_data(const Darray *da) {
    assert(da);
    return da->data;
}
