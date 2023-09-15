//
// Created by scott on 9/15/2023.
//

#ifndef SHMOCODEBASE_USER_INPUT_H
#define SHMOCODEBASE_USER_INPUT_H

#include "base.h"

#ifndef DONT_DEFINE_KEY_CODE_ENUM
typedef enum key_code_t {
    VK_LBUTTON      = 0x01,
    VK_RBUTTON      = 0x02,
    VK_CANCEL       = 0x03,
    VK_MBUTTON      = 0x04,
    VK_XBUTTON1     = 0x05,
    VK_XBUTTON2     = 0x06,

    VK_BACK         = 0x08,
    VK_TAB          = 0x09,

    VK_CLEAR        = 0x0C,
    VK_RETURN       = 0x0D,

    VK_SHIFT        = 0x10,
    VK_CONTROL      = 0x11,
    VK_MENU         = 0x12,
    VK_PAUSE        = 0x13,
    VK_CAPITAL      = 0x14,

    VK_ESCAPE       = 0x1B,

    VK_SPACE        = 0x20,
    VK_PRIOR        = 0x21,
    VK_NEXT         = 0x22,
    VK_END          = 0x23,
    VK_HOME         = 0x24,
    VK_LEFT         = 0x25,
    VK_RIGHT        = 0x26,
    VK_UP           = 0x27,
    VK_DOWN         = 0x28,
    VK_SELECT       = 0x29,
    VK_PRINT        = 0x2A,
    VK_EXECUTE      = 0x2B,
    VK_SNAPSHOT     = 0x2C,
    VK_INSERT       = 0x2D,
    VK_DELETE       = 0x2E,
    VK_HELP         = 0x2F,

    // ... ASCII VALUES

    VK_SLEEP        = 0x5F,
    VK_NUMPAD0      = 0x60,
    VK_NUMPAD1      = 0x61,
    VK_NUMPAD2      = 0x62,
    VK_NUMPAD3      = 0x63,
    VK_NUMPAD4      = 0x64,
    VK_NUMPAD5      = 0x65,
    VK_NUMPAD6      = 0x66,
    VK_NUMPAD7      = 0x67,
    VK_NUMPAD8      = 0x68,
    VK_NUMPAD9      = 0x69,
    VK_MULTIPLY     = 0x6A,
    VK_ADD          = 0x6B,
    VK_SEPARATOR    = 0x6C,
    VK_SUBTRACT     = 0x6D,
    VK_DECIMAL      = 0x6E,
    VK_DIVIDE       = 0x6F,
    VK_F1           = 0x70,
    VK_F2           = 0x71,
    VK_F3           = 0x72,
    VK_F4           = 0x73,
    VK_F5           = 0x74,
    VK_F6           = 0x75,
    VK_F7           = 0x76,
    VK_F8           = 0x77,
    VK_F9           = 0x78,
    VK_F10          = 0x79,
    VK_F11          = 0x7A,
    VK_F12          = 0x7B,
    VK_F13          = 0x7C,
    VK_F14          = 0x7D,
    VK_F15          = 0x7E,
    VK_F16          = 0x7F,
    VK_F17          = 0x80,
    VK_F18          = 0x81,
    VK_F19          = 0x82,
    VK_F20          = 0x83,
    VK_F21          = 0x84,
    VK_F22          = 0x85,
    VK_F23          = 0x86,
    VK_F24          = 0x87,

    VK_NUMLOCK      = 0x90,
    VK_SCROLL       = 0x91,

    VK_LSHIFT       = 0xA0,
    VK_RSHIFT       = 0xA1,
    VK_LCONTROL     = 0xA2,
    VK_RCONTROL     = 0xA3,
    VK_LMENU        = 0xA4,
    VK_RMENU        = 0xA5,

    VK_OEM_1        = 0xBA, // ;: key
    VK_OEM_PLUS     = 0xBB,
    VK_OEM_COMMA    = 0xBC,
    VK_OEM_MINUS    = 0xBD,
    VK_OEM_PERIOD   = 0xBE,
    VK_OEM_2        = 0xBF, // /? key
    VK_OEM_3        = 0xC0, // `~ key

    VK_OEM_4        = 0xDB, // [{ key
    VK_OEM_5        = 0xDC, // \| key
    VK_OEM_6        = 0xDD, // ]} key
    VK_OEM_7        = 0xDE, // '" key
    VK_OEM_8        = 0xDF, // misc

    COUNT_VK        = 0x100
} key_code_t;

#define VK_SEMICOLON VK_OEM_1
#define VK_FSLASH    VK_OEM_2
#define VK_TILDA     VK_OEM_3
#define VK_LBRACKET  VK_OEM_4
#define VK_BSLASH    VK_OEM_5
#define VK_RBRACKET  VK_OEM_6
#define VK_QUOTE     VK_OEM_7
#else
typedef enum key_code_t {
    COUNT_VK = 0x100
} key_code_t;
#endif

#define KEY_DOWN_MASK     0x01
#define KEY_PRESSED_MASK  0x02
#define KEY_RELEASED_MASK 0x04
#define KEY_REPEATED_MASK 0x08

bool keyboard_check(u8 vk_code);
bool keyboard_check_pressed(u8 vk_code);
bool keyboard_check_released(u8 vk_code);
bool keyboard_check_repeated(u8 vk_code);
u8 keyboard_lastkey(void);

void keyboard_update(void);

#endif //SHMOCODEBASE_USER_INPUT_H
