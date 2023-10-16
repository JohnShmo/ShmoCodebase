//
// Created by scott on 9/13/2023.
//

#include "shmo/strings.h"
#include "shmo/darray.h"

Strview strview(const char *data, usize length) {
    if (!data || length == 0)
        return nullstrview;
    return (Strview) { .data = data, .length = length };
}

Strview strview_of(const char *cstr) {
    if (!cstr)
        return nullstrview;
    return (Strview) { .data = cstr, .length = strlen(cstr) };
}

char *strview_dup(Strview view, Allocator *allocator) {;
    if (!allocator) {
        allocator = stdalloc;
    }

    char *buffer = allocator_malloc(allocator, view.length + 1);
    if (!buffer)
        return nullptr;
    memcpy(buffer, view.data, view.length);
    buffer[view.length] = '\0';
    return buffer;
}

void strview_cpy(Strview view, char *dest, usize dest_size) {
    if (!dest || dest_size == 0) {
        return;
    }

    usize len = min(dest_size - 1, view.length);

    memcpy(dest, view.data, len);
    dest[len] = '\0';
}

usize strview_cat(Strview lhs, Strview rhs, char *dest, usize dest_size) {
    usize result = lhs.length + rhs.length;
    if (!dest || dest_size == 0) {
        return result;
    }

    strview_cpy(lhs, dest, dest_size);
    if (dest_size > lhs.length) {
        strview_cpy(rhs, dest + lhs.length, dest_size - lhs.length);
    }
    return result;
}

char *strview_join(Strview *views, usize views_count, Strview delim, Allocator *allocator) {
    if (!allocator)
        allocator = stdalloc;
    char *result = nullptr;
    usize total_len = strview_len_all(views, views_count);
    usize delim_len = strview_len(delim);
    if (views_count > 1 && !strview_is_null(delim))
        total_len += delim_len * (views_count - 1);
    result = allocator_malloc(allocator, total_len + 1);
    if (!result)
        return nullptr;
    if (total_len == 0)
        goto finish;
    usize offset = 0;
    for (usize i = 0; i < views_count; ++i) {
        strview_cpy(views[i], result + offset, (total_len + 1) - offset);
        offset += views[i].length;
        if (i < views_count - 1) {
            strview_cpy(delim, result + offset, (total_len + 1) - offset);
            offset += delim.length;
        }
    }
    finish:
    result[total_len] = '\0';
    return result;
}

const char *strview_data(Strview view) {
    return view.data;
}

usize strview_len(Strview view) {
    if (strview_is_null(view))
        return 0;
    return view.length;
}

usize strview_len_all(Strview *views, usize views_count) {
    if (!views)
        return 0;
    usize result = 0;
    for (usize i = 0; i < views_count; ++i) {
        result += strview_len(views[i]);
    }
    return result;
}

usize string_len(const char *str) {
    if (!str)
        return 0;
    usize result = 0;
    while(*(str++))
        ++result;
    return result;
}

usize string_len_all(const char **strs, usize strs_count) {
    if (!strs)
        return 0;
    usize total = 0;
    for (usize i = 0; i < strs_count; ++i) {
        total += string_len(strs[i]);
    }
    return total;
}

void string_cpy(char *dest, usize dest_size, const char *src) {
    if (!dest || dest_size == 0 || !src)
        return;
    usize copied_count = 0;
    while(*src && dest_size - 1 > copied_count) {
        *(dest++) = *(src++);
        ++copied_count;
    }
    *dest = '\0';
}

usize string_cat(char *dest, usize dest_size, const char *src) {
    usize dest_len = string_len(dest);
    usize src_len = string_len(src);
    usize result = dest_len + src_len;

    if (!dest || !src || dest_len >= dest_size)
        return result;

    string_cpy(dest + dest_len, dest_size - dest_len, src);
    return result;
}

char *string_dup(const char *src, Allocator *allocator) {
    if (!src)
        return nullptr;
    if (!allocator)
        allocator = stdalloc;
    usize result_size = string_len(src) + 1;
    char *result = allocator_malloc(allocator, result_size);
    if (!result)
        return nullptr;
    string_cpy(result, result_size, src);
    return result;
}

