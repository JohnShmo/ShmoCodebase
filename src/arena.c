//
// Created by scott on 9/21/2023.
//

#include "shmo/arena.h"

typedef struct LinearArenaPage LinearArenaPage;

struct Arena {
    LinearArenaPage *pages;
};

#define ARENA_MIN_PAGE_SIZE (1024ULL * 8ULL)

struct LinearArenaPage {
    u8 *start;
    u8 *end;
    u8 *current;
    u8 *previous_alloc;
    LinearArenaPage *next;
};

local_fn bool arena_can_page_fit(LinearArenaPage *p, usize n) {
    assert(p);
    assert(n);
    usize diff = (isize)p->end - (isize)p->current;
    return n <= diff;
}

local_fn bool arena_page_create(LinearArenaPage *p, usize size) {
    assert(p);
    assert(size);
    size = max(ARENA_MIN_PAGE_SIZE, size);
    p->start = malloc(size);
    if (!p->start)
        return false;
    p->current = p->start;
    p->end = p->start + size;
    p->next = nullptr;
    p->previous_alloc = nullptr;
    return true;
}

local_fn u8 *arena_page_alloc(LinearArenaPage *p, usize size) {
    assert(p);
    assert(size);
    assert(arena_can_page_fit(p, size));
    u8 *result = p->current;
    p->previous_alloc = result;
    p->current += size;
    return result;
}

local_fn u8 *arena_page_realloc(LinearArenaPage *p, usize size) {
    assert(p);
    assert(size);
    assert(arena_can_page_fit(p, size));
    p->current += size;
    return p->previous_alloc;
}

local_fn void arena_page_destroy(LinearArenaPage *p) {
    assert(p);
    free(p->start);
    p->start = nullptr;
    p->end = nullptr;
    p->current = nullptr;
    p->next = nullptr;
}

Arena *arena_create(void) {
    Arena *dest = malloc(sizeof(Arena));
    if (!dest)
        return nullptr;
    dest->pages = nullptr;
    return dest;
}

void arena_destroy(Arena *a) {
    if (!a)
        return;
    arena_release(a);
    free(a);
}

void arena_release(Arena *a) {
    assert(a);
    while(a->pages) {
        LinearArenaPage *to_free = a->pages;
        a->pages = to_free->next;
        arena_page_destroy(to_free);
        free(to_free);
    }
}

void *arena_malloc(Arena *a, usize n) {
    assert(a);
    assert(n);
    n = (usize)round_up_u64((u64)n, (u64)sizeof(usize));

    if (!a->pages) {
        LinearArenaPage *page = malloc(sizeof(LinearArenaPage));
        if (!page)
            return nullptr;
        if (!arena_page_create(page, n)){
            free(page);
            return nullptr;
        }
        a->pages = page;
    } else if (!arena_can_page_fit(a->pages, n)) {
        LinearArenaPage *old = a->pages;
        LinearArenaPage *page = malloc(sizeof(LinearArenaPage));
        if (!page)
            return nullptr;
        if (!arena_page_create(page, n)){
            free(page);
            return nullptr;
        }
        a->pages = page;
        a->pages->next = old;
    }

    u8 *fetched = arena_page_alloc(a->pages, n);
    return fetched;
}

void *arena_calloc(Arena *a, usize n, usize size) {
    assert(a);
    assert(n);
    assert(size);
    n = n * size;
    void *result = arena_malloc(a, n);
    if (!result)
        return nullptr;
    memory_zero(result, n);
    return result;
}

void *arena_realloc(Arena *a, void *p, usize n) {
    assert(a);
    assert(n);

    if (!p) {
        return arena_malloc(a, n);
    }

    if (a->pages && p == a->pages->previous_alloc && arena_can_page_fit(a->pages, n)) {
        return arena_page_realloc(a->pages, n);
    }

    void *new_loc = arena_malloc(a, n);
    if (!new_loc)
        return nullptr;
    memcpy(new_loc, p, n);

    arena_free(a, p);
    return new_loc;
}

void arena_free(Arena *a, void *p) {
    (void)a; (void)p;
    // Doesn't do anything...
}

Allocator allocator_arena(Arena *arena) {
    return (Allocator) {
            .allocator = arena,
            .malloc_func = (MallocFunc) arena_malloc,
            .calloc_func = (CallocFunc) arena_calloc,
            .realloc_func = (ReallocFunc) arena_realloc,
            .free_func = (FreeFunc) arena_free
    };
}
