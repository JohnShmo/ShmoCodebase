//
// Created by scott on 9/13/2023.
//

#include "shmo/strings.h"

strview_t strview(const char *data, size_t length) {
    assert(data);
    return (strview_t) { .data = data, .length = length };
}

strview_t strview_of(const char *cstr) {
    assert(cstr);
    return (strview_t) { .data = cstr, .length = strlen(cstr) };
}

const char *strview_cstr(const strview_t *view) {
    static char *buffer = nullptr;
    static size_t buffer_size = 0;

    assert(view);

    if (!buffer || buffer_size < view->length + 1) {
        if (buffer) {
            heap_free(stdalloc, buffer);
        }
        buffer = heap_malloc(stdalloc, view->length + 1);
        assert(buffer);
        buffer_size = view->length + 1;
    }
    strview_cpy(view, buffer, buffer_size);

    return buffer;
}

const char *strview_dup(const strview_t *view, heap_allocator_t *allocator) {
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

void strview_cpy(const strview_t *view, char *dest, size_t dest_size) {
    assert(view);
    assert(dest);
    assert(dest_size);

    size_t len = min(dest_size - 1, view->length);

    memcpy(dest, view->data, len);
    dest[len] = '\0';
}

const char *strview_data(const strview_t *view) {
    assert(view);
    return view->data;
}

size_t strview_len(const strview_t *view) {
    assert(view);
    return view->length;
}