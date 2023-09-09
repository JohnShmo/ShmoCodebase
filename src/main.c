#include "shmo/darray.h"
#include <stdio.h>

int main() {

    darray_t arr;
    darray_create(&arr, sizeof(i32), stdalloc);

    darray_reserve(&arr, 10);

    darray_destroy(&arr);
    return 0;
}
