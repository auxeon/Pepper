#include <pepper.h>
/**
 @author Abhikalp Unakal
 @date 08 june 2021
 @file ps_chronon.c
 @brief wall clock time realted utilities
*/

#if defined(_WIN32) || defined(_WIN64)
#define _CRT_SECURE_NO_WARNINGS
#include "Windows.h"
#elif defined(__APPLE__)
#include "mach/mach_time.h"
#endif

#include <stdbool.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include "ps_chronon.h"
#include "ps_logging.h"
#include "ps_vector.h"

ps_vector_declare(uint64_t);

typedef struct ps_clock_data{
    ps_vector_uint64_t timers_t0;
    uint64_t dawn;
    bool started;
    double dt;
    uint64_t freq;
}ps_clock_data;
/*
    accurate to milliseconds resolution
*/
uint64_t ps_clock_now(ps_clock_data* c) {
#if defined(_WIN32) || defined(_WIN64)
    uint64_t ticks;
    /* prefer the QueryPerformance counter API if exists*/
    if(QueryPerformanceCounter((LARGE_INTEGER*)&ticks)){
        return ticks;
    }
    else{
        /* windows uses _timeb and _ftime instead of timeb and ftime */
        struct _timeb timebuffer;
        /*_timeb has a max resolution of 1ms*/
        _ftime(&timebuffer);
        return (uint64_t)(((timebuffer.time * c->freq) + timebuffer.millitm));
    }
#elif defined(__APPLE__)
    return mach_absolute_time();
#else
    struct timeb timebuffer;
    ftime(&timebuffer);
    return (uint64_t)(((timebuffer.time * c->freq) + timebuffer.millitm));
#endif
}

ps_clock_data* ps_clock_get(){
    ps_clock_data* clock = (ps_clock_data*)malloc(sizeof(ps_clock_data));
    if (clock) {
        ps_vector_create(clock->timers_t0, uint64_t);
        clock->started = false;
        clock->dt = 0.0;
#if defined(_WIN32) || defined(_WIN64)
        uint64_t freq;
        if(QueryPerformanceFrequency((LARGE_INTEGER*)&freq)){
            clock->freq = freq;
        }
#elif defined(__APPLE__)
        mach_timebase_info_data_t info_data;
        mach_timebase_info(&info_data);
        /*beware info_data is a struct of uint32_t numer, denom - division result will be double but casting it to uint64_t*/
        clock->freq = (uint64_t)((info_data.denom * 1e9) / info_data.numer);

#else
        clock->freq = (uint64_t)1000;
#endif
    }

    return clock;
}

void ps_clock_start(ps_clock_data* ps_clock) {
    if(!ps_clock->started){
        ps_clock->dawn = ps_clock_now(ps_clock);
        ps_clock->started = true;
    }
    ps_vector_push_back(ps_clock->timers_t0,ps_clock_now(ps_clock),uint64_t);
}
void ps_clock_stop(ps_clock_data* ps_clock) {
    if(ps_vector_size(ps_clock->timers_t0)>0){
        ps_clock->dt = (double)(ps_clock_now(ps_clock) - ps_vector_back(ps_clock->timers_t0))/ps_clock->freq; 
        ps_vector_pop_back(ps_clock->timers_t0,uint64_t);
    }
}
double ps_clock_uptime(ps_clock_data* ps_clock) {
    return (double)(ps_clock_now(ps_clock) - ps_clock->dawn)/ps_clock->freq;
}

void ps_clock_reset_uptime(ps_clock_data* ps_clock){
    ps_clock->dawn = ps_clock_now(ps_clock);
}

double ps_clock_dt(ps_clock_data* ps_clock){
    return (double)(ps_clock_now(ps_clock) - ps_vector_back(ps_clock->timers_t0))/ps_clock->freq; 
}

void ps_clock_dt_print(ps_clock_data* ps_clock){
    PS_INFO("%lf s",ps_clock_dt(ps_clock));
    fflush(stdout);
}

void ps_clock_reset(ps_clock_data* ps_clock){
    ps_vector_back(ps_clock->timers_t0) = ps_clock_now(ps_clock); 
}

void ps_clock_fps_print(ps_clock_data* ps_clock){
    PS_INFO("%lf FPS",1.0/ps_clock_dt(ps_clock));
    // fflush(stdout);
}

double ps_clock_fps(ps_clock_data* ps_clock){
    return 1.0/ps_clock_dt(ps_clock);
}

void ps_clock_update(ps_clock_data* ps_clock, double fps){
    while(fps != 0.0 && ps_clock_dt(ps_clock) < (double)1.0/fps){
            
    }
}

/**
 @author Abhikalp Unakal
 @date 01 july 2021
 @file ps_events.c
 @brief pepper engine api for event systems
*/

#include "ps_events.h"
/**
 @author Abhikalp Unakal
 @date  16 jan 2022
 @file ps_window.c
 @brief pepper engine core os api layer wrapper
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ps_types.h"
#include "ps_defines.h"
#include "ps_window.h"
#include "ps_keycodes.h"
#include "ps_logging.h"
#include "ps_math.h"

typedef struct ps_mouse_t{
  ps_size_t num; // number of buttons
  uint8_t* keystate; // current frame button states
  ps_vec2 position; // current mouse position
}ps_mouse_t;

typedef struct ps_keyboard_t{
  ps_size_t num; // number of keys 
  uint8_t* keystate; // current key state
}ps_keyboard_t;

typedef struct ps_input_t{
  ps_mouse_t mouse0; // previous mouse
  ps_mouse_t mouse1; // current mouse
  ps_keyboard_t keyboard0; // previous keyboard
  ps_keyboard_t keyboard1; // current keyboard
}ps_input_t;

typedef struct ps_window_impl{
  GLFWwindow* handle0; // graphics handle 0
  ps_input_t* input0; // input handle 0
}ps_window_impl;

typedef struct ps_window{
  char title[WINDOW_TITLE_LEN];
  int width;
  int height;
  ps_window_impl window;
}ps_window;

ps_window* ps_window_current = NULL;

ps_input_t* ps_window_input_get_handle(ps_window* window); // get the input manager struct for window
void ps_window_input_release(ps_window* window); // release the input manager resources
extern bool ps_vec2_equals(ps_vec2 v0, ps_vec2 v1);

// window handle and graphics methods
ps_window* ps_window_get_handle() {
  ps_window* window = (ps_window*)malloc(sizeof(ps_window));
  window->window.handle0 = NULL;
  window->window.input0 = (ps_input_t*)malloc(sizeof(ps_window_impl));
  if(!window){
      fprintf(stderr, "%s : %s : line %d : failed to get window handle\n",__FILE__, __FUNCTION__, __LINE__);
      exit(-1);
  }
  return window;
}

void ps_window_release(ps_window* window) {
  ps_window_input_release(window);
  free(window);
}

void ps_window_init(ps_window* window, const char* title, int width, int height){
  #ifdef GLFW
  if(!glfwInit()){
      fprintf(stderr, "%s : %s : line %d : failed to initialize glfw\n",__FILE__, __FUNCTION__, __LINE__);
      exit(-1);
  }
  strncpy(window->title,title,ps_min(strlen(title),WINDOW_TITLE_LEN));
  window->width = width;
  window->height = height;
  // create window
  glfwWindowHint(GLFW_DECORATED, true);
	glfwWindowHint(GLFW_RESIZABLE, true);
  window->window.handle0 = glfwCreateWindow(window->width,window->height,window->title,NULL,NULL);
  if(!window->window.handle0){
      fprintf(stderr, "%s : %s : line %d : failed to create glfw window\n",__FILE__, __FUNCTION__, __LINE__);
      exit(-1);  
  }
  // setup context
  glfwMakeContextCurrent(window->window.handle0);
  // load opengl functions
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
      fprintf(stderr, "%s : %s : line %d : failed to initialize OpenGL context\n",__FILE__, __FUNCTION__, __LINE__);
      exit(-1);
  }

  glViewport(0,0,window->width,window->height);
  glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
  //setup the input handles
  window->window.input0 = ps_window_input_get_handle(window);
  #endif
}

ps_vec2 ps_window_screen_get_size(ps_window* window){
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  return (ps_vec2){.x=(double)mode->width,.y=(double)mode->height};
}

ps_vec2 ps_window_get_size(ps_window* window){
  return (ps_vec2){.x=window->width,.y=window->height};
}

void ps_window_update(ps_window* window){
  
}

void ps_window_resize(GLFWwindow* glfwwindow, int w, int h){
  glfwSetWindowSize(glfwwindow, w, h);
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  // double ratio = w / h;
  // if(ratio > 1.0){
  //   glViewport((GLint)fabs((mode->width - w)/2.0), 0, APPW, APPH);
  // }
  // else if (ratio < 1.0){
  //   glViewport(0,(GLint)fabs((mode->width - w)/2.0), APPW, APPH);
  // }
  glViewport(0, 0, w, h);
}

void ps_window_set_title(ps_window* window, const char* buffer) {
  #ifdef GLFW
  glfwSetWindowTitle(window->window.handle0, buffer);
  #endif
}

const char* ps_window_get_title(ps_window* window){
  #ifdef GLFW
  return window->title;
  #endif
}

int ps_window_should_close(ps_window* window){
  #ifdef GLFW
  return glfwWindowShouldClose(window->window.handle0);
  #endif
}

void ps_window_swap_buffers(ps_window* window){
  #ifdef GLFW
  glfwSwapBuffers(window->window.handle0);
  #endif
}

void ps_window_poll_events(ps_window* window){
  #ifdef GLFW
  glfwPollEvents();
  #endif
}

void ps_window_destroy(ps_window* window){
  #ifdef GLFW
  glfwDestroyWindow(window->window.handle0);
  glfwTerminate();
  #endif
}

// window input methods

int ps_window_input_keyboard_get_max_keys(ps_window* window){
  int res = -1;
  #ifdef GLFW
  res = GLFW_KEY_LAST;
  #endif
  return res;
}

int ps_window_input_mouse_get_max_keys(ps_window* window){
  int res = -1;
  #ifdef GLFW
  res = GLFW_MOUSE_BUTTON_LAST;
  #endif
  return res;
}

ps_input_t* ps_window_input_get_handle(ps_window* window){
  ps_input_t* inputmanager = (ps_input_t*)malloc(sizeof(ps_input_t));
  #ifdef GLFW
  if(!inputmanager){
      fprintf(stderr, "%s : %s : line %d : failed to get window input handle\n",__FILE__, __FUNCTION__, __LINE__);
      exit(-1);
  }
  // get the number of keyboard keys
  // get the number of mouse buttons
  inputmanager->keyboard0.num = ps_window_input_keyboard_get_max_keys(window);
  inputmanager->keyboard1.num = ps_window_input_keyboard_get_max_keys(window);
  inputmanager->mouse0.num = ps_window_input_mouse_get_max_keys(window);
  inputmanager->mouse1.num = ps_window_input_mouse_get_max_keys(window);

  // initialize the keyboard default keys
  inputmanager->keyboard0.keystate = (uint8_t*)malloc(sizeof(uint8_t)*inputmanager->keyboard0.num);
  inputmanager->keyboard1.keystate = (uint8_t*)malloc(sizeof(uint8_t)*inputmanager->keyboard1.num);
  
  // initialize the mouse keystates
  inputmanager->mouse0.keystate = (uint8_t*)malloc(sizeof(uint8_t*)*inputmanager->mouse0.num);
  inputmanager->mouse1.keystate = (uint8_t*)malloc(sizeof(uint8_t*)*inputmanager->mouse1.num);
  if(!inputmanager->mouse0.keystate || !inputmanager->mouse1.keystate || !inputmanager->keyboard0.keystate || !inputmanager->keyboard1.keystate){
      fprintf(stderr, "%s : %s : line %d : failed to allocate buffers for input states\n",__FILE__, __FUNCTION__, __LINE__);
      exit(-1);
  }
  inputmanager->mouse0.position = (ps_vec2){.x=0.0,.y=0.0};
  inputmanager->mouse1.position = (ps_vec2){.x=0.0,.y=0.0};

  #endif
  // return the instance 
  return inputmanager;
}

void ps_window_input_update(ps_window* window){
  #ifdef GLFW
  // refresh monitor and window states
  ps_vec2 old_window_size = (ps_vec2){.x=window->width,.y=window->height};
  int w,h;
  glfwGetWindowSize(window->window.handle0, &w, &h);
  ps_vec2 new_window_size = (ps_vec2){w,h};
  if(!ps_vec2_equals(old_window_size,new_window_size)){
    window->width = (int)new_window_size.x;
    window->height = (int)new_window_size.y;
    glViewport(0,0,window->width,window->height);
  }
 

  // refresh keyboard states
  window->window.input0->keyboard0.num = window->window.input0->keyboard1.num;
  memcpy(window->window.input0->keyboard0.keystate, window->window.input0->keyboard1.keystate, sizeof(uint8_t)*window->window.input0->keyboard0.num);
  for(int i=0; i<window->window.input0->keyboard1.num; ++i){
    window->window.input0->keyboard1.keystate[i] = glfwGetKey(window->window.handle0, i);
  }

  // refresh mouse states
  window->window.input0->mouse0.num = window->window.input0->mouse1.num;
  memcpy(window->window.input0->mouse0.keystate, window->window.input0->mouse0.keystate, sizeof(uint8_t)*window->window.input0->mouse0.num);
  for(int i=0; i<window->window.input0->mouse1.num; ++i){
    window->window.input0->mouse1.keystate[i] = glfwGetMouseButton(window->window.handle0, i);
  }
  window->window.input0->mouse0.position = window->window.input0->mouse1.position;
  glfwGetCursorPos(window->window.handle0, &window->window.input0->mouse1.position.x, &window->window.input0->mouse1.position.y);

  #endif
}

void ps_window_input_release(ps_window* window){
  #ifdef GLFW
  free(window->window.input0->keyboard0.keystate);
  free(window->window.input0->keyboard1.keystate);
  free(window->window.input0->mouse0.keystate);
  free(window->window.input0->mouse1.keystate);
  #endif
}

bool ps_window_input_keyboard_ispressed(ps_window* window, ps_keyboard_keycodes keycode){
  return
  window->window.input0->keyboard1.keystate[keycode] == PS_ACTION_PRESS && 
  window->window.input0->keyboard0.keystate[keycode] == PS_ACTION_PRESS;
}

bool ps_window_input_keyboard_isreleased(ps_window* window, ps_keyboard_keycodes keycode){
  return
  window->window.input0->keyboard1.keystate[keycode] == PS_ACTION_RELEASE && 
  window->window.input0->keyboard0.keystate[keycode] == PS_ACTION_RELEASE;
}

bool ps_window_input_keyboard_istriggered_down(ps_window* window, ps_keyboard_keycodes keycode){
  return
  window->window.input0->keyboard1.keystate[keycode] == PS_ACTION_PRESS && 
  window->window.input0->keyboard0.keystate[keycode] == PS_ACTION_RELEASE;
}

bool ps_window_input_keyboard_istriggered_up(ps_window* window, ps_keyboard_keycodes keycode){
  return
  window->window.input0->keyboard1.keystate[keycode] == PS_ACTION_RELEASE && 
  window->window.input0->keyboard0.keystate[keycode] == PS_ACTION_PRESS;
}

ps_vec2 ps_window_input_mouse_get_position(ps_window* window){
  return window->window.input0->mouse1.position;
}

void ps_window_input_mouse_set_position(ps_window* window, ps_vec2 position){
  window->window.input0->mouse1.position = position;
  #ifdef GLFW
  glfwSetCursorPos(window->window.handle0, position.x, position.y);
  #endif
}

bool ps_window_input_mouse_ispressed(ps_window* window, ps_mouse_keycodes keycode){
  return
  window->window.input0->mouse1.keystate[keycode] == PS_ACTION_PRESS && 
  window->window.input0->mouse0.keystate[keycode] == PS_ACTION_PRESS;
}

bool ps_window_input_mouse_isreleased(ps_window* window, ps_mouse_keycodes keycode){
  return
  window->window.input0->mouse1.keystate[keycode] == PS_ACTION_RELEASE && 
  window->window.input0->mouse0.keystate[keycode] == PS_ACTION_RELEASE;
}

bool ps_window_input_mouse_istriggered_down(ps_window* window, ps_mouse_keycodes keycode){
  return
  window->window.input0->mouse1.keystate[keycode] == PS_ACTION_PRESS && 
  window->window.input0->mouse0.keystate[keycode] == PS_ACTION_RELEASE;
}

bool ps_window_input_mouse_istriggered_up(ps_window* window, ps_mouse_keycodes keycode){
  return
  window->window.input0->mouse1.keystate[keycode] == PS_ACTION_RELEASE && 
  window->window.input0->mouse0.keystate[keycode] == PS_ACTION_PRESS;
}
