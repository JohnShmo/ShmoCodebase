#include "shmo/user_input.h"
#include "shmo/app.h"
#include <stdio.h>
#include <stdlib.h>

int main(i32 argc, char *argv[]) {
    if (!app_open("test1",
                  v2i(APP_POS_CENTERED, APP_POS_CENTERED),
                  v2i(1080, 720))) {
        return EXIT_FAILURE;
    }

    while(!app_should_close()) {
        app_poll_events();
    }

    app_close();

    return EXIT_SUCCESS;
}