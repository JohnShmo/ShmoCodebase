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

char *strview_dup(Strview view, HeapAllocator *allocator) {;
    if (!allocator) {
        allocator = stdalloc;
    }

    char *buffer = heap_malloc(allocator, view.length + 1);
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

char *strview_join(Strview *views, usize views_count, Strview delim, HeapAllocator *allocator) {
    if (!allocator)
        allocator = stdalloc;
    char *result = nullptr;
    usize total_len = strview_len_all(views, views_count);
    usize delim_len = strview_len(delim);
    if (views_count > 1 && !strview_is_null(delim))
        total_len += delim_len * (views_count - 1);
    result = heap_malloc(allocator, total_len + 1);
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

char *string_dup(const char *src, HeapAllocator *allocator) {
    if (!src)
        return nullptr;
    if (!allocator)
        allocator = stdalloc;
    usize result_size = string_len(src) + 1;
    char *result = heap_malloc(allocator, result_size);
    if (!result)
        return nullptr;
    string_cpy(result, result_size, src);
    return result;
}

char *string_join(const char **strs, usize strs_count, const char *delim, HeapAllocator *allocator) {
    if (!allocator)
        allocator = stdalloc;
    char *result = nullptr;
    usize total_len = string_len_all(strs, strs_count);
    usize delim_len = string_len(delim);
    if (strs_count > 1 && delim)
        total_len += delim_len * (strs_count - 1);
    result = heap_malloc(allocator, total_len + 1);
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
    HeapAllocator *allocator;
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

StringBuilder *string_builder_create(HeapAllocator *allocator) {
    if (!allocator)
        allocator = stdalloc;

    StringBuilder *builder = heap_malloc(allocator, sizeof(StringBuilder));
    if (!builder)
        return nullptr;

    builder->darray = darray_create(sizeof(char), allocator);
    if (!builder->darray) {
        heap_free(allocator, builder);
        return nullptr;
    }

    builder->allocator = allocator;

    return builder;
}

void string_builder_destroy(StringBuilder *builder) {
    if (!builder)
        return;

    darray_destroy(builder->darray);
    heap_free(builder->allocator, builder);
}

bool string_builder_push_char(StringBuilder *builder, char c) {
    if (!builder || c == '\0')
        return false;

    string_builder_strip_null_chars(builder);

    return darray_pushb(builder->darray, bytes_of(c));
}

bool string_builder_push_str(StringBuilder *builder, const char *s) {
    if (!builder || !s)
        return false;

    string_builder_strip_null_chars(builder);

    while(*s) {
        if (!string_builder_push_char(builder, *s))
            return false;
        ++s;
    }

    return true;
}

bool string_builder_push_fmt(StringBuilder *builder, const char *fmt, ...) {
    if (!builder || !fmt)
        return false;

    string_builder_strip_null_chars(builder);

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

    usize position = darray_size(builder->darray);
    if (!darray_resize(builder->darray, position + buffer_size, nullbytes)) {
        return false;
    }

    buffer = (char *)darray_data(builder->darray) + position;

    va_start(vl, fmt);
    vsnprintf(buffer, buffer_size, fmt, vl);
    va_end(vl);

    darray_popb(builder->darray); // pop off the null terminator

    return true;
}

bool string_builder_push_view(StringBuilder *builder, Strview v) {
    if (!builder || !strview_is_null(v))
        return false;

    string_builder_strip_null_chars(builder);

    for (usize i = 0; i < v.length; ++i) {
        if (!string_builder_push_char(builder, v.data[i]))
            return false;
    }

    return true;
}

bool string_builder_set_char(StringBuilder *builder, usize index, char c);
bool string_builder_fill_char(StringBuilder *builder, R1u range, char c);
bool string_builder_insert_char(StringBuilder *builder, usize index, char c);
bool string_builder_insert_str(StringBuilder *builder, usize index, const char *s);
bool string_builder_insert_fmt(StringBuilder *builder, usize index, const char *fmt, ...);
bool string_builder_insert_view(StringBuilder *builder, usize index, Strview v);
bool string_builder_remove(StringBuilder *builder, R1u range);
const char *string_builder_cstr(const StringBuilder *builder);
char string_builder_get_char(const StringBuilder *builder, usize index);
Strview string_builder_get_view(const StringBuilder *builder, R1u range);
usize string_builder_len(const StringBuilder *builder);
