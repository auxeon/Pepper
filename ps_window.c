/**
 @author Abhikalp Unakal
 @date  16 jan 2022
 @file ps_graphics.c
 @brief pepper engine core os api layer wrapper
*/

#include "ps_types.h"
#include "ps_defines.h"
#include "ps_window.h"
#include "ps_keycodes.h"
#include "ps_logging.h"
#include "ps_math.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "math.h"

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
