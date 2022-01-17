/**
 @author Abhikalp Unakal
 @date 16 jan 
 @file ps_keycodes.h
 @brief keycodes for input devices
*/

#ifndef PS_KEYCODES_H
#define PS_KEYCODES_H

#define GLFW
#include "../ps_defines.h"

#ifdef GLFW
typedef enum ps_keystates{
  PS_ACTION_PRESS = GLFW_PRESS,
  PS_ACTION_RELEASE = GLFW_RELEASE
}ps_keystates;

typedef enum ps_keyboard_keycodes{
  PS_KEY_UNKNOWN    = GLFW_KEY_UNKNOWN,
  PS_KEY_SPACE    = GLFW_KEY_SPACE,
  PS_KEY_APOSTROPHE = GLFW_KEY_APOSTROPHE,
  PS_KEY_COMMA = GLFW_KEY_COMMA,
  PS_KEY_MINUS = GLFW_KEY_MINUS,
  PS_KEY_PERIOD = GLFW_KEY_PERIOD,
  PS_KEY_SLASH = GLFW_KEY_SLASH,
  PS_KEY_0    = GLFW_KEY_0,
  PS_KEY_1    = GLFW_KEY_1,
  PS_KEY_2    = GLFW_KEY_2,
  PS_KEY_3    = GLFW_KEY_3,
  PS_KEY_4    = GLFW_KEY_4,
  PS_KEY_5    = GLFW_KEY_5,
  PS_KEY_6    = GLFW_KEY_6,
  PS_KEY_7    = GLFW_KEY_7,
  PS_KEY_8    = GLFW_KEY_8,
  PS_KEY_9    = GLFW_KEY_9,
  PS_KEY_SEMICOLON = GLFW_KEY_SEMICOLON,
  PS_KEY_EQUAL = GLFW_KEY_EQUAL,
  PS_KEY_A    = GLFW_KEY_A,
  PS_KEY_B    = GLFW_KEY_B,
  PS_KEY_C    = GLFW_KEY_C,
  PS_KEY_D    = GLFW_KEY_D,
  PS_KEY_E    = GLFW_KEY_E,
  PS_KEY_F    = GLFW_KEY_F,
  PS_KEY_G    = GLFW_KEY_G,
  PS_KEY_H    = GLFW_KEY_H,
  PS_KEY_I    = GLFW_KEY_I,
  PS_KEY_J    = GLFW_KEY_J,
  PS_KEY_K    = GLFW_KEY_K,
  PS_KEY_L    = GLFW_KEY_L,
  PS_KEY_M    = GLFW_KEY_M,
  PS_KEY_N    = GLFW_KEY_N,
  PS_KEY_O    = GLFW_KEY_O,
  PS_KEY_P    = GLFW_KEY_P,
  PS_KEY_Q    = GLFW_KEY_Q,
  PS_KEY_R    = GLFW_KEY_R,
  PS_KEY_S    = GLFW_KEY_S,
  PS_KEY_T    = GLFW_KEY_T,
  PS_KEY_U    = GLFW_KEY_U,
  PS_KEY_V    = GLFW_KEY_V,
  PS_KEY_W    = GLFW_KEY_W,
  PS_KEY_X    = GLFW_KEY_X,
  PS_KEY_Y    = GLFW_KEY_Y,
  PS_KEY_Z    = GLFW_KEY_Z,
  PS_KEY_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET,
  PS_KEY_BACKSLASH = GLFW_KEY_BACKSLASH,
  PS_KEY_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET,
  PS_KEY_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,
  PS_KEY_WORLD_1 = GLFW_KEY_WORLD_1,
  PS_KEY_WORLD_2 = GLFW_KEY_WORLD_2,
  PS_KEY_ESCAPE    = GLFW_KEY_ESCAPE   ,
  PS_KEY_ENTER    = GLFW_KEY_ENTER   ,
  PS_KEY_TAB    = GLFW_KEY_TAB   ,
  PS_KEY_BACKSPACE    = GLFW_KEY_BACKSPACE   ,
  PS_KEY_INSERT    = GLFW_KEY_INSERT   ,
  PS_KEY_DELETE    = GLFW_KEY_DELETE   ,
  PS_KEY_RIGHT    = GLFW_KEY_RIGHT   ,
  PS_KEY_LEFT    = GLFW_KEY_LEFT   ,
  PS_KEY_DOWN    = GLFW_KEY_DOWN   ,
  PS_KEY_UP    = GLFW_KEY_UP   ,
  PS_KEY_PAGE_UP    = GLFW_KEY_PAGE_UP   ,
  PS_KEY_PAGE_DOWN    = GLFW_KEY_PAGE_DOWN   ,
  PS_KEY_HOME    = GLFW_KEY_HOME   ,
  PS_KEY_END    = GLFW_KEY_END   ,
  PS_KEY_CAPS_LOCK    = GLFW_KEY_CAPS_LOCK   ,
  PS_KEY_SCROLL_LOCK    = GLFW_KEY_SCROLL_LOCK   ,
  PS_KEY_NUM_LOCK    = GLFW_KEY_NUM_LOCK   ,
  PS_KEY_PRINT_SCREEN    = GLFW_KEY_PRINT_SCREEN   ,
  PS_KEY_PAUSE    = GLFW_KEY_PAUSE   ,
  PS_KEY_F1    = GLFW_KEY_F1,
  PS_KEY_F2    = GLFW_KEY_F2,
  PS_KEY_F3    = GLFW_KEY_F3,
  PS_KEY_F4    = GLFW_KEY_F4,
  PS_KEY_F5    = GLFW_KEY_F5,
  PS_KEY_F6    = GLFW_KEY_F6,
  PS_KEY_F7    = GLFW_KEY_F7,
  PS_KEY_F8    = GLFW_KEY_F8,
  PS_KEY_F9    = GLFW_KEY_F9,
  PS_KEY_F10    = GLFW_KEY_F10,
  PS_KEY_F11    = GLFW_KEY_F11,
  PS_KEY_F12    = GLFW_KEY_F12,
  PS_KEY_F13    = GLFW_KEY_F13,
  PS_KEY_F14    = GLFW_KEY_F14,
  PS_KEY_F15    = GLFW_KEY_F15,
  PS_KEY_F16    = GLFW_KEY_F16,
  PS_KEY_F17    = GLFW_KEY_F17,
  PS_KEY_F18    = GLFW_KEY_F18,
  PS_KEY_F19    = GLFW_KEY_F19,
  PS_KEY_F20    = GLFW_KEY_F20,
  PS_KEY_F21    = GLFW_KEY_F21,
  PS_KEY_F22    = GLFW_KEY_F22,
  PS_KEY_F23    = GLFW_KEY_F23,
  PS_KEY_F24    = GLFW_KEY_F24,
  PS_KEY_F25    = GLFW_KEY_F25,
  PS_KEY_KP_0    = GLFW_KEY_KP_0,
  PS_KEY_KP_1    = GLFW_KEY_KP_1,
  PS_KEY_KP_2    = GLFW_KEY_KP_2,
  PS_KEY_KP_3    = GLFW_KEY_KP_3,
  PS_KEY_KP_4    = GLFW_KEY_KP_4,
  PS_KEY_KP_5    = GLFW_KEY_KP_5,
  PS_KEY_KP_6    = GLFW_KEY_KP_6,
  PS_KEY_KP_7    = GLFW_KEY_KP_7,
  PS_KEY_KP_8    = GLFW_KEY_KP_8,
  PS_KEY_KP_9    = GLFW_KEY_KP_9,
  PS_KEY_KP_DECIMAL    = GLFW_KEY_KP_DECIMAL,
  PS_KEY_KP_DIVIDE    = GLFW_KEY_KP_DIVIDE,
  PS_KEY_KP_MULTIPLY    = GLFW_KEY_KP_MULTIPLY,
  PS_KEY_KP_SUBTRACT    = GLFW_KEY_KP_SUBTRACT,
  PS_KEY_KP_ADD    = GLFW_KEY_KP_ADD,
  PS_KEY_KP_ENTER    = GLFW_KEY_KP_ENTER,
  PS_KEY_KP_EQUAL    = GLFW_KEY_KP_EQUAL,
  PS_KEY_LEFT_SHIFT    = GLFW_KEY_LEFT_SHIFT,
  PS_KEY_LEFT_CONTROL    = GLFW_KEY_LEFT_CONTROL,
  PS_KEY_LEFT_ALT    = GLFW_KEY_LEFT_ALT,
  PS_KEY_LEFT_SUPER    = GLFW_KEY_LEFT_SUPER,
  PS_KEY_RIGHT_SHIFT    = GLFW_KEY_RIGHT_SHIFT,
  PS_KEY_RIGHT_CONTROL    = GLFW_KEY_RIGHT_CONTROL,
  PS_KEY_RIGHT_ALT    = GLFW_KEY_RIGHT_ALT,
  PS_KEY_RIGHT_SUPER    = GLFW_KEY_RIGHT_SUPER,
  PS_KEY_MENU    = GLFW_KEY_MENU,
  PS_KEY_LAST = GLFW_KEY_LAST,
}ps_keyboard_keycodes;

