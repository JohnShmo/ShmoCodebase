//
// Created by scott on 9/15/2023.
//

#include "shmo/base.h"

#include "shmo/user_input.h"

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

u8 keyboard_lastkey(void) {
    return last_pressed_key;
}

void keyboard_key_update(u8 vk_code) {
    key_states[vk_code] &= ~KEY_PRESSED_MASK;
    key_states[vk_code] &= ~KEY_RELEASED_MASK;
    key_states[vk_code] &= ~KEY_REPEATED_MASK;
}

void keyboard_key_clear(u8 vk_code) {
    key_states[vk_code] = 0;
}

void keyboard_key_press(u8 vk_code) {
    key_states[vk_code] |= KEY_PRESSED_MASK;
    key_states[vk_code] |= KEY_DOWN_MASK;

    if (vk_code > VK_XBUTTON2) {
        last_pressed_key = vk_code;
    }
}

void keyboard_key_release(u8 vk_code) {
    key_states[vk_code] |= KEY_RELEASED_MASK;
    key_states[vk_code] &= ~KEY_DOWN_MASK;
}

void keyboard_key_repeat(u8 vk_code) {
    key_states[vk_code] |= KEY_REPEATED_MASK;
}