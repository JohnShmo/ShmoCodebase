//
// Created by scott on 9/13/2023.
//

#include <stdlib.h>
#include "shmo/strings.h"

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
