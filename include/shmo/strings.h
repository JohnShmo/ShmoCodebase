//
// Created by scott on 9/13/2023.
//

#ifndef SHMOCODEBASE_STRINGS_H
#define SHMOCODEBASE_STRINGS_H

#include "allocator.h"

typedef struct strview_t {
    const char *data;
    size_t length;
} strview_t;

strview_t strview(const char *data, size_t length);
strview_t strview_of(const char *cstr);
const char *strview_cstr(const strview_t *view);
char *strview_dup(const strview_t *view, heap_allocator_t *allocator);
void strview_cpy(const strview_t *view, char *dest, size_t dest_size);
size_t strview_cat(const strview_t *lhs, const strview_t *rhs, char *dest, size_t dest_size);
const char *strview_data(const strview_t *view);
size_t strview_len(const strview_t *view);

#endif //SHMOCODEBASE_STRINGS_H