char *string_join(const char **strs, usize strs_count, const char *delim, Allocator *allocator) {
    if (!allocator)
        allocator = stdalloc;
    char *result = nullptr;
    usize total_len = string_len_all(strs, strs_count);
    usize delim_len = string_len(delim);
    if (strs_count > 1 && delim)
        total_len += delim_len * (strs_count - 1);
    result = allocator_malloc(allocator, total_len + 1);
    if (!result)
        return nullptr;
    if (total_len == 0)
        goto finish;
    usize offset = 0;
    for (usize i = 0; i < strs_count; ++i) {
        string_cpy(result + offset, (total_len + 1) - offset, strs[i]);
        offset += string_len(strs[i]);
        if (i < strs_count - 1) {
            string_cpy(result + offset, (total_len + 1) - offset, delim);
            offset += delim_len;
        }
    }
    finish:
    result[total_len] = '\0';
    return result;
}

Strview string_get_view(const char *str, R1u range) {
    if (!str || range.begin > range.end || range.end - range.begin == 0)
        return nullstrview;
    usize len = string_len(str);
    if (range.begin >= len)
        return nullstrview;
    if (range.end > len)
        return nullstrview;
    return strview(str + range.begin, range.end - range.begin);
}

char string_get_char(const char *str, usize index) {
    if (!str)
        return '\0';
    return str[index];
}

struct StringBuilder {
    Darray *darray;
    Allocator *allocator;
};

local_fn void string_builder_strip_null_chars(StringBuilder *builder) {
    assert(builder);
    while(true) {
        Bytes back = darray_back(builder->darray);
        if (bytes_is_null(back))
            break;
        if (bytes_to(char, back) == '\0')
            darray_popb(builder->darray);
        else
            break;
    }
}

StringBuilder *string_builder_create(Allocator *allocator) {
    if (!allocator)
        allocator = stdalloc;

    StringBuilder *self = allocator_malloc(allocator, sizeof(StringBuilder));
    if (!self)
        return nullptr;

    self->darray = darray_create(sizeof(char), allocator);
    if (!self->darray) {
        allocator_free(allocator, self);
        return nullptr;
    }

    self->allocator = allocator;

    return self;
}

void string_builder_destroy(StringBuilder *self) {
    if (!self)
        return;

    darray_destroy(self->darray);
    allocator_free(self->allocator, self);
}

bool string_builder_push_char(StringBuilder *self, char c) {
    assert(self);
    if (c == '\0')
        return false;
    string_builder_strip_null_chars(self);

    return darray_pushb(self->darray, bytes_of(c));
}

bool string_builder_push_str(StringBuilder *self, const char *s) {
    assert(self);
    if (!s)
        return false;
    string_builder_strip_null_chars(self);

    while(*s) {
        if (!string_builder_push_char(self, *s))
            return false;
        ++s;
    }

    return true;
}

bool string_builder_push_fmt(StringBuilder *self, const char *fmt, ...) {
    assert(self);
    if (!fmt)
        return false;
    string_builder_strip_null_chars(self);

    char *buffer;
    usize buffer_size;

    va_list vl;
    va_start(vl, fmt);
    int res = vsnprintf(nullptr, 0, fmt, vl);
    if (res < 0)
        return false;
    buffer_size = res + 1;
    va_end(vl);

    if (buffer_size == 1)
        return true;

    usize position = darray_size(self->darray);
    if (!darray_resize(self->darray, position + buffer_size, nullbytes)) {
        return false;
    }

    buffer = (char *)darray_data(self->darray) + position;

    va_start(vl, fmt);
    vsnprintf(buffer, buffer_size, fmt, vl);
    va_end(vl);

    darray_popb(self->darray); // pop off the null terminator

    return true;
}

bool string_builder_push_view(StringBuilder *self, Strview v) {
    assert(self);
    if (strview_is_null(v))
        return false;
    string_builder_strip_null_chars(self);

    for (usize i = 0; i < v.length; ++i) {
        if (!string_builder_push_char(self, v.data[i]))
            return false;
    }

    return true;
}

bool string_builder_set_char(StringBuilder *self, usize index, char c) {
    assert(self);
    string_builder_strip_null_chars(self);
    return darray_set(self->darray, index, bytes_of(c));
}

