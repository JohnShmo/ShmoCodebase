#include "shmo/user_input.h"
#include <stdio.h>

int main() {

    while (true) {
        keyboard_update();
        if (keyboard_check_pressed(VK_SPACE)) {
            puts("Pressed Space!\n");
        }
        if (keyboard_check_pressed('A')) {
            puts("Pressed 'A'!\n");
        }
        if (keyboard_check_released('A')) {
            puts("Released 'A'!\n");
        }
        if (keyboard_check_repeated(VK_SPACE)) {
            puts("Repeated Space!\n");
        }
        if (keyboard_check_pressed(VK_ESCAPE)) {
            break;
        }
    }

    return 0;
}