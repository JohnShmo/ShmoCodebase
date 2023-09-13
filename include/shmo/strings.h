//
// Created by scott on 9/13/2023.
//

#ifndef SHMOCODEBASE_STRINGS_H
#define SHMOCODEBASE_STRINGS_H

#include "allocator.h"

typedef struct string_t {
    char *data;
    size_t length;
    heap_allocator_t *allocator;
} string_t;

typedef struct strview_t {
    const char *data;
    size_t length;
} strview_t;

void string_create(string_t *dest, const char *cstr, heap_allocator_t *allocator);
void string_copy(string_t *restrict dest, const string_t *restrict src, heap_allocator_t *allocator);
void string_copy_view(string_t *restrict dest, const strview_t *restrict src, heap_allocator_t *allocator);
void string_destroy(string_t *str);
const char *string_cstr(const string_t *str);
void string_cstr_to(const string_t *str, char *dest, size_t dest_size);
char *string_data(string_t *str);
const char *string_const_data(const string_t *str);
size_t string_length(const string_t *str);

strview_t strview(const char *data, size_t length);
strview_t strview_of(const string_t *str);
strview_t strview_of_cstr(const char *cstr);
const char *strview_cstr(const strview_t *view);
void strview_cstr_to(const strview_t *view, char *dest, size_t dest_size);
const char *strview_data(const strview_t *view);
size_t strview_length(const strview_t *view);

#endif //SHMOCODEBASE_STRINGS_H
