//
// Created by scott on 9/8/2023.
//

#include "shmo/free_list_arena.h"

typedef struct FreeListArenaBlockHeader FreeListArenaBlockHeader;
typedef struct FreeListArenaPage FreeListArenaPage;

typedef enum FreeListArenaBlockSize {
    FREE_LIST_ARENA_BLOCK_32,
    FREE_LIST_ARENA_BLOCK_64,
    FREE_LIST_ARENA_BLOCK_128,
    FREE_LIST_ARENA_BLOCK_256,
    FREE_LIST_ARENA_BLOCK_512,
    FREE_LIST_ARENA_BLOCK_1024,
    FREE_LIST_ARENA_BLOCK_2048,
    FREE_LIST_ARENA_BLOCK_4096,
    FREE_LIST_ARENA_BLOCK_8192,
    FREE_LIST_ARENA_BLOCK_LARGE,
    COUNT_FREE_LIST_ARENA_BLOCK
} FreeListArenaBlockSize;

struct FreeListArena {
    FreeListArenaPage *pages;
    FreeListArenaBlockHeader *free_blocks[COUNT_FREE_LIST_ARENA_BLOCK];
};

#define ARENA_MIN_PAGE_SIZE (1024ULL * 8ULL)

struct FreeListArenaBlockHeader {
    usize size;
    FreeListArenaBlockHeader *next;
};

struct FreeListArenaPage {
    u8 *start;
    u8 *end;
    u8 *current;
    FreeListArenaPage *next;
};

local_fn bool arena_can_page_fit(FreeListArenaPage *p, usize n) {
    assert(p);
    assert(n);
    usize diff = (isize)p->end - (isize)p->current;
    return n <= diff;
}

local_fn void arena_page_create(FreeListArenaPage *p, usize size) {
    assert(p);
    assert(size);
    size = max(ARENA_MIN_PAGE_SIZE, size);
    p->start = malloc(size);
    assert(p->start);
    p->current = p->start;
    p->end = p->start + size;
    p->next = nullptr;
}

local_fn u8 *arena_page_alloc(FreeListArenaPage *p, usize size) {
    assert(p);
    assert(size);
    assert(arena_can_page_fit(p, size));
    u8 *result = p->current;
    p->current += size;
    return result;
}

local_fn FreeListArenaBlockSize arena_get_block_size(usize n) {
    if (n <= 32)
        return FREE_LIST_ARENA_BLOCK_32;
    else if (n <= 64)
        return FREE_LIST_ARENA_BLOCK_64;
    else if (n <= 128)
        return FREE_LIST_ARENA_BLOCK_128;
    else if (n <= 256)
        return FREE_LIST_ARENA_BLOCK_256;
    else if (n <= 512)
        return FREE_LIST_ARENA_BLOCK_512;
    else if (n <= 1024)
        return FREE_LIST_ARENA_BLOCK_1024;
    else if (n <= 2048)
        return FREE_LIST_ARENA_BLOCK_2048;
    else if (n <= 4096)
        return FREE_LIST_ARENA_BLOCK_4096;
    else if (n <= 8192)
        return FREE_LIST_ARENA_BLOCK_8192;
    else
        return FREE_LIST_ARENA_BLOCK_LARGE;
}

local_fn usize arena_calculate_final_block_size(usize n, FreeListArenaBlockSize block_size) {
    switch (block_size) {
        case FREE_LIST_ARENA_BLOCK_32:
            return (usize)32;
        case FREE_LIST_ARENA_BLOCK_64:
            return (usize)64;
        case FREE_LIST_ARENA_BLOCK_128:
            return (usize)128;
        case FREE_LIST_ARENA_BLOCK_256:
            return (usize)256;
        case FREE_LIST_ARENA_BLOCK_512:
            return (usize)512;
        case FREE_LIST_ARENA_BLOCK_1024:
            return (usize)1024;
        case FREE_LIST_ARENA_BLOCK_2048:
            return (usize)2048;
        case FREE_LIST_ARENA_BLOCK_4096:
            return (usize)4096;
        case FREE_LIST_ARENA_BLOCK_8192:
            return (usize)8192;
        case FREE_LIST_ARENA_BLOCK_LARGE:
            return n;
        default:
            assert_break();
    }
}

