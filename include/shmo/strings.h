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
char *strview_dup(Strview view, Allocator *allocator);
void strview_cpy(Strview view, char *dest, usize dest_size);
usize strview_cat(Strview lhs, Strview rhs, char *dest, usize dest_size);
char *strview_join(Strview *views, usize views_count, Strview sep, Allocator *allocator);
const char *strview_data(Strview view);
usize strview_len(Strview view);
usize strview_len_all(Strview *views, usize views_count);

usize string_len(const char *str);
usize string_len_all(const char **strs, usize strs_count);
void string_cpy(char *dest, usize dest_size, const char *src);
usize string_cat(char *dest, usize dest_size, const char *src);
char *string_dup(const char *src, Allocator *allocator);
char *string_join(const char **strs, usize strs_count, const char *delim, Allocator *allocator);
Strview string_get_view(const char *str, R1u range);
char string_get_char(const char *str, usize index);

typedef struct StringBuilder StringBuilder;

StringBuilder *string_builder_create(Allocator *allocator);
void string_builder_destroy(StringBuilder *self);
bool string_builder_push_char(StringBuilder *self, char c);
bool string_builder_push_str(StringBuilder *self, const char *s);
bool string_builder_push_fmt(StringBuilder *self, const char *fmt, ...);
bool string_builder_push_view(StringBuilder *self, Strview v);
bool string_builder_set_char(StringBuilder *self, usize index, char c);
bool string_builder_fill_char(StringBuilder *self, R1u range, char c);
bool string_builder_insert_char(StringBuilder *self, usize index, char c);
bool string_builder_insert_str(StringBuilder *self, usize index, const char *s);
bool string_builder_insert_fmt(StringBuilder *self, usize index, const char *fmt, ...);
bool string_builder_insert_view(StringBuilder *self, usize index, Strview v);
bool string_builder_remove(StringBuilder *self, R1u range);
void string_builder_clear(StringBuilder *self);
const char *string_builder_cstr(StringBuilder *self);
char string_builder_get_char(StringBuilder *self, usize index);
Strview string_builder_get_view(StringBuilder *self, R1u range);
usize string_builder_len(StringBuilder *self);

#endif //SHMOCODEBASE_STRINGS_H
