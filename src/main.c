#include "shmo/darray.h"
#include <stdio.h>

int main() {

    darray_t arr;
    darray_create(&arr, sizeof(i32), stdalloc);

    darray_pushb(&arr, &((i32){10}));
    darray_pushf(&arr, &((i32){1}));
    darray_pushf(&arr, &((i32){2}));
    darray_pushf(&arr, &((i32){3}));
    darray_pushf(&arr, &((i32){4}));
    darray_pushf(&arr, &((i32){5}));
    darray_remove(&arr, 2);
    darray_insert(&arr, 2, &((i32){999}));

    printf("size = %llu\n", darray_size(&arr));
    printf("capacity = %llu\n", darray_capacity(&arr));

    for (i32 i = 0; i < darray_size(&arr); ++i) {
        printf("[%d] = %d\n", i, *(i32*) darray_at(&arr, i));
    }

    darray_destroy(&arr);
    return 0;
}
