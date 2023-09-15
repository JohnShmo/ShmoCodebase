//
// Created by scott on 9/15/2023.
//

#include "shmo/base.h"

#if OS_WINDOWS == 1
#define DONT_DEFINE_KEY_CODE_ENUM
#include "shmo/user_input.h"
#include <windows.h>
#include <winuser.h>

volatile static u8 key_states[COUNT_VK] = { 0 };
volatile static u8 last_pressed_key = 0;

bool keyboard_check(u8 vk_code) {
    return (key_states[vk_code] & KEY_DOWN_MASK) != 0;
}

bool keyboard_check_pressed(u8 vk_code) {
    return (key_states[vk_code] & KEY_PRESSED_MASK) != 0;
}

bool keyboard_check_released(u8 vk_code) {
    return (key_states[vk_code] & KEY_RELEASED_MASK) != 0;
}

bool keyboard_check_repeated(u8 vk_code) {
    return (key_states[vk_code] & KEY_REPEATED_MASK) != 0;
}

void keyboard_update(void) {
    last_pressed_key = 0;
    for (i32 i = 0; i < COUNT_VK; ++i) {
        i32 scan_result = GetAsyncKeyState(i);
        i32 scan_repeat = scan_result & 0x01;
        i32 scan_down = scan_result & 0x8000;
        u8 key_state = key_states[i];
        key_state &= ~KEY_PRESSED_MASK;
        key_state &= ~KEY_RELEASED_MASK;
        key_state &= ~KEY_REPEATED_MASK;
        if (scan_down != 0) {
            if (!(key_state & KEY_DOWN_MASK)) {
                key_state |= KEY_PRESSED_MASK;
                last_pressed_key = (u8)i;
            } else if (scan_repeat != 0) {
                key_state |= KEY_REPEATED_MASK;
                last_pressed_key = (u8)i;
            }
            key_state |= KEY_DOWN_MASK;
        } else {
            if (key_state & KEY_DOWN_MASK) {
                key_state |= KEY_RELEASED_MASK;
            }
            key_state &= ~KEY_DOWN_MASK;
        }
        key_states[i] = key_state;
    }
}

u8 keyboard_lastkey(void) {
    return last_pressed_key;
}
#endif // OS_WINDOWS