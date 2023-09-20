//
// Created by scott on 9/8/2023.
//

#include "shmo/arena.h"

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

struct arena_t {
    arena_page_t *pages;
    arena_block_header_t *free_blocks[COUNT_ARENA_BLOCK];
};

#define ARENA_MIN_PAGE_SIZE (1024ULL * 8ULL)

struct arena_block_header_t {
    usize size;
    arena_block_header_t *next;
};

struct arena_page_t {
    u8 *start;
    u8 *end;
    u8 *current;
    arena_page_t *next;
};

local_fn bool arena_can_page_fit(arena_page_t *p, usize n) {
    assert(p);
    assert(n);
    usize diff = int_from_ptr(p->end) - int_from_ptr(p->current);
    return n <= diff;
}

local_fn void arena_page_create(arena_page_t *p, usize size) {
    assert(p);
    assert(size);
    p->start = malloc(max(ARENA_MIN_PAGE_SIZE, size));
    assert(p->start);
    p->current = p->start;
    p->end = p->start + size;
    p->next = nullptr;
}

local_fn u8 *arena_page_alloc(arena_page_t *p, usize size) {
    assert(p);
    assert(size);
    assert(arena_can_page_fit(p, size));
    u8 *result = p->current;
    p->current += size;
    return result;
}

local_fn arena_block_size_t arena_get_block_size(usize n) {
    if (n <= 32)
        return ARENA_BLOCK_32;
    else if (n <= 64)
        return ARENA_BLOCK_64;
    else if (n <= 128)
        return ARENA_BLOCK_128;
    else if (n <= 256)
        return ARENA_BLOCK_256;
    else if (n <= 512)
        return ARENA_BLOCK_512;
    else if (n <= 1024)
        return ARENA_BLOCK_1024;
    else if (n <= 2048)
        return ARENA_BLOCK_2048;
    else if (n <= 4096)
        return ARENA_BLOCK_4096;
    else if (n <= 8192)
        return ARENA_BLOCK_8192;
    else
        return ARENA_BLOCK_LARGE;
}

local_fn usize arena_calculate_final_block_size(usize n, arena_block_size_t block_size) {
    switch (block_size) {
        case ARENA_BLOCK_32:
            return (usize)32;
        case ARENA_BLOCK_64:
            return (usize)64;
        case ARENA_BLOCK_128:
            return (usize)128;
        case ARENA_BLOCK_256:
            return (usize)256;
        case ARENA_BLOCK_512:
            return (usize)512;
        case ARENA_BLOCK_1024:
            return (usize)1024;
        case ARENA_BLOCK_2048:
            return (usize)2048;
        case ARENA_BLOCK_4096:
            return (usize)4096;
        case ARENA_BLOCK_8192:
            return (usize)8192;
        case ARENA_BLOCK_LARGE:
            return n;
        default:
            assert_break();
    }
}

local_fn void arena_page_destroy(arena_page_t *p) {
    assert(p);
    free(p->start);
    p->start = nullptr;
    p->end = nullptr;
    p->current = nullptr;
    p->next = nullptr;
}

arena_t *arena_create(void) {
    arena_t *dest = malloc(sizeof(arena_t));
    dest->pages = nullptr;
    for (i32 i = 0; i < COUNT_ARENA_BLOCK; ++i) {
        dest->free_blocks[i] = nullptr;
    }
    return dest;
}

void arena_destroy(arena_t *a) {
    assert(a);
    arena_release(a);
    free(a);
}

void arena_release(arena_t *a) {
    assert(a);
    while(a->pages) {
        arena_page_t *to_free = a->pages;
        a->pages = to_free->next;
        arena_page_destroy(to_free);
        free(to_free);
    }
    for (i32 i = 0; i < COUNT_ARENA_BLOCK; ++i) {
        a->free_blocks[i] = nullptr;
    }
}

void *arena_malloc(arena_t *a, usize n) {
    assert(a);
    assert(n);
    n = (usize)round_up_u64((u64)n + (u64)sizeof(arena_block_header_t), 32ULL);
    arena_block_size_t block_size = arena_get_block_size(n);
    n = arena_calculate_final_block_size(n, block_size);

    if (a->free_blocks[block_size]) {
        arena_block_header_t *free_block = a->free_blocks[block_size];
        arena_block_header_t *prev_block = a->free_blocks[block_size];
        if (block_size == ARENA_BLOCK_LARGE) {
            while (free_block && n > free_block->size) {
                prev_block = free_block;
                free_block = free_block->next;
            }
        }
        if (free_block) {
            arena_block_header_t *next_block = free_block->next;
            if (free_block == a->free_blocks[block_size]) {
                a->free_blocks[block_size] = next_block;
            } else {
                prev_block->next = next_block;
            }
            free_block->next = nullptr;
            u8 *free_mem = (u8 *)(free_block + 1);
            return free_mem;
        }
    }

    if (!a->pages) {
        a->pages = malloc(sizeof(arena_page_t));
        assert(a->pages);
        arena_page_create(a->pages, n);
    } else if (!arena_can_page_fit(a->pages, n)) {
        arena_page_t *old = a->pages;
        a->pages = malloc(sizeof(arena_page_t));
        assert(a->pages);
        arena_page_create(a->pages, n);
        a->pages->next = old;
    }

    u8 *fetched = arena_page_alloc(a->pages, n);
    arena_block_header_t *header = (arena_block_header_t *)fetched;
    header->next = nullptr;
    header->size = n;

    u8 *result = fetched + sizeof(arena_block_header_t);
    return result;
}

void *arena_calloc(arena_t *a, usize n, usize size) {
    assert(a);
    assert(n);
    assert(size);
    n = n * size;
    void *result = arena_malloc(a, n);
    assert(result);
    memory_zero(result, n);
    return result;
}

void *arena_realloc(arena_t *a, void *p, usize n) {
    assert(a);
    assert(n);

    if (!p) {
        void *result = arena_malloc(a, n);
        assert(result);
        return result;
    }

    arena_block_header_t *header = ((arena_block_header_t *)p) - 1;
    if (header->size - sizeof(arena_block_header_t) >= n) {
        return p;
    }

    void *new_loc = arena_malloc(a, n);
    assert(new_loc);
    memory_copy(new_loc, p, n);

    arena_free(a, p);
    return new_loc;
}

void arena_free(arena_t *a, void *p) {
    assert(a);
    if (!p)
        return;
    arena_block_header_t *header = ((arena_block_header_t *)p) - 1;
    arena_block_size_t block_size = arena_get_block_size(header->size);
    header->next = a->free_blocks[block_size];
    a->free_blocks[block_size] = header;
}