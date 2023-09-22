//
// Created by scott on 9/13/2023.
//

#include <stdlib.h>
#include "shmo/strings.h"

Strview strview(const char *data, usize length) {
    assert(data);
    return (Strview) { .data = data, .length = length };
}

Strview strview_of(const char *cstr) {
    assert(cstr);
    return (Strview) { .data = cstr, .length = strlen(cstr) };
}

#define CSTR_BUFFER_SIZE (1024ULL * 8ULL)
static char cstr_buffer[CSTR_BUFFER_SIZE];

const char *strview_cstr(Strview view) {
    strview_cpy(view, cstr_buffer, CSTR_BUFFER_SIZE);
    return cstr_buffer;
}

char *strview_dup(Strview view, HeapAllocator *allocator) {;
    if (!allocator) {
        allocator = stdalloc;
    }

    char *buffer = heap_malloc(allocator, view.length + 1);
    assert(buffer);
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

usize strview_join(Strview *views, usize views_count, Strview sep, char *dest, usize dest_size) {
    // TODO: Fix this broken-ass function
    usize result = 0;
    usize write_index = 0;

    if (views_count == 0) {
        return result;
    }

    if (views_count == 1) {
        result = views[0].length;
        if (dest && dest_size) {
            strview_cpy(views[0], dest, dest_size);
        }
        return result;
    }

    for (usize i = 1; i < views_count; ++i) {
        result += strview_cat(views[i-1], views[i], dest + write_index, dest_size > write_index ? dest_size - write_index : 0);
        write_index += views[i-1].length;
        if (i < views_count - 1) {
            result += strview_cat(views[i], sep, dest + write_index, dest_size > write_index ? dest_size - write_index : 0);
            write_index += sep.length;
        }
    }

    return result;
}

const char *strview_data(Strview view) {
    return view.data;
}

usize strview_len(Strview view) {
    return view.length;
}
