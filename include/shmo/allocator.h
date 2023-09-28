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

typedef struct Allocator {
    void *allocator;
    MallocFunc malloc_func;
    CallocFunc calloc_func;
    ReallocFunc realloc_func;
    FreeFunc free_func;
} Allocator;

void *allocator_malloc(Allocator *a, usize n);
void *allocator_calloc(Allocator *a, usize n, usize size);
void *allocator_realloc(Allocator *a, void *p, usize n);
void allocator_free(Allocator *a, void *p);

Allocator allocator_free_list_arena(FreeListArena *arena);
Allocator allocator_linear_arena(LinearArena *arena);

extern Allocator *const stdalloc;

#endif //SHMOCODEBASE_ALLOCATOR_H
