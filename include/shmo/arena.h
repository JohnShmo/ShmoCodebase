//
// Created by scott on 9/21/2023.
//

#ifndef SHMOCODEBASE_ARENA_H
#define SHMOCODEBASE_ARENA_H

#include "allocator.h"

typedef struct Arena Arena;

Arena *arena_create(void);
void arena_destroy(Arena *a);
void arena_release(Arena *a);
void *arena_malloc(Arena *a, usize n);
void *arena_calloc(Arena *a, usize n, usize size);
void *arena_realloc(Arena *a, void *p, usize n);
void arena_free(Arena *a, void *p);

Allocator allocator_arena(Arena *arena);

#endif //SHMOCODEBASE_ARENA_H
