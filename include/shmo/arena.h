//
// Created by scott on 9/8/2023.
//

#ifndef SHMOCODEBASE_ARENA_H
#define SHMOCODEBASE_ARENA_H

#include "base.h"

typedef struct arena_block_header_t arena_block_header_t;
typedef struct arena_page_t arena_page_t;

typedef enum arena_block_size_t {
    ARENA_BLOCK_32,
    ARENA_BLOCK_64,
    ARENA_BLOCK_128,
    ARENA_BLOCK_256,
    ARENA_BLOCK_512,
    ARENA_BLOCK_1024,
    ARENA_BLOCK_2048,
    ARENA_BLOCK_4096,
    ARENA_BLOCK_8192,
    ARENA_BLOCK_LARGE,
    COUNT_ARENA_BLOCK
} arena_block_size_t;

typedef struct arena_t {
    arena_page_t *pages;
    arena_block_header_t *free_blocks[COUNT_ARENA_BLOCK];
} arena_t;

arena_t arena_create(void);
void arena_destroy(arena_t *a);
void arena_release(arena_t *a);
void *arena_malloc(arena_t *a, size_t n);
void *arena_calloc(arena_t *a, size_t n, size_t size);
void *arena_realloc(arena_t *a, void *p, size_t n);
void arena_free(arena_t *a, void *p);

#endif //SHMOCODEBASE_ARENA_H
