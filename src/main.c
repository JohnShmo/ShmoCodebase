#include "shmo/table.h"
#include <stdio.h>

int main() {
    arena_t a;
    arena_create(&a);
    heap_allocator_t alloc = heap_allocator_arena(&a);

    table_t t;
    table_create(&t, hash_string, compare_string, &alloc);

    table_set(&t, bytes_of_str("Mark"), bytes_of_str("cool guy"));
    table_set(&t, bytes_of_str("Dave"), bytes_of_str("super cool guy"));
    table_set(&t, bytes_of_str("Maria"), bytes_of_str("cool girl"));
    table_set(&t, bytes_of_str("Dog"), bytes_of_str("goodest boy"));
    table_set(&t, bytes_of_str("1"), bytes_of_str("foo"));
    table_set(&t, bytes_of_str("2"), bytes_of_str("foo"));
    table_set(&t, bytes_of_str("3"), bytes_of_str("foo"));
    table_set(&t, bytes_of_str("4"), bytes_of_str("foo"));
    table_set(&t, bytes_of_str("5"), bytes_of_str("foo"));
    table_set(&t, bytes_of_str("6"), bytes_of_str("foo"));
    table_set(&t, bytes_of_str("7"), bytes_of_str("foo"));
    table_set(&t, bytes_of_str("8"), bytes_of_str("foo"));
    table_set(&t, bytes_of_str("9"), bytes_of_str("foo"));
    table_set(&t, bytes_of_str("10"), bytes_of_str("bar"));
    table_set(&t, bytes_of_str("11"), bytes_of_str("bar"));
    table_set(&t, bytes_of_str("12"), bytes_of_str("bar"));
    table_set(&t, bytes_of_str("13"), bytes_of_str("bar"));
    table_set(&t, bytes_of_str("14"), bytes_of_str("bar"));
    table_set(&t, bytes_of_str("15"), bytes_of_str("bar"));
    table_set(&t, bytes_of_str("16"), bytes_of_str("bar"));

    printf("%s = %s\n", "Mark", bytes_to_str(table_get(&t, bytes_of_str("Mark"))));
    printf("%s = %s\n", "Dave", bytes_to_str(table_get(&t, bytes_of_str("Dave"))));
    printf("%s = %s\n", "Maria", bytes_to_str(table_get(&t, bytes_of_str("Maria"))));
    printf("%s = %s\n", "Dog", bytes_to_str(table_get(&t, bytes_of_str("Dog"))));

    table_destroy(&t);
    arena_destroy(&a);
    return 0;
}
