//
// Created by scott on 9/9/2023.
//

#ifndef SHMOCODEBASE_ALLOCATOR_H
#define SHMOCODEBASE_ALLOCATOR_H

#include "base.h"

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

void *allocator_malloc(Allocator *self, usize n);
void *allocator_calloc(Allocator *self, usize n, usize size);
void *allocator_realloc(Allocator *self, void *p, usize n);
void allocator_free(Allocator *self, void *p);



extern Allocator *const stdalloc;

#endif //SHMOCODEBASE_ALLOCATOR_H
