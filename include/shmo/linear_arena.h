//
// Created by scott on 9/21/2023.
//

#ifndef SHMOCODEBASE_LINEAR_ARENA_H
#define SHMOCODEBASE_LINEAR_ARENA_H

#include "base.h"

typedef struct LinearArena LinearArena;

LinearArena *linear_arena_create(void);
void linear_arena_destroy(LinearArena *a);
void linear_arena_release(LinearArena *a);
void *linear_arena_malloc(LinearArena *a, usize n);
void *linear_arena_calloc(LinearArena *a, usize n, usize size);
void *linear_arena_realloc(LinearArena *a, void *p, usize n);
void linear_arena_free(LinearArena *a, void *p);

#endif //SHMOCODEBASE_LINEAR_ARENA_H
