//
// Created by scott on 9/9/2023.
//

#include "shmo/allocator.h"

local_fn void *malloc_c(void *a, size_t n) {
    (void)a;
    return malloc(n);
}

local_fn void *calloc_c(void *a, size_t n, size_t size) {
    (void)a;
    return calloc(n, size);
}

local_fn void *realloc_c(void *a, void *p, size_t n) {
    (void)a;
    return realloc(p, n);
}

local_fn void free_c(void *a, void *p) {
    (void)a;
    free(p);
}

static Allocator stdalloc_ = {
    .allocator = nullptr,
    .malloc_func = malloc_c,
    .calloc_func = calloc_c,
    .realloc_func = realloc_c,
    .free_func = free_c
};
Allocator *const stdalloc = &stdalloc_;



void *allocator_malloc(Allocator *a, usize n) {
    if (!a || !a->malloc_func)
        return nullptr;
    return a->malloc_func(a->allocator, n);
}

void *allocator_calloc(Allocator *a, usize n, usize size) {
    if (!a || !a->calloc_func)
        return nullptr;
    return a->calloc_func(a->allocator, n, size);
}

void *allocator_realloc(Allocator *a, void *p, usize n) {
    if (!a || !a->realloc_func)
        return nullptr;
    return a->realloc_func(a->allocator, p, n);
}

void allocator_free(Allocator *a, void *p) {
    if (!a || !a->free_func)
        return;
    a->free_func(a->allocator, p);
}
