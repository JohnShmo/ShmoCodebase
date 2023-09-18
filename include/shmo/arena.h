//
// Created by scott on 9/8/2023.
//

#ifndef SHMOCODEBASE_ARENA_H
#define SHMOCODEBASE_ARENA_H

#include "base.h"

typedef struct arena_t arena_t;

arena_t *arena_create(void);
void arena_destroy(arena_t *a);
void arena_release(arena_t *a);
void *arena_malloc(arena_t *a, size_t n);
void *arena_calloc(arena_t *a, size_t n, size_t size);
void *arena_realloc(arena_t *a, void *p, size_t n);
void arena_free(arena_t *a, void *p);

#endif //SHMOCODEBASE_ARENA_H
