//
// Created by scott on 9/9/2023.
//

#ifndef SHMOCODEBASE_ALLOCATOR_H
#define SHMOCODEBASE_ALLOCATOR_H

#include "arena.h"

typedef void *(*malloc_func_t)(void *, size_t);
typedef void *(*calloc_func_t)(void *, size_t, size_t);
typedef void *(*realloc_func_t)(void *, void *, size_t);
typedef void (*free_func_t)(void *, void *);

typedef struct heap_allocator_t {
    void *allocator;
    malloc_func_t malloc_func;
    calloc_func_t calloc_func;
    realloc_func_t realloc_func;
    free_func_t free_func;
} heap_allocator_t;

void *heap_malloc(heap_allocator_t *a, size_t n);
void *heap_calloc(heap_allocator_t *a, size_t n, size_t size);
void *heap_realloc(heap_allocator_t *a, void *p, size_t n);
void heap_free(heap_allocator_t *a, void *p);

heap_allocator_t heap_allocator_arena(arena_t *arena);

extern heap_allocator_t *stdalloc;

#endif //SHMOCODEBASE_ALLOCATOR_H
