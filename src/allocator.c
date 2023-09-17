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

static heap_allocator_t stdalloc_ = {
    .allocator = nullptr,
    .malloc_func = malloc_c,
    .calloc_func = calloc_c,
    .realloc_func = realloc_c,
    .free_func = free_c
};
heap_allocator_t *stdalloc = &stdalloc_;

heap_allocator_t heap_allocator_arena(arena_t *arena) {
    return (heap_allocator_t) {
        .allocator = arena,
        .malloc_func = (malloc_func_t)arena_malloc,
        .calloc_func = (calloc_func_t)arena_calloc,
        .realloc_func = (realloc_func_t)arena_realloc,
        .free_func = (free_func_t)arena_free
    };
}

void *heap_malloc(heap_allocator_t *a, size_t n) {
    assert(a);
    assert(a->malloc_func);
    return a->malloc_func(a->allocator, n);
}

void *heap_calloc(heap_allocator_t *a, size_t n, size_t size) {
    assert(a);
    assert(a->calloc_func);
    return a->calloc_func(a->allocator, n, size);
}

void *heap_realloc(heap_allocator_t *a, void *p, size_t n) {
    assert(a);
    assert(a->realloc_func);
    return a->realloc_func(a->allocator, p, n);
}

void heap_free(heap_allocator_t *a, void *p) {
    assert(a);
    assert(a->free_func);
    a->free_func(a->allocator, p);
}