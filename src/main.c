#include "shmo/table.h"
#include <stdio.h>

int main(i32 argc, char *argv[]) {
    (void)argc; (void)argv;

    Table *table = table_create(stdalloc);
    table_put(table, bytes_of_str("Mike"), bytes_of_str("Cool guy"));
    table_put(table, bytes_of_str("Dave"), bytes_of_str("Cooler guy"));

    for (TableItr itr = table_itr(table); !table_itr_end(itr); itr = table_itr_next(itr)) {
        TablePair pair = table_itr_get(itr);
        const char *key = bytes_to_str(pair.key);
        const char *val = bytes_to_str(pair.val);
        printf("%s : %s\n", key, val);
    }

    table_destroy(table);
    return 0;
}
