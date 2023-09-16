//
// Created by scott on 9/15/2023.
//

#include "shmo/app.h"
#include "shmo/user_input.h"
#include <SDL.h>

static volatile bool should_close = false;
static SDL_Window *window = nullptr;

bool app_open(const char *title, v2i_t position, v2i_t size) {
    bool init = SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) == 0;
    if (!init) {
        return false;
    }
    window = SDL_CreateWindow(title, position.x, position.y, size.x, size.y, SDL_WINDOW_OPENGL);
    if (!window) {
        SDL_Quit();
        return false;
    }
    return true;
}

void app_poll_events(void) {
    SDL_Event e;
    while (SDL_PollEvent(&e) && !should_close) {
        switch (e.type) {
            case SDL_QUIT:
                should_close = true;
                break;

            default:
                break;
        }
    }
}

bool app_should_close(void) {
    return should_close;
}

void app_close(void) {
    if (window) {
        SDL_DestroyWindow(window);
    }
    window = nullptr;
    SDL_Quit();
}