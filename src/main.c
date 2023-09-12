#include "shmo/table.h"
#include <stdio.h>

int main() {
    table_t *table = table_create(sizeof(char *),
                                  sizeof(char *),
                                  hash_cstr,
                                  compare_cstr,
                                  stdalloc);

    char *dave_key = "Dave";
    char *tim_key = "Tim";
    char *dave_val = "Cool guy.";
    char *tim_val = "Super cool guy.";

    table_put(table, &dave_key, &dave_val);
    table_put(table, &tim_key, &tim_val);

    printf("%s : %s\n", dave_key, *(char **)table_get(table, &dave_key));
    printf("%s : %s\n", tim_key, *(char **)table_get(table, &tim_key));

    table_destroy(table);
    return 0;
}
