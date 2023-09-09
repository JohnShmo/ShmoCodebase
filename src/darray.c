//
// Created by scott on 9/9/2023.
//

#include "shmo/darray.h"

void darray_create(darray_t *da, size_t element_size, heap_allocator_t *allocator) {
    assert(da);
    assert(element_size);
    if (!allocator)
        allocator = stdalloc;
    da->size = 0;
    da->capacity = 0;
    da->element_size = element_size;
    da->data = nullptr;
    da->allocator = allocator;
}

void darray_destroy(darray_t *da) {
    assert(da);
    assert(da->allocator);
    heap_free(da->allocator, da->data);
    da->size = 0;
    da->capacity = 0;
    da->element_size = 0;
    da->data = nullptr;
    da->allocator = nullptr;
}

void darray_reserve(darray_t *da, size_t n) {
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

void darray_resize(darray_t *da, size_t n, const void *fillval) {
    assert(da);

    darray_reserve(da, n);

    if (n > da->size) {
        size_t count = n - da->size;
        if (fillval) {
            for (size_t i = 0; i < count; ++i) {
                size_t index = da->size + i;
                index *= da->element_size;
                memory_copy(((u8 *)da->data) + index, fillval, da->element_size);
            }
        } else {
            memory_zero(((u8 *)da->data) + (da->size * da->element_size), count * da->element_size);
        }
    }

    da->size = n;
}

void darray_shrink(darray_t *da);
void darray_pushb(darray_t *da, const void *val);
void darray_popb(darray_t *da);
void darray_pushf(darray_t *da, const void *val);
void darray_popf(darray_t *da);
void darray_insert(darray_t *da, size_t index, const void *val);
void darray_remove(darray_t *da, size_t index);
void *darray_back(darray_t *da);
void *darray_front(darray_t *da);
void *darray_at(darray_t *da, size_t index);
bool darray_empty(darray_t *da);
size_t darray_size(darray_t *da);
size_t darray_capacity(darray_t *da);
size_t darray_element_size(darray_t *da);