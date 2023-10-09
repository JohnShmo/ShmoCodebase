#include "shmo/darray.h"

int main(void) {
    Darray *arr = darray_create(sizeof(int), stdalloc);

    int i = 100000;
    while (i > 0) {
        darray_pushb(arr, bof(i));
        --i;
    }

    for (i = 0; i < (int)darray_size(arr); ++i) {
        printf("[%d] : %d\n", i, bto(int, darray_at(arr, i)));
    }

    darray_destroy(arr);
    return 0;
}
