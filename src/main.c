#include "shmo/user_input.h"
#include "shmo/window.h"
#include <stdio.h>
#include <stdlib.h>

int main(i32 argc, char *argv[]) {
    if (!app_open()) {
        return EXIT_FAILURE;
    }
    window_t window = window_create("test",
                                    v2i(WINDOWPOS_CENTERED, WINDOWPOS_CENTERED),
                                    v2i(1080, 720));
    while(!app_should_close()) {
        app_poll_events();
    }
    window_destroy(&window);
    app_close();

    return EXIT_SUCCESS;
}