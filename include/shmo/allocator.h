//
// Created by scott on 9/9/2023.
//

#ifndef SHMOCODEBASE_ALLOCATOR_H
#define SHMOCODEBASE_ALLOCATOR_H

#include "free_list_arena.h"
#include "linear_arena.h"

typedef void *(*MallocFunc)(void *, usize);
typedef void *(*CallocFunc)(void *, usize, usize);
typedef void *(*ReallocFunc)(void *, void *, usize);
typedef void (*FreeFunc)(void *, void *);

typedef struct HeapAllocator {
    void *allocator;
    MallocFunc malloc_func;
    CallocFunc calloc_func;
    ReallocFunc realloc_func;
    FreeFunc free_func;
} HeapAllocator;

void *heap_malloc(HeapAllocator *a, usize n);
void *heap_calloc(HeapAllocator *a, usize n, usize size);
void *heap_realloc(HeapAllocator *a, void *p, usize n);
void heap_free(HeapAllocator *a, void *p);

HeapAllocator heap_allocator_free_list_arena(FreeListArena *arena);
HeapAllocator heap_allocator_linear_arena(LinearArena *arena);

extern HeapAllocator *const stdalloc;

#endif //SHMOCODEBASE_ALLOCATOR_H
