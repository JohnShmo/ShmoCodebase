#include "shmo/arena.h"
#include <stdio.h>

int main() {
    arena_t arena;
    arena_create(&arena);

    arena_destroy(&arena);
    return 0;
}