bool string_builder_fill_char(StringBuilder *self, R1u range, char c) {
    assert(self);
    if (range.begin >= range.end)
        return false;
    string_builder_strip_null_chars(self);

    usize count = range.end - range.begin;
    if (range.begin + count > darray_size(self->darray))
        return false;
    for (usize i = 0; i < count; ++i) {
        if (!string_builder_set_char(self, range.begin + i, c))
            return false;
    }
    return true;
}

bool string_builder_insert_char(StringBuilder *self, usize index, char c) {
    assert(self);
    if (c == '\0')
        return false;
    string_builder_strip_null_chars(self);

    return darray_insert(self->darray, index, bytes_of(c));
}

bool string_builder_insert_str(StringBuilder *self, usize index, const char *s) {
    assert(self);
    if (!s)
        return false;
    string_builder_strip_null_chars(self);

    usize len = string_len(s);
    if (!darray_resize(self->darray, darray_size(self->darray) + len, nullbytes))
        return false;

    char *data = darray_data(self->darray);
    data += index;

    memory_copy(data + len, data, darray_size(self->darray) - index);

    for (usize i = 0; i < len; ++i) {
        data[i] = s[i];
    }

    return true;
}

bool string_builder_insert_fmt(StringBuilder *self, usize index, const char *fmt, ...) {
    assert(self);
    if (!fmt)
        return false;
    string_builder_strip_null_chars(self);

    char *buffer;
    usize buffer_size;

    va_list vl;
    va_start(vl, fmt);
    int res = vsnprintf(nullptr, 0, fmt, vl);
    if (res < 0)
        return false;
    buffer_size = res + 1;
    va_end(vl);

    if (buffer_size == 1)
        return true;

    if (!darray_resize(self->darray, darray_size(self->darray) + buffer_size, nullbytes))
        return false;
    buffer = darray_data(self->darray);
    buffer += index;

    memory_copy(buffer + buffer_size, buffer, darray_size(self->darray) - index);

    va_start(vl, fmt);
    vsnprintf(buffer, buffer_size, fmt, vl);
    va_end(vl);

    darray_remove(self->darray, index + buffer_size - 1); // remove the null terminator

    return true;
}

bool string_builder_insert_view(StringBuilder *self, usize index, Strview v) {
    assert(self);
    if (strview_is_null(v))
        return false;
    string_builder_strip_null_chars(self);

    for (usize i = 0; i < v.length; ++i) {
        if (!string_builder_insert_char(self, index++, v.data[i]))
            return false;
    }

    return true;
}

bool string_builder_remove(StringBuilder *self, R1u range) {
    assert(self);
    if (range.begin >= range.end)
        return false;
    string_builder_strip_null_chars(self);

    usize count = range.end - range.begin;
    if (range.begin + count > darray_size(self->darray))
        return false;
    for (usize i = 0; i < count; ++i) {
        if (!darray_remove(self->darray, range.begin)) {
            return false;
        }
    }
    return true;
}

void string_builder_clear(StringBuilder *self) {
    assert(self);
    darray_clear(self->darray);
}

const char *string_builder_cstr(StringBuilder *self) {
    assert(self);
    string_builder_strip_null_chars(self);

    if (!darray_pushb(self->darray, bytes_of((char){'\0'})))
        return nullptr;

    return darray_const_data(self->darray);
}

char string_builder_get_char(StringBuilder *self, usize index) {
    assert(self);
    string_builder_strip_null_chars(self);

    Bytes elm = darray_at(self->darray, index);
    if (bytes_is_null(elm))
        return '\0';

    return bytes_to(char, elm);
}

Strview string_builder_get_view(StringBuilder *self, R1u range) {
    assert(self);
    string_builder_strip_null_chars(self);

    if (range.begin >= range.end)
        return nullstrview;
    usize count = range.end - range.begin;
    if (range.begin + count > darray_size(self->darray))
        return nullstrview;

    return strview((const char *)darray_const_data(self->darray) + range.begin, count);
}

usize string_builder_len(StringBuilder *self) {
    assert(self);
    string_builder_strip_null_chars(self);
    return darray_size(self->darray);
}
