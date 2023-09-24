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

static HeapAllocator stdalloc_ = {
    .allocator = nullptr,
    .malloc_func = malloc_c,
    .calloc_func = calloc_c,
    .realloc_func = realloc_c,
    .free_func = free_c
};
HeapAllocator *stdalloc = &stdalloc_;

HeapAllocator heap_allocator_free_list_arena(FreeListArena *arena) {
    return (HeapAllocator) {
        .allocator = arena,
        .malloc_func = (MallocFunc) free_list_arena_malloc,
        .calloc_func = (CallocFunc) free_list_arena_calloc,
        .realloc_func = (ReallocFunc) free_list_arena_realloc,
        .free_func = (FreeFunc) free_list_arena_free
    };
}

HeapAllocator heap_allocator_linear_arena(LinearArena *arena) {
    return (HeapAllocator) {
        .allocator = arena,
        .malloc_func = (MallocFunc) linear_arena_malloc,
        .calloc_func = (CallocFunc) linear_arena_calloc,
        .realloc_func = (ReallocFunc) linear_arena_realloc,
        .free_func = (FreeFunc) linear_arena_free
    };
}

void *heap_malloc(HeapAllocator *a, usize n) {
    if (!a || !a->malloc_func)
        return nullptr;
    return a->malloc_func(a->allocator, n);
}

void *heap_calloc(HeapAllocator *a, usize n, usize size) {
    if (!a || !a->calloc_func)
        return nullptr;
    return a->calloc_func(a->allocator, n, size);
}

void *heap_realloc(HeapAllocator *a, void *p, usize n) {
    if (!a || !a->realloc_func)
        return nullptr;
    return a->realloc_func(a->allocator, p, n);
}

void heap_free(HeapAllocator *a, void *p) {
    if (!a || !a->free_func)
        return;
    a->free_func(a->allocator, p);
}
