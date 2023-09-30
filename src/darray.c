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

local_fn bool darray_reserve_if_full(Darray *self) {
    if (self->size == self->capacity) {
        return darray_reserve(self, self->capacity * 4 + 1);
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

void darray_destroy(Darray *self) {
    if (!self)
        return;

    allocator_free(self->allocator, self->data);
    self->size = 0;
    self->capacity = 0;
    self->element_size = 0;
    self->data = nullptr;

    allocator_free(self->allocator, self);
}

bool darray_reserve(Darray *self, usize n) {
    assert(self);

    if (n <= self->capacity) {
        return true;
    }

    void *new_data = allocator_realloc(self->allocator, self->data, n * self->element_size);
    if (!new_data)
        return false;

    self->data = new_data;
    self->capacity = n;

    return true;
}

bool darray_resize(Darray *self, usize n, Bytes fillval) {
    assert(self);

    if (!bytes_is_null(fillval) && fillval.size != self->element_size)
        return false;

    if (!darray_reserve(self, n))
        return false;

    if (n > self->size) {
        usize count = n - self->size;
        if (!bytes_is_null(fillval)) {
            for (usize i = 0; i < count; ++i) {
                usize index = self->size + i;
                index *= self->element_size;
                memory_copy((self->data) + index, fillval.p, self->element_size);
            }
        } else {
            memory_zero((self->data) + (self->size * self->element_size), count * self->element_size);
        }
    }

    self->size = n;

    return true;
}

bool darray_shrink(Darray *self) {
    assert(self);

    if (self->size == self->capacity)
        return true;

    void *old_data = self->data;
    void *new_data = allocator_malloc(self->allocator, self->size * self->element_size);
    if (!new_data)
        return false;

    self->data = new_data;
    self->capacity = self->size;

    if (old_data) {
        memory_copy(self->data, old_data, self->size * self->element_size);
        allocator_free(self->allocator, old_data);
    }

    return true;
}

void darray_clear(Darray *self) {
    assert(self);
    self->size = 0;
}

bool darray_pushb(Darray *self, Bytes val) {
    assert(self);
    if (bytes_is_null(val))
        return false;

    if (val.size != self->element_size)
        return false;

    if (!darray_reserve_if_full(self))
        return false;

    usize index = self->size * self->element_size;
    memory_copy((self->data) + index, val.p, self->element_size);

    self->size += 1;
    return true;
}

bool darray_popb(Darray *self) {
    assert(self);
    if (self->size == 0)
        return false;

    self->size -= 1;
    return true;
}

bool darray_pushf(Darray *self, Bytes val) {
    assert(self);
    if (bytes_is_null(val))
        return false;

    if (val.size != self->element_size)
        return false;

    if (!darray_reserve_if_full(self))
        return false;

    if (self->size > 0)
        memory_copy(((u8 *)self->data) + self->element_size, self->data, self->size * self->element_size);

    memory_copy((self->data), val.p, self->element_size);

    self->size += 1;
    return true;
}

bool darray_popf(Darray *self) {
    assert(self);
    if (self->size == 0)
        return false;

    if (self->size > 1) {
        memory_copy(self->data, (self->data) + self->element_size, (self->size - 1) * self->element_size);
    }
    self->size -= 1;
    return true;
}

bool darray_insert(Darray *self, usize index, Bytes val) {
    assert(self);
    if (index > self->size || bytes_is_null(val))
        return false;

    if (index == self->size)
        return darray_pushb(self, val);

    if (index == 0)
        return darray_pushf(self, val);

    if (val.size != self->element_size)
        return false;

    if (!darray_reserve_if_full(self))
        return false;

    index = index * self->element_size;
    if (self->size > 0) {
        memory_copy((self->data) + index + self->element_size, self->data + index, (self->size * self->element_size) - index);
    }

    memory_copy((self->data) + index, val.p, self->element_size);
    self->size += 1;
    return true;
}

bool darray_remove(Darray *self, usize index) {
    assert(self);
    if (index >= self->size)
        return false;

    if (index == self->size - 1)
        return darray_popb(self);

    if (index == 0)
        return darray_popf(self);

    index = index * self->element_size;
    memory_copy((self->data) + index, (self->data) + index + self->element_size, (self->size - 1) * self->element_size - index);
    self->size -= 1;

    return true;
}

Bytes darray_back(const Darray *self) {
    assert(self);
    if (self->size == 0)
        return nullbytes;

    usize index = (self->size - 1) * self->element_size;
    return (Bytes) { .p = (self->data) + index, .size = self->element_size };
}

Bytes darray_front(const Darray *self) {
    assert(self);
    if (self->size == 0)
        return nullbytes;

    return (Bytes) { .p = self->data, .size = self->element_size };
}

Bytes darray_at(const Darray *self, usize index) {
    assert(self);
    if (self->size == 0 || index >= self->size)
        return nullbytes;

    index = index * self->element_size;
    return (Bytes) { .p = (self->data) + index, .size = self->element_size };
}

bool darray_setb(Darray *self, Bytes val) {
    assert(self);
    return darray_set(self, self->size - 1, val);
}

bool darray_setf(Darray *self, Bytes val) {
    assert(self);
    return darray_set(self, 0, val);
}

bool darray_set(Darray *self, usize index, Bytes val) {
    assert(self);
    if (self->size == 0 || index >= self->size)
        return false;

    if (val.size != self->element_size)
        return false;

    index = index * self->element_size;
    memory_copy((self->data) + index, val.p, self->element_size);
    return true;
}

bool darray_empty(const Darray *self) {
    assert(self);
    return self->size == 0;
}

usize darray_size(const Darray *self) {
    assert(self);
    return self->size;
}

usize darray_elm_size(const Darray *self) {
    assert(self);
    return self->element_size;
}

usize darray_capacity(const Darray *self) {
    assert(self);
    return self->capacity;
}

void *darray_data(Darray *self) {
    assert(self);
    return self->data;
}

const void *darray_const_data(const Darray *self) {
    assert(self);
    return self->data;
}
