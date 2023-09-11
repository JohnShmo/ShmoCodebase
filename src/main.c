#include "shmo/table.h"
#include <stdio.h>

int main() {
    arena_t a;
    arena_create(&a);
    heap_allocator_t alloc = heap_allocator_arena(&a);



    arena_destroy(&a);
    return 0;
}
