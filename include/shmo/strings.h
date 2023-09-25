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
#define nullstrview (Strview) { .data = nullptr, .length = 0 }
#define strview_is_null(V) ((V).data == nullptr || (V).length == 0)

Strview strview(const char *data, usize length);
Strview strview_of(const char *cstr);
const char *strview_cstr(Strview view);
char *strview_dup(Strview view, HeapAllocator *allocator);
void strview_cpy(Strview view, char *dest, usize dest_size);
usize strview_cat(Strview lhs, Strview rhs, char *dest, usize dest_size);
char *strview_join(Strview *views, usize views_count, Strview sep, HeapAllocator *allocator);
const char *strview_data(Strview view);
usize strview_len(Strview view);
usize strview_len_all(Strview *views, usize views_count);

usize string_len(const char *str);
usize string_len_all(const char **strs, usize strs_count);
void string_cpy(char *dest, usize dest_size, const char *src);
usize string_cat(char *dest, usize dest_size, const char *src);
char *string_dup(const char *src, HeapAllocator *allocator);
char *string_join(const char **strs, usize strs_count, const char *delim, HeapAllocator *allocator);

#endif //SHMOCODEBASE_STRINGS_H
