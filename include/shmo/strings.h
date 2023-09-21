//
// Created by scott on 9/13/2023.
//

#ifndef SHMOCODEBASE_STRINGS_H
#define SHMOCODEBASE_STRINGS_H

#include "allocator.h"

typedef struct Strview {
    const char *data;
    usize length;
} Strview;

Strview strview(const char *data, usize length);
Strview strview_of(const char *cstr);
const char *strview_cstr(Strview view);
char *strview_dup(Strview view, HeapAllocator *allocator);
void strview_cpy(Strview view, char *dest, usize dest_size);
usize strview_cat(Strview lhs, Strview rhs, char *dest, usize dest_size);
const char *strview_data(Strview view);
usize strview_len(Strview view);

#endif //SHMOCODEBASE_STRINGS_H
