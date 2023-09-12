#include "shmo/darray.h"
#include <stdio.h>

DARRAY_DEF(darrayi32, i32)

int main() {
    darrayi32_t *arr = darrayi32_create(stdalloc);

    darrayi32_pushb(arr, 10);
    darrayi32_pushb(arr, 20);
    darrayi32_pushb(arr, 30);
    darrayi32_pushb(arr, 40);
    darrayi32_pushb(arr, 50);
    darrayi32_pushb(arr, 60);

    for (size_t i = 0; i < darrayi32_size(arr); ++i) {
        printf("[%llu] -> %d\n", i, *darrayi32_at(arr, i));
    }

    darrayi32_destroy(arr);
    return 0;
}
