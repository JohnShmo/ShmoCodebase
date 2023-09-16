//
// Created by scott on 9/15/2023.
//

#include "shmo/app.h"
#include "shmo/window.h"
#include "shmo/user_input.h"
#include <SDL.h>

static volatile bool should_close = false;

bool app_open(void) {
    return SDL_Init(SDL_INIT_EVERYTHING) == 0;
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
    return should_close || window_count() == 0;
}

void app_close(void) {
    SDL_Quit();
}