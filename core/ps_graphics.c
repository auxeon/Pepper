/*
@author Abhikalp Unakal
@date   08 june 2021
@file   ps_engine.c
@desc   pepper engine api wrapper for future
*/

#include "ps_graphics.h"

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#define GLFW

typedef union ps_graphics_impl{
    GLFWwindow* handle;
}ps_graphics_impl;

typedef struct ps_graphics_window{
    char title[80];
    int width;
    int height;
    ps_graphics_impl window;
}ps_graphics_window;

void ps_graphics_init(ps_graphics_window* window, const char* title, int width, int height){

    #ifdef GLFW
    if(!glfwInit()){
        fprintf(stderr, "%s : %s : line %d : failed to initialize glfw\n",__FILE__, __FUNCTION__, __LINE__);
        exit(-1);
    }

    strcpy(window->title,title);
    window->width = width;
    window->height = height;

    // create window
    window->window.handle = glfwCreateWindow(window->width,window->height,window->title,NULL,NULL);
    if(!window->window.handle){
        fprintf(stderr, "%s : %s : line %d : failed to create glfw window\n",__FILE__, __FUNCTION__, __LINE__);
        exit(-1);  
    }
    // setup context
    glfwMakeContextCurrent(window->window.handle);

    // load opengl functions
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        fprintf(stderr, "%s : %s : line %d : failed to initialize OpenGL context\n",__FILE__, __FUNCTION__, __LINE__);
        exit(-1);
    }
    #endif
}

int ps_graphics_window_should_close(ps_graphics_window* window){
    #ifdef GLFW
    return glfwWindowShouldClose(window->window.handle);
    #endif
}

void ps_graphics_window_swap_buffers(ps_graphics_window* window){
    #ifdef GLFW
    glfwSwapBuffers(window->window.handle);
    #endif
}

void ps_graphics_window_poll_events(ps_graphics_window* window){
    #ifdef GLFW
    glfwPollEvents();
    #endif
}

void ps_graphics_destroy(ps_graphics_window* window){
    #ifdef GLFW
    glfwDestroyWindow(window->window.handle);
    glfwTerminate();
    #endif
}