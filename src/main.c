#include "shmo/darray.h"
#include <stdio.h>

DARRAY_DEF(IntArray, int)

int main(i32 argc, char *argv[]) {

    IntArray_t *b = IntArray_create(stdalloc);

    IntArray_destroy(b);

    return 0;
}