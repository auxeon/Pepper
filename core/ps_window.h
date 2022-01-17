/**
 @author Abhikalp Unakal
 @date 16 jan 2022
 @file ps_window.h
 @brief pepper engine window abstraction api
*/

#ifndef PS_WINDOW_H
#define PS_WINDOW_H

#include "stdbool.h"
#include "ps_keycodes.h"
#include "../ps_types.h"

typedef struct ps_window ps_window;
typedef struct ps_mouse_t ps_mouse_t;
typedef struct ps_keyboard_t ps_keyboard_t;
typedef struct ps_input_t ps_input_t;

ps_window*          ps_window_get_handle(); // get the graphics window handle
void                ps_window_init(ps_window* window, const char* title, int width, int height); // initialize window context
void                ps_window_set_title(ps_window* window, const char* buffer); // set the window title
void                ps_window_poll_events(ps_window* window); // refresh underlying events for the window
int                 ps_window_should_close(ps_window* window); // is the window signals to close
void                ps_window_swap_buffers(ps_window* window); // swap the render buffers of current window
void                ps_window_release(ps_window* window); // release any acquired resources
void                ps_window_destroy(ps_window* window); // destroy the window and its context and stop processing events
ps_vec2             ps_window_screen_get_size(ps_window* window); // get current screen w,h

void                ps_window_input_update(ps_window* window); // update input events state

int                 ps_window_input_keyboard_get_max_keys(ps_window* window); // get max keys on keyboard
bool                ps_window_input_keyboard_ispressed(ps_window* window, ps_keyboard_keycodes keycode); // key pressed -> pressed
bool                ps_window_input_keyboard_isreleased(ps_window* window, ps_keyboard_keycodes keycode); // key released -> released
bool                ps_window_input_keyboard_istriggered_down(ps_window* window, ps_keyboard_keycodes keycode); // key released -> pressed
bool                ps_window_input_keyboard_istriggered_up(ps_window* window, ps_keyboard_keycodes keycode); // key pressed -> released

int                 ps_window_input_mouse_get_max_keys(ps_window* window); // get max keys on mouse
ps_vec2             ps_window_input_mouse_get_position(ps_window* window); // get mouse position (0.0,0.0) top left
void                ps_window_input_mouse_set_position(ps_window* window, ps_vec2 position); // set mouse position (0.0,0.0) top left
bool                ps_window_input_mouse_ispressed(ps_window* window, ps_mouse_keycodes keycode); // mouse key pressed -> pressed
bool                ps_window_input_mouse_isreleased(ps_window* window, ps_mouse_keycodes keycode); // mouse key released -> released
bool                ps_window_input_mouse_istriggered_down(ps_window* window, ps_mouse_keycodes keycode); // mouse key released -> pressed
bool                ps_window_input_mouse_istriggered_up(ps_window* window, ps_mouse_keycodes keycode); // mouse key pressed -> released

#endif 