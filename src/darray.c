//
// Created by scott on 9/9/2023.
//

#include "shmo/darray.h"

struct Darray {
    usize size;
    usize capacity;
    usize element_size;
    void *data;
    HeapAllocator *allocator;
};

local_fn void darray_reserve_if_full(Darray *da) {
    if (da->size == da->capacity) {
        darray_reserve(da, da->capacity * 4 + 1);
    }
}

Darray *darray_create(usize element_size, HeapAllocator *allocator) {
    assert(element_size);
    if (!allocator)
        allocator = stdalloc;

    Darray *dest = heap_malloc(allocator, sizeof(Darray));
    assert(dest);

    dest->size = 0;
    dest->capacity = 0;
    dest->element_size = element_size;
    dest->data = nullptr;
    dest->allocator = allocator;

    return dest;
}

void darray_destroy(Darray *da) {
    assert(da);

    heap_free(da->allocator, da->data);
    da->size = 0;
    da->capacity = 0;
    da->element_size = 0;
    da->data = nullptr;

    heap_free(da->allocator, da);
}

void darray_reserve(Darray *da, usize n) {
    assert(da);

    if (n <= da->capacity) {
        return;
    }

    void *old_data = da->data;
    da->data = heap_malloc(da->allocator, n * da->element_size);
    da->capacity = n;

    if (old_data) {
        memory_copy(da->data, old_data, da->size * da->element_size);
        heap_free(da->allocator, old_data);
    }
}

void darray_resize(Darray *da, usize n, const void *fillval) {
    assert(da);

    darray_reserve(da, n);

    if (n > da->size) {
        usize count = n - da->size;
        if (fillval) {
            for (usize i = 0; i < count; ++i) {
                usize index = da->size + i;
                index *= da->element_size;
                memory_copy(((u8 *)da->data) + index, fillval, da->element_size);
            }
        } else {
            memory_zero(((u8 *)da->data) + (da->size * da->element_size), count * da->element_size);
        }
    }

    da->size = n;
}

void darray_shrink(Darray *da) {
    assert(da);

    if (da->size == da->capacity) {
        return;
    }

    void *old_data = da->data;
    da->data = heap_malloc(da->allocator, da->size * da->element_size);
    da->capacity = da->size;

    if (old_data) {
        memory_copy(da->data, old_data, da->size * da->element_size);
        heap_free(da->allocator, old_data);
    }
}

void darray_clear(Darray *da) {
    assert(da);
    da->size = 0;
}

void darray_pushb(Darray *da, const void *val) {
    assert(da);

    darray_reserve_if_full(da);

    usize index = da->size * da->element_size;
    if (val) {
        memory_copy(((u8 *)da->data) + index, val, da->element_size);
    } else {
        memory_zero(((u8 *)da->data) + index, da->element_size);
    }
    da->size += 1;
}

void darray_popb(Darray *da) {
    assert(da);
    assert(da->size);

    da->size -= 1;
}

void darray_pushf(Darray *da, const void *val) {
    assert(da);

    darray_reserve_if_full(da);
    if (da->size > 0) {
        memory_copy(((u8 *)da->data) + da->element_size, da->data, da->size * da->element_size);
    }

    if (val) {
        memory_copy(((u8 *)da->data), val, da->element_size);
    } else {
        memory_zero(((u8 *)da->data), da->element_size);
    }
    da->size += 1;
}

void darray_popf(Darray *da) {
    assert(da);
    assert(da->size);

    if (da->size > 1) {
        memory_copy(da->data, ((u8 *) da->data) + da->element_size, (da->size - 1) * da->element_size);
    }
    da->size -= 1;
}

void darray_insert(Darray *da, usize index, const void *val) {
    assert(da);
    assert(index <= da->size);

    if (index == da->size) {
        darray_pushb(da, val);
        return;
    }
    if (index == 0) {
        darray_pushf(da, val);
        return;
    }

    darray_reserve_if_full(da);
    index = index * da->element_size;
    if (da->size > 0) {
        memory_copy(((u8 *)da->data) + index + da->element_size, (u8 *)da->data + index, (da->size * da->element_size) - index);
    }

    if (val) {
        memory_copy(((u8 *)da->data) + index, val, da->element_size);
    } else {
        memory_zero(((u8 *)da->data) + index, da->element_size);
    }
    da->size += 1;
}

void darray_remove(Darray *da, usize index) {
    assert(da);
    assert(index <= da->size);

    if (index == da->size) {
        darray_popb(da);
        return;
    }
    if (index == 0) {
        darray_popf(da);
        return;
    }

    index = index * da->element_size;
    memory_copy(((u8 *)da->data) + index, ((u8 *)da->data) + index + da->element_size, (da->size - 1) * da->element_size - index);
    da->size -= 1;
}

void *darray_back(Darray *da) {
    assert(da);
    assert(da->size);

    usize index = (da->size - 1) * da->element_size;
    return ((u8 *)da->data) + index;
}

void *darray_front(Darray *da) {
    assert(da);
    assert(da->size);

    return da->data;
}

void *darray_at(Darray *da, usize index) {
    assert(da);
    assert(da->size);

    index = index * da->element_size;
    return ((u8 *)da->data) + index;
}

const void *darray_const_at(const Darray *da, usize index) {
    assert(da);
    assert(da->size);

    index = index * da->element_size;
    return ((u8 *)da->data) + index;
}

void darray_set(Darray *da, usize index, const void *val) {
    assert(da);
    assert(da->size);

    index = index * da->element_size;
    if (val) {
        memory_copy(((u8 *)da->data) + index, val, da->element_size);
    } else {
        memory_zero(((u8 *)da->data) + index, da->element_size);
    }
}

bool darray_empty(const Darray *da) {
    assert(da);
    return da->size == 0;
}

usize darray_size(const Darray *da) {
    assert(da);
    return da->size;
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