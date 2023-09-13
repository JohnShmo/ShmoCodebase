//
// Created by scott on 9/13/2023.
//

#include "shmo/strings.h"

void string_create(string_t *dest, const char *cstr, heap_allocator_t *allocator) {
    assert(dest);
    assert(cstr);
    if (!allocator) {
        allocator = stdalloc;
    }
    string_destroy(dest);

    size_t len = strlen(cstr);

    dest->data = heap_malloc(allocator, len ? len : 1);
    assert(dest->data);
    dest->length = len;

    memcpy(dest->data, cstr, len);
}

void string_copy(string_t *restrict dest, const string_t *restrict src, heap_allocator_t *allocator) {
    assert(dest);
    assert(src);
    strview_t view = strview_of(src);
    string_copy_view(dest, &view, allocator);
}

void string_copy_view(string_t *restrict dest, const strview_t *restrict src, heap_allocator_t *allocator) {
    assert(dest);
    assert(src);
    if (!allocator) {
        allocator = stdalloc;
    }
    string_destroy(dest);

    size_t len = src->length;

    dest->data = heap_malloc(allocator, len ? len : 1);
    assert(dest->data);
    dest->length = len;

    memcpy(dest->data, src->data, len);
}

void string_destroy(string_t *str) {
    assert(str);
    if (!str->allocator) {
        return;
    }
    heap_free(str->allocator, str->data);
    str->data = nullptr;
    str->length = 0;
    str->allocator = nullptr;
}

const char *string_cstr(const string_t *str) {
    assert(str);
    strview_t view = strview_of(str);
    return strview_cstr(&view);
}

void string_cstr_to(const string_t *str, char *dest, size_t dest_size) {
    assert(str);
    assert(dest);
    assert(dest_size);
    strview_t view = strview_of(str);
    strview_cstr_to(&view, dest, dest_size);
}

char *string_data(string_t *str) {
    assert(str);
    return str->data;
}

const char *string_const_data(const string_t *str) {
    assert(str);
    return str->data;
}

size_t string_length(const string_t *str) {
    assert(str);
    return str->length;
}

strview_t strview(const char *data, size_t length) {
    assert(data);
    return (strview_t) { .data = data, .length = length };
}

strview_t strview_of(const string_t *str)  {
    assert(str);
    return (strview_t) { .data = str->data, .length = str->length };
}

strview_t strview_of_cstr(const char *cstr) {
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
    strview_cstr_to(view, buffer, buffer_size);

    return buffer;
}

void strview_cstr_to(const strview_t *view, char *dest, size_t dest_size) {
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

size_t strview_length(const strview_t *view) {
    assert(view);
    return view->length;
}