typedef enum ps_mouse_keycodes{
  PS_MOUSE_BUTTON_1 = GLFW_MOUSE_BUTTON_1,
  PS_MOUSE_BUTTON_2 = GLFW_MOUSE_BUTTON_2,
  PS_MOUSE_BUTTON_3 = GLFW_MOUSE_BUTTON_3,
  PS_MOUSE_BUTTON_4 = GLFW_MOUSE_BUTTON_4,
  PS_MOUSE_BUTTON_5 = GLFW_MOUSE_BUTTON_5,
  PS_MOUSE_BUTTON_6 = GLFW_MOUSE_BUTTON_6,
  PS_MOUSE_BUTTON_7 = GLFW_MOUSE_BUTTON_7,
  PS_MOUSE_BUTTON_8 = GLFW_MOUSE_BUTTON_8,
  PS_MOUSE_BUTTON_LAST = GLFW_MOUSE_BUTTON_LAST,
  PS_MOUSE_BUTTON_LEFT = GLFW_MOUSE_BUTTON_LEFT,
  PS_MOUSE_BUTTON_RIGHT = GLFW_MOUSE_BUTTON_RIGHT,
  PS_MOUSE_BUTTON_MIDDLE = GLFW_MOUSE_BUTTON_MIDDLE
}ps_mouse_keycodes;

#endif
#endif
