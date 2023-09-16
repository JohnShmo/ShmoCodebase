#include "shmo/user_input.h"
#include <stdio.h>

#include <SDL.h>

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1080, 720, SDL_WINDOW_OPENGL);

    while(true) {
        SDL_Event event;
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }
    }

    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}