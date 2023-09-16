//
// Created by scott on 9/15/2023.
//

#ifndef SHMOCODEBASE_WINDOW_H
#define SHMOCODEBASE_WINDOW_H

#include "app.h"

typedef struct window_t {
    void *handle;
} window_t;

#define WINDOWPOS_CENTERED (0x2FFF0000u | (0))

window_t window_create(const char *title, v2i_t position, v2i_t size);
void window_destroy(window_t *w);
size_t window_count(void);

#endif //SHMOCODEBASE_WINDOW_H
