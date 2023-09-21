//
// Created by scott on 9/8/2023.
//

#ifndef SHMOCODEBASE_FREE_LIST_ARENA_H
#define SHMOCODEBASE_FREE_LIST_ARENA_H

#include "base.h"

typedef struct FreeListArena FreeListArena;

FreeListArena *free_list_arena_create(void);
void free_list_arena_destroy(FreeListArena *a);
void free_list_arena_release(FreeListArena *a);
void *free_list_arena_malloc(FreeListArena *a, usize n);
void *free_list_arena_calloc(FreeListArena *a, usize n, usize size);
void *free_list_arena_realloc(FreeListArena *a, void *p, usize n);
void free_list_arena_free(FreeListArena *a, void *p);

#endif //SHMOCODEBASE_FREE_LIST_ARENA_H