local_fn void arena_page_destroy(FreeListArenaPage *p) {
    assert(p);
    free(p->start);
    p->start = nullptr;
    p->end = nullptr;
    p->current = nullptr;
    p->next = nullptr;
}

FreeListArena *free_list_arena_create(void) {
    FreeListArena *dest = malloc(sizeof(FreeListArena));
    assert(dest);
    dest->pages = nullptr;
    for (i32 i = 0; i < COUNT_FREE_LIST_ARENA_BLOCK; ++i) {
        dest->free_blocks[i] = nullptr;
    }
    return dest;
}

void free_list_arena_destroy(FreeListArena *a) {
    assert(a);
    free_list_arena_release(a);
    free(a);
}

void free_list_arena_release(FreeListArena *a) {
    assert(a);
    while(a->pages) {
        FreeListArenaPage *to_free = a->pages;
        a->pages = to_free->next;
        arena_page_destroy(to_free);
        free(to_free);
    }
    for (i32 i = 0; i < COUNT_FREE_LIST_ARENA_BLOCK; ++i) {
        a->free_blocks[i] = nullptr;
    }
}

void *free_list_arena_malloc(FreeListArena *a, usize n) {
    assert(a);
    assert(n);
    n = (usize)round_up_u64((u64)n + (u64)sizeof(FreeListArenaBlockHeader), 32ULL);
    FreeListArenaBlockSize block_size = arena_get_block_size(n);
    n = arena_calculate_final_block_size(n, block_size);

    if (a->free_blocks[block_size]) {
        FreeListArenaBlockHeader *free_block = a->free_blocks[block_size];
        FreeListArenaBlockHeader *prev_block = a->free_blocks[block_size];
        if (block_size == FREE_LIST_ARENA_BLOCK_LARGE) {
            while (free_block && n > free_block->size) {
                prev_block = free_block;
                free_block = free_block->next;
            }
        }
        if (free_block) {
            FreeListArenaBlockHeader *next_block = free_block->next;
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
        a->pages = malloc(sizeof(FreeListArenaPage));
        assert(a->pages);
        arena_page_create(a->pages, n);
    } else if (!arena_can_page_fit(a->pages, n)) {
        FreeListArenaPage *old = a->pages;
        a->pages = malloc(sizeof(FreeListArenaPage));
        assert(a->pages);
        arena_page_create(a->pages, n);
        a->pages->next = old;
    }

    u8 *fetched = arena_page_alloc(a->pages, n);
    FreeListArenaBlockHeader *header = (FreeListArenaBlockHeader *)fetched;
    header->next = nullptr;
    header->size = n;

    u8 *result = fetched + sizeof(FreeListArenaBlockHeader);
    return result;
}

void *free_list_arena_calloc(FreeListArena *a, usize n, usize size) {
    assert(a);
    assert(n);
    assert(size);
    n = n * size;
    void *result = free_list_arena_malloc(a, n);
    assert(result);
    memory_zero(result, n);
    return result;
}

void *free_list_arena_realloc(FreeListArena *a, void *p, usize n) {
    assert(a);
    assert(n);

    if (!p) {
        void *result = free_list_arena_malloc(a, n);
        assert(result);
        return result;
    }

    FreeListArenaBlockHeader *header = ((FreeListArenaBlockHeader *)p) - 1;
    if (header->size - sizeof(FreeListArenaBlockHeader) >= n) {
        return p;
    }

    void *new_loc = free_list_arena_malloc(a, n);
    assert(new_loc);
    memory_copy(new_loc, p, n);

    free_list_arena_free(a, p);
    return new_loc;
}

void free_list_arena_free(FreeListArena *a, void *p) {
    assert(a);
    if (!p)
        return;
    FreeListArenaBlockHeader *header = ((FreeListArenaBlockHeader *)p) - 1;
    FreeListArenaBlockSize block_size = arena_get_block_size(header->size);
    header->next = a->free_blocks[block_size];
    a->free_blocks[block_size] = header;
}
