#include "shmo/darray.h"
#include "shmo/set.h"
#include "shmo/table.h"
#include <stdio.h>

DARRAY_DEF(IntArray, intarray, i32)
TABLE_DEF(IntTable, inttable, i32, i32, hash_i32, compare_i32)
SET_DEF(IntSet, intset, i32, hash_i32, compare_i32)

int main(i32 argc, char *argv[]) {
    (void)argc; (void)argv;

    LinearArena *arena = linear_arena_create();
    HeapAllocator allocator = heap_allocator_linear_arena(arena);

    IntTable *t = inttable_create(&allocator);

    inttable_put(t, 0, 100);
    inttable_put(t, 1, 200);
    inttable_put(t, 2, 300);
    inttable_put(t, 3, 400);
    inttable_put(t, 4, 500);
    inttable_put(t, 5, 600);
    inttable_put(t, 6, 700);
    inttable_put(t, 7, 800);
    inttable_put(t, 8, 900);


    for (i32 i = 0; i < 9; ++i) {
        printf("%d : %d\n", i, *inttable_get(t, i));
    }

    inttable_destroy(t);
    linear_arena_destroy(arena);
    return 0;
}
