//
// Created by scott on 9/15/2023.
//

#include "shmo/window.h"
#include <SDL.h>

static volatile size_t count = 0;

window_t window_create(const char *title, v2i_t position, v2i_t size) {

    void *handle = SDL_CreateWindow(title,
                                   position.x,
                                   position.y,
                                   size.x,
                                   size.y,
                                   SDL_WINDOW_OPENGL);
    if (handle) {
        ++count;
        return (window_t) { .handle = handle };
    }
    return (window_t) { .handle = nullptr };
}

void window_destroy(window_t *w) {
    assert(w);
    if (!w->handle) {
        return;
    }
    --count;
    SDL_DestroyWindow(w->handle);
    w->handle = nullptr;
}

size_t window_count(void) {
    return count;
}