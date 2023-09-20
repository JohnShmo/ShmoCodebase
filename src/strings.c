//
// Created by scott on 9/13/2023.
//

#include <stdlib.h>
#include "shmo/strings.h"

strview_t strview(const char *data, usize length) {
    assert(data);
    return (strview_t) { .data = data, .length = length };
}

strview_t strview_of(const char *cstr) {
    assert(cstr);
    return (strview_t) { .data = cstr, .length = strlen(cstr) };
}

#define CSTR_BUFFER_SIZE (1024ULL * 8ULL)
static char cstr_buffer[CSTR_BUFFER_SIZE];

const char *strview_cstr(const strview_t *view) {
    assert(view);
    strview_cpy(view, cstr_buffer, CSTR_BUFFER_SIZE);
    return cstr_buffer;
}

char *strview_dup(const strview_t *view, heap_allocator_t *allocator) {
    assert(view);
    if (!allocator) {
        allocator = stdalloc;
    }

    char *buffer = heap_malloc(allocator, view->length + 1);
    assert(buffer);
    memcpy(buffer, view->data, view->length);
    buffer[view->length] = '\0';
    return buffer;
}

void strview_cpy(const strview_t *view, char *dest, usize dest_size) {
    assert(view);
    if (!dest || dest_size == 0) {
        return;
    }

    usize len = min(dest_size - 1, view->length);

    memcpy(dest, view->data, len);
    dest[len] = '\0';
}

usize strview_cat(const strview_t *lhs, const strview_t *rhs, char *dest, usize dest_size) {
    assert(lhs);
    assert(rhs);

    usize result = lhs->length + rhs->length;
    if (!dest || dest_size == 0) {
        return result;
    }

    strview_cpy(lhs, dest, dest_size);
    if (dest_size > lhs->length) {
        strview_cpy(rhs, dest + lhs->length, dest_size - lhs->length);
    }
    return result;
}

const char *strview_data(const strview_t *view) {
    assert(view);
    return view->data;
}

usize strview_len(const strview_t *view) {
    assert(view);
    return view->length;
}