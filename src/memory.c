//
// Created by scott on 9/12/2023.
//

#include "shmo/memory.h"

void *ref_create(ref_t *r, size_t n_bytes, destructor_func_t destructor_func, heap_allocator_t *allocator) {
    assert(r);
    assert(n_bytes);
    if (allocator == nullptr) {
        allocator = stdalloc;
    }
    r->destructor_func = destructor_func;
    r->allocator = allocator;
    r->count = 1;
    r->p = heap_malloc(allocator, n_bytes);
    assert(r->p);
    return r->p;
}

void ref_inc(ref_t *r) {
    assert(r);
    assert(r->p);
    assert(r->count);
    r->count++;
}

void ref_dec(ref_t *r) {
    assert(r);
    assert(r->p);
    assert(r->count);
    r->count--;
    if (r->count == 0) {
        if (r->destructor_func) {
            r->destructor_func(r->p);
        }
        heap_free(r->allocator, r->p);
    }
}

void *ref_get(ref_t *r) {
    assert(r);
    assert(r->p);
    assert(r->count);
    return r->p;
}