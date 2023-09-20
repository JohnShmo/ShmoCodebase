#include "shmo/darray.h"
#include "shmo/set.h"
#include <stdio.h>

DARRAY_DEF(IntArray, i32)

SET_DEF(IntSet, i32, nullptr, nullptr)

int main(i32 argc, char *argv[]) {

    IntArray_t *b = IntArray_create(stdalloc);

    IntArray_destroy(b);

    return 0;
}