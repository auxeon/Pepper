/*
@author Abhikalp Unakal
@date   08 june 2021
@file   ps_graphics.h
@desc   pepper engine graphics api
*/

#ifndef PS_GRAPHICS_H
#define PS_GRAPHICS_H

#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "../lib/glad/include/glad/glad.h"
#include "../lib/glfw/include/GLFW/glfw3.h"

typedef union ps_graphics_impl ps_graphics_impl;
typedef struct ps_graphics_window ps_graphics_window;

void    ps_graphics_init(ps_graphics_window* window, const char* title, int width, int height);
void    ps_graphics_destroy(ps_graphics_window* window);
void    ps_graphics_window_poll_events(ps_graphics_window* window);
int     ps_graphics_window_should_close(ps_graphics_window* window);
void    ps_graphics_window_swap_buffers(ps_graphics_window* window);
ps_graphics_window* ps_graphics_get_window();
void ps_graphics_release_window(ps_graphics_window* window);

#endif 