#include "shmo/table.h"
#include "shmo/set.h"
#include <stdio.h>

TABLE_DEF(IntStrTable, istable, i32, char *, hash_i32, compare_i32)
SET_DEF(StrSet, strset, char *, hash_i32, compare_i32)

int main(i32 argc, char *argv[]) {
    (void)argc; (void)argv;

    IntStrTable *t = istable_create(stdalloc);

    istable_put(t, 0, "This");
    istable_put(t, 10, "Is an example");
    istable_put(t, 20, "Of an unordered");
    istable_put(t, 30, "Table");
    istable_put(t, 45, "The order of these entries");
    istable_put(t, 100, "Might get out of whack!");
    istable_put(t, 101, "But that's alright");
    istable_put(t, 122, "It's not supposed to be an array!");

    IntStrTable_ReleasedPair r = istable_remove(t, 10);
    if (r.released) {
        printf("REMOVED: %d : %s\n", r.key, r.val);
    }

    for (TableItr itr = istable_itr(t); !istable_itr_end(itr); itr = istable_itr_next(itr)) {
        const IntStrTable_Pair *pair = istable_itr_get(itr);
        printf("%d : %s\n", *pair->key, *pair->val);
    }

    istable_destroy(t);
    return 0;
}
