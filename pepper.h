#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#ifndef PS_BST_H
#define PS_BST_H

#define true 1
#define false 0
#define COUNT(x) sizeof(x)/sizeof(x[0])


typedef struct node{
    int data;
    struct node* parent;
    struct node* left;
    struct node* right;
}node;

static node* bst_create_node(int d){
    node* n = (node*)malloc(sizeof(node));
    n->data = d;
    n->parent = n;
    n->left = NULL;
    n->right = NULL;
    return n;
}

static node** bst_insert(node** n, node** pn, int d){
    if(!*n){
        *n = bst_create_node(d);
        (*n)->parent = pn?*pn:NULL;
        return n;
    }
    if((*n)->data > d){
        bst_insert(&((*n)->left),n,d);
    }
    else if((*n)->data < d){
        bst_insert(&((*n)->right),n,d);
    }
}

static int bst_exists(node** rn, int d){
    int res = false;
    if(!*rn){
        res = false;
    }
    else if((*rn)->data == d){
        res = true;
    }
    else if((*rn)->data > d){
        res = bst_exists(&((*rn)->left),d);
    }
    else if((*rn)->data < d){
        res = bst_exists(&((*rn)->right),d);
    }
    return res;
}

static node** bst_find(node** n, int d){
    // base case
    if(!*n){
        return NULL;
    }
    if((*n)->data == d){
        return n;
    }
    else if((*n)->data > d){
        return bst_find(&((*n)->left),d);
    }
    else{
        return bst_find(&((*n)->right),d);
    }

}

static node* bst_from_postorder(int* data, int l, int r){
    if(l>r){
        return NULL;
    }
    node* n = bst_create_node(data[r]);

    int i=r-1;
    while(data[i] > n->data){
        i -=1;
    }
    n->right = bst_from_postorder(data,i+1,r-1);
    n->left = bst_from_postorder(data, l, i);
    return n;
}

static void bst_log_node_details(node**n, node** k){
    printf("parent address : %p\n",(*n)?(*n):NULL);
    printf("parent left : %p\n",(*n)->left?(*n)->left:NULL);
    printf("parent right : %p\n",(*n)->right?(*n)->right:NULL);
    printf("kid address : %p\n",(*k)?(*k):NULL);
    printf("kid left : %p\n",(*k)?(*k)->left:NULL);
    printf("kid right : %p\n",(*k)?(*k)->right:NULL);
}

/*
 * 0 left
 * 1 right
 * -1 not a kid
 */
static int bst_is_k_left_or_right(node** n, node** k){
    //bst_log_node_details(n,k);
    if((*n)->left == *k){
        return 0;
    }
    else if((*n)->right == *k){
        return 1;
    }
    return -1;
}
/*
 *  number of kids
 */
static int bst_count_kids(node** n){
    int res = 0;
    res += (*n)->left?1:0;
    res += (*n)->right?1:0;
    return res;
}
/*
 *  finds inorder predecessor of key
 */
static node** bst_inorder_pred(node** n, int key){
    node** found = NULL;
    found = bst_find(n,key);
    if(!*found){
        return found;   
    }
    if((*found)->left){
        node** poda = &((*found)->left);
        while((*poda)->right){
            poda = &((*poda)->right);
        }
        found = poda;
        return found;
    }
}
/*    

            12
       6       16
   3     8    14
  1 5        13


    leaf    : just delete and set parent left/right to NULL
    one kid : par(node)->(right/left) = node->(kid(left/right)))
    two kid : find inorder pred : pred->par->(left/right) <swap> node

*/
static void bst_copy_node(node** d, node** s){
    (*d)->data = (*s)->data;
}

/*
 * 1 success
 * 0 fail
 */


static int bst_delete(node** n, int key){
    if(!(*n)){
      return false;  
    }
    node** found = bst_find(n,key);
    if(!found){
        return false;
    }
    int nkids = bst_count_kids(found);
    node** par = &((*found)->parent);
    node** pred = NULL;
    node* t = *found;
    switch(nkids){
        case 0:
            switch(bst_is_k_left_or_right(par,found)){
                case 0:
                    (*par)->left = NULL;
                    free(*found);
                    break;
                case 1:
                    (*par)->right = NULL;
                    free(*found);
                    break;
                default:
                    break;
            }
            break;
        case 1:
            switch(bst_is_k_left_or_right(par,found)){
                case 0:
                    (*par)->left = (*found)->left?(*found)->left:(*found)->right;
                    free(t);
                    break;
                case 1:
                    (*par)->right = (*found)->left?(*found)->left:(*found)->right;
                    free(t);
                    break;
                default:
                    break;
            }
            break;
        case 2:
            // find inorder predecessor
            // copy data and then delete pred
            pred = bst_inorder_pred(found,key);
            if(!pred){
                t = *n;
                (*n) = (*n)->right;
                free(t);
            }
            bst_copy_node(found,pred);
            par = &((*pred)->parent);
            switch(bst_is_k_left_or_right(par,pred)){
                case 0:
                    (*par)->left = NULL;
                    free(*pred);
                    break;
                case 1:
                    (*par)->right = NULL;
                    free(*pred);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
        
    }
    return true;
}

/*
 *  print post order traversal of bst
 */
static void bst_print_postorder(node** n){
    if(!(*n)){
        return;
    }
    bst_print_postorder(&((*n)->left));
    bst_print_postorder(&((*n)->right));
    printf("%d ", (*n)->data);
}
        
static int bst_tests(int argc, char** argv){
   /*
             12
       6            16
   3     8        14
  1 5           13

 * */
    node* root = NULL;
    bst_insert(&root,NULL,12);
    bst_insert(&root,&root,6);
    bst_insert(&root,&root,16);
    bst_insert(&root,&root,3);
    bst_insert(&root,&root,8);
    bst_insert(&root,&root,14);
    bst_insert(&root,&root,1);
    bst_insert(&root,&root,5);
    bst_insert(&root,&root,13);
   
    printf("tree 1 : manual insertion\n"); 
    bst_print_postorder(&root);
    

    // creating new tree from old post order
    int post_data[] = {1,5,3,8,6,13,14,16,12};
    int index = COUNT(post_data)-1;
    
    node* postbst = bst_from_postorder(
        post_data,
        0,
        COUNT(post_data)-1
    );
    
    printf("\ntree 2 : bst from postorder\n");
    bst_print_postorder(&postbst);
    printf("\n");

    

    //bst_delete(&root,13);
    printf("delete 8\n");
    bst_delete(&root,8);
    bst_print_postorder(&root);
    printf("\n");
    printf("delete 14\n");
    bst_delete(&root,14);
    bst_print_postorder(&root);
    printf("\n");
    printf("delete 6\n");
    bst_delete(&root,6);
    bst_print_postorder(&root);
    printf("\n");
    printf("find\n");
    printf("23 : %d\n",bst_exists(&root,23));
    printf("7 : %d\n",bst_exists(&root,7));
    printf("\n");
    // finding the predecessor
    int key = 12;
    printf("pred(%d) : %d\n",key,(*bst_inorder_pred(&root,key))->data);
    return 0;
}




#endif
/**
 @author Abhikalp Unakal
 @date 08 june 2021
 @file ps_chronon.h
 @brief wall clock time realted utilities
*/

#ifndef PS_CHRONON_H
#define PS_CHRONON_H

#include "ps_types.h"

typedef struct ps_clock_data ps_clock_data;

ps_clock_data*  ps_clock_get();
uint64_t        ps_clock_now();
void            ps_clock_start(ps_clock_data* ps_clock);
void            ps_clock_stop(ps_clock_data* ps_clock);
double          ps_clock_uptime(ps_clock_data* ps_clock);
double          ps_clock_dt(ps_clock_data* ps_clock);
void            ps_clock_dt_print(ps_clock_data* ps_clock);
void            ps_clock_reset(ps_clock_data* ps_clock);
void            ps_clock_fps_print(ps_clock_data* ps_clock);
double          ps_clock_fps(ps_clock_data* ps_clock);
void            ps_clock_update(ps_clock_data* ps_clock, double fps);
void            ps_clock_reset_uptime(ps_clock_data* ps_clock);

#endif
/**
 @author Abhikalp Unakal
 @date 15 jan 2022
 @file ps_defines.h
 @brief pepper engine defines
*/

#ifndef PS_DEFINES_H
#define PS_DEFINES_H

// WINDOWS
#if defined _WIN64 || defined _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define random rand
// included Windows.h before glad.h gets included in pch.h/ps_graphics.h to deal with warning C4005: 'APIENTRY': macro redefinition 
#include "Windows.h"
// telling optimus to switch to nvidia graphics card instead of internal 
// _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
#endif


// APPLE
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif


// WINDOWING FRAMEWORK
#ifdef GLFW
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#endif

#endif
/**
 @author Abhikalp Unakal
 @date 05 feb 2022
 @file ps_ecs.h
 @brief pepper engine entity component system 
*/

#ifndef PS_ECS_H
#define PS_ECS_H

#include "ps_types.h"




#endif/**
 @author Abhikalp Unakal
 @date 08 june 2021
 @file ps_engine.h
 @brief pepper engine api wrapper for future
*/

#ifndef PEPPER_H
#define PEPPER_H



#endif/**
 @author Abhikalp Unakal
 @date 01 july 2021
 @file ps_events.h
 @brief pepper engine api for event systems
*/

#ifndef PS_EVENTS_H
#define PS_EVENTS_H

#include "ps_types.h"

typedef struct ps_event{
    ps_event_id id;
    ps_size_t sz;
}ps_event;

#endif/**
 @author Abhikalp Unakal
 @date 06 july 2021
 @file ps_heap.h
 @brief ps_heap max and min heap 
*/

#include <stdlib.h>
#include <string.h>
#include "ps_types.h"

#define parent(i) (i-1)/2
#define left(i) 2*i + 1
#define right(i) left(i) + 1

static void heapify(
    void* vec,
    ps_size_t i,
    ps_size_t n,
    ps_size_t data_size,
    int cmp(const void* a, const void* b)
    ){
    void* temp = malloc(data_size);
    if (!temp) {
        return;
    }
    while (1) {
        ps_size_t l = left(i);
        ps_size_t r = right(i);
        ps_size_t max_e = i;
        if (l < n && l >= 0 && cmp((char*)vec + (data_size*max_e), (char*)vec + (data_size*l))) {
            max_e = l;
        }
        if (r < n && r >= 0 && cmp((char*)vec + (data_size*max_e), (char*)vec + (data_size*r))) {
            max_e = r;
        }
        if (max_e != i) {
            memcpy((char*)temp, (char*)vec + (data_size * max_e), data_size);
            memcpy((char*)vec + (data_size * max_e), (char*)vec + (data_size * i), data_size);
            memcpy((char*)vec + (data_size * i), (char*)temp, data_size);
            i = max_e;
        }
        else {
            break;
        }
    }
    free(temp);
}

 static void make_heap(
    void* vec,
    ps_size_t n,
    ps_size_t data_size,
    int cmp(const void* a, const void* b)
    ){
    for (ps_size_t i = n/2; i >= 0; --i) {
        heapify(vec, i, n, data_size, cmp);
    }
}
/**
 @author Abhikalp Unakal
 @date 16 jan 
 @file ps_keycodes.h
 @brief keycodes for input devices
*/

#ifndef PS_KEYCODES_H
#define PS_KEYCODES_H

#include "ps_defines.h"

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
/**
 @author Abhikalp Unakal
 @date 08 june 2021
 @file ps_logging.c
 @brief logging helpers
*/

#ifndef PS_LOGGING_H
#define PS_LOGGING_H

#include "stdio.h"
#include "ps_types.h"

#define PS_ERROR(...) printf("[E] ");printf(__VA_ARGS__); printf("\n");
#define PS_WARN(...) printf("[W] ");printf(__VA_ARGS__); printf("\n");
#define PS_INFO(...) printf("[I] ");printf(__VA_ARGS__); printf("\n");

#define print_ps_vec2(vec) printf("(%f, %f)\n",vec.x, vec.y);
#define print_ps_vec3(vec) printf("(%f, %f, %f)\n",vec.x, vec.y, vec.z);
#define print_ps_vec4(vec) printf("(%f, %f, %f, %f)\n",vec.x, vec.y, vec.y, vec.w);

#endif
/**
 @author Abhikalp Unakal
 @date 13 oct 2021
 @file ps_math.c
 @brief math functions
*/

#ifndef PS_MATH_H
#define PS_MATH_H

#include "ps_types.h"
#include "math.h"


// declarations

static double      ps_lerp(double s, double e, double lo, double hi, double t);

static bool        ps_vec2_equals(const ps_vec2 v0, const ps_vec2 v1);
static double      ps_vec2_magnitude(const ps_vec2 v0);
static ps_vec2     ps_vec2_sub(const ps_vec2 v0, const ps_vec2 v1);
static ps_vec2     ps_vec2_add(const ps_vec2 v0, const ps_vec2 v1);
static ps_vec2     ps_vec2_scale(const ps_vec2 v0, const ps_vec2 s0);
static double      ps_vec2_dot(const ps_vec2 v0, const ps_vec2 v1);
static ps_vec2     ps_vec2_normalize(const ps_vec2 v0);

static bool        ps_vec3_equals(const ps_vec3 v0, const ps_vec3 v1);
static double      ps_vec3_magnitude(const ps_vec3 v0);
static ps_vec3     ps_vec3_sub(const ps_vec3 v0, const ps_vec3 v1);
static ps_vec3     ps_vec3_add(const ps_vec3 v0, const ps_vec3 v1);
static ps_vec3     ps_vec3_scale(const ps_vec3 v0, const ps_vec3 s0);
static double      ps_vec3_dot(const ps_vec3 v0, const ps_vec3 v1);
static ps_vec3     ps_vec3_cross(const ps_vec3 v0, const ps_vec3 v1);
static ps_vec3     ps_vec3_normalize(const ps_vec3 v0);

static bool        ps_vec4_equals(const ps_vec4 v0, const ps_vec4 v1);
static double      ps_vec4_magnitude(const ps_vec4 v0);
static ps_vec4     ps_vec4_sub(const ps_vec4 v0, const ps_vec4 v1);
static ps_vec4     ps_vec4_add(const ps_vec4 v0, const ps_vec4 v1);
static ps_vec4     ps_vec4_scale(const ps_vec4 v0, const ps_vec4 s0);
static double      ps_vec4_dot(const ps_vec4 v0, const ps_vec4 v1);
static ps_vec4     ps_vec4_normalize(const ps_vec4 v0);

static ps_vec4     ps_vec4_quaternion_conjugate(const ps_vec4 v0);
static ps_vec4     ps_vec4_quaternion_multiply(const ps_vec4 v0, const ps_vec4 v1);
static ps_vec4     ps_vec4_quaternion_slerp(const ps_vec4 q0, const ps_vec4 q1, double t);



// definitions


/*
    [s,e] -> [lo,hi] @ t
*/
static double ps_lerp(double s, double e, double lo, double hi, double t){

    double st = (t - s) / (e - s);
    return (1-st)*lo + (st)*hi;
}


static bool ps_vec2_equals(ps_vec2 v0, ps_vec2 v1){
    return v0.x == v1.x && v0.y == v1.y;
}

static double ps_vec2_magnitude(ps_vec2 v0) {
    return sqrt(v0.x*v0.x + v0.y*v0.y);
}

static ps_vec2 ps_vec2_sub(const ps_vec2 v0, const ps_vec2 v1) {
    return (ps_vec2){.x=v0.x - v1.x, .y=v0.y - v1.y};
}

static ps_vec2 ps_vec2_add(const ps_vec2 v0, const ps_vec2 v1) {
    return (ps_vec2){.x=v0.x + v1.x, .y=v0.y + v1.y};
}

static ps_vec2 ps_vec2_scale(const ps_vec2 v0, const ps_vec2 s0) {
    return (ps_vec2){.x=v0.x*s0.x, .y=v0.y*s0.y};
}

static double ps_vec2_dot(const ps_vec2 v0, const ps_vec2 v1){
    return v0.x*v1.x + v0.y*v1.y;
}

static ps_vec2 ps_vec2_normalize(const ps_vec2 v0){
    double magnitude = ps_vec2_magnitude(v0);
    if(magnitude - 0.0 < EPSILON){
        return (ps_vec2){.x=0.0, .y=0.0};
    }
    return (ps_vec2){.x=v0.x/magnitude, .y=v0.y/magnitude};
}


static bool ps_vec3_equals(ps_vec3 v0, ps_vec3 v1){
    return v0.x == v1.x && v0.y == v1.y && v0.z == v1.z;
}

static double ps_vec3_magnitude(ps_vec3 v0) {
    return sqrt(v0.x*v0.x + v0.y*v0.y + v0.z*v0.z);
}

static ps_vec3 ps_vec3_sub(const ps_vec3 v0, const ps_vec3 v1) {
    return (ps_vec3){.x=v0.x - v1.x, .y=v0.y - v1.y, .z=v0.z - v1.z};
}

static ps_vec3 ps_vec3_add(const ps_vec3 v0, const ps_vec3 v1) {
    return (ps_vec3){.x=v0.x + v1.x, .y=v0.y + v1.y, .z=v0.z+v1.z};
}

static ps_vec3 ps_vec3_scale(const ps_vec3 v0, const ps_vec3 s0) {
    return (ps_vec3){.x=v0.x*s0.x, .y=v0.y*s0.y, .z=v0.z*s0.z};
}

static double ps_vec3_dot(const ps_vec3 v0, const ps_vec3 v1){
    return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
}

static ps_vec3 ps_vec3_cross(const ps_vec3 v0, const ps_vec3 v1){
    return (ps_vec3){.x=v0.y*v1.z - v0.z*v1.y, .y=v0.z*v1.x - v0.x*v1.z, .z=v0.x*v1.y - v0.y*v1.x};
}

static ps_vec3 ps_vec3_normalize(const ps_vec3 v0){
    double magnitude = ps_vec3_magnitude(v0);
    if(magnitude - 0.0 < EPSILON){
        return (ps_vec3){.x=0.0, .y=0.0, .z=0.0};
    }
    return (ps_vec3){.x=v0.x/magnitude, .y=v0.y/magnitude, .z=v0.z/magnitude};
}


static bool ps_vec4_equals(ps_vec4 v0, ps_vec4 v1){
    return v0.x == v1.x && v0.y == v1.y && v0.z == v1.z && v0.w == v1.w;
}

static double ps_vec4_magnitude(ps_vec4 v0) {
    return sqrt(v0.x*v0.x + v0.y*v0.y + v0.z*v0.z + v0.w*v0.w);
}

static ps_vec4 ps_vec4_sub(const ps_vec4 v0, const ps_vec4 v1) {
    return (ps_vec4){.x=v0.x - v1.x, .y=v0.y - v1.y, .z=v0.z - v1.z, .w=v0.w - v1.w};
}

static ps_vec4 ps_vec4_add(const ps_vec4 v0, const ps_vec4 v1) {
    return (ps_vec4){.x=v0.x + v1.x, .y=v0.y + v1.y, .z=v0.z+v1.z, .w=v0.w+v1.w};
}

static ps_vec4 ps_vec4_scale(const ps_vec4 v0, const ps_vec4 s0) {
    return (ps_vec4){.x=v0.x*s0.x, .y=v0.y*s0.y, .z=v0.z+s0.z, .w=v0.w*s0.w};
}

static double ps_vec4_dot(const ps_vec4 v0, const ps_vec4 v1){
    return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z + v0.w*v1.w;
}

static ps_vec4 ps_vec4_normalize(const ps_vec4 v0){
    double magnitude = ps_vec4_magnitude(v0);
    if(magnitude - 0.0 < EPSILON){
        return (ps_vec4){.x=0.0, .y=0.0, .z=0.0, .w=0.0};
    }
    return (ps_vec4){.x=v0.x/magnitude, .y=v0.y/magnitude, .z=v0.z/magnitude, .w=v0.w/magnitude};
}

/*
    quaternion = [vector_part, scalar_part]
    q0 = [a, sa]
    q0_conjugate = [-a, sa]
*/
static ps_vec4 ps_vec4_quaternion_conjugate(const ps_vec4 v0){
    return (ps_vec4){.x=-v0.x, .y=-v0.y, .z=-v0.z, .w=v0.w};
}

/*
    a = Ai + Bj + Ck
    b = Di + Ej + Fk

    quaternion = [vector_part, scalar_part]
    q0 = [a, sa]
    q1 = [b, sb]

    q0 * q1 = [scale(b,sa) + scale(a,sb) + cross(a,b), sa*sb - dot(a,b)]
*/
static ps_vec4 ps_vec4_quaternion_multiply(const ps_vec4 v0, const ps_vec4 v1){

    ps_vec3 a = (ps_vec3){.x=v0.x, .y=v0.y, .z=v0.z};
    ps_vec3 b = (ps_vec3){.x=v1.x, .y=v1.y, .z=v1.z};
    ps_vec3 complex_res = 
    ps_vec3_add(
        ps_vec3_add(
            ps_vec3_scale(b, (ps_vec3){.x=v0.w, .y=v0.w, .z=v0.w}), 
            ps_vec3_scale(a, (ps_vec3){.x=v1.w, .y=v1.w, .z=v1.w})
        ), 
        ps_vec3_cross(a,b)
    );
    return (ps_vec4){.x=complex_res.x, .y=complex_res.y, .z=complex_res.z, .w=v0.w*v1.w - ps_vec3_dot(a,b)};
}

/*
    Speherical lerp
    a = q0 normalized
    b = q1 normalized
    theta = acos(a.b)
    r = (sin((1-t) * theta)/sin(theta)) * a + (sin(t * theta) / sin(theta)) * b
*/
static ps_vec4 ps_vec4_quaternion_slerp(const ps_vec4 q0, const ps_vec4 q1, double t){
    ps_vec4 a = ps_vec4_normalize(q0);
    ps_vec4 b = ps_vec4_normalize(q1);
    double theta = acos(ps_vec4_dot(a,b));
    double one_by_sin_theta = 1 / sin(theta);
    double sin_one_minus_t_theta = sin((1-t)*theta);
    double sin_t_theta = sin(t*theta);
    double mult_a = sin_one_minus_t_theta * one_by_sin_theta;
    double mult_b = sin_t_theta * one_by_sin_theta;
    return ps_vec4_add(
        ps_vec4_scale(a, (ps_vec4){mult_a, mult_a, mult_a, mult_a}),
        ps_vec4_scale(b, (ps_vec4){mult_b, mult_b, mult_b, mult_b})
    );
}



#endif
/**
 @author Abhikalp Unakal
 @date 06 june 2021
 @file mergesort.h
 @brief header file to sort stuff in a generic sense
*/
#ifndef PS_MERGESORT_H
#define PS_MERGESORT_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ps_types.h"

static void ps_merge(void* vec, ps_size_t s, ps_size_t e, ps_size_t d_sz, int cmp(const void* a, const void* b)){
    ps_size_t i=0,j=0,k=s;
    ps_size_t m = s + (e-s)/2;
    ps_size_t ln = m-s+1; 
    ps_size_t rn = e-m;
    char* l = (char*)malloc((ln+rn)*d_sz);
    char* r = l + ln*d_sz;
    if(!l){
        fprintf(stderr, "%s : %s : line %d : failed to allocate memory\n",__FILE__, __FUNCTION__, __LINE__);
        exit(-1);
    }
    memcpy(l, (char*)vec + d_sz*s, d_sz*(e-s+1));
    while(i<ln && j<rn){
        cmp(l + d_sz*i, r + d_sz*j)?
        memcpy((char*)vec + d_sz*(k++), l + d_sz*(i++), d_sz):
        memcpy((char*)vec + d_sz*(k++), r + d_sz*(j++), d_sz);
    }
    while(i<ln){
        memcpy((char*)vec + d_sz*(k++), l + d_sz*(i++), d_sz);
    }
    while(j<rn){
        memcpy((char*)vec + d_sz*(k++), r + d_sz*(j++), d_sz);
    }
    free(l);
}

static void ps_mergesort(void* vec, ps_size_t s, ps_size_t e, ps_size_t d_sz, int cmp(const void* a, const void* b)){
    if(s >= e){
        return;
    }
    ps_size_t m = s + (e - s)/2;
    ps_mergesort(vec, s, m, d_sz, cmp);
    ps_mergesort(vec, m+1, e, d_sz, cmp);
    ps_merge(vec,s,e,d_sz,cmp);
}

#endif
#include "stdio.h"
#include "stdbool.h"

static bool progress( 
    int* total, 
    int* index, 
    int* nback,
    bool (*update)(int* t, int* i),
    int (*msgformat)(int* t, int* i)){
    for(int i=0;i<*nback;++i){
      fprintf(stderr,"\b");
      fflush(stderr);
    }
    bool res = update(total,index);
    *nback = msgformat(total,index);
    return res;
}
/**
 @author Abhikalp Unakal
 @date 08 june 2021
 @file ps_types.h
 @brief custom types
*/

#ifndef TYPES_H
#define TYPES_H

#include "stdint.h"
#include "stdbool.h"

// constants
#define APPNAME "Pepper"
#define APPW 800
#define APPH 800
#define FPS 60
#define MAXFPS 1000
#define MAXENTITIES 1000000
#define MAXCOMPONENTS 64
#define MAXSYSTEMS 64
#define WINDOW_TITLE_LEN 80
#define PI 3.14159265359
#define EPSILON 1e-13
#define PS_COLOR_EMERALD 46.0/256.0, 204/256.0, 113/256.0
#define PS_COLOR_RED 231.0/256.0, 76/256.0, 60/256.0


// macros
#define ps_count(x) sizeof((x))/sizeof((x)[0])
#define ps_deg2rad(x) PI/180.0*(x)
#define ps_rad2deg(x) 180.0/PI*(x)
#define ps_max(a,b) (((a) > (b)) ? (a) : (b))
#define ps_min(a,b) (((a) < (b)) ? (a) : (b))
#define ps_clamp(v,lo,hi) (((v) < (lo))? (lo) : ((v) > (hi))? (hi) : (v))
#define ps_bit64_set(data,n) ((data) |=  (1<<(n)))
#define ps_bit64_clr(data,n) ((data) &= ~(1<<(n)))
#define ps_bit64_flp(data,n) ((data) ^=  (1<<(n)))
#define ps_bit64_chk(data,n) (((data)>>(n)) & 1)
#define ps_bit64_one(data) ((data) = ~((data) & 0))
#define ps_bit64_zro(data) ((data) &= 0)
#define ps_bit64_sho(data) for(int i=0;i<64;++i){printf("%d",ps_bit64_chk(data,i));}



// typedefs
typedef uint64_t ps_entity_id;
typedef uint64_t ps_component_id;
typedef uint64_t ps_system_id;
typedef uint64_t ps_archetype;

typedef struct ps_vec2{
    union {double x, u, s, w;};
    union { double y, v, t, h;};
}ps_vec2;

typedef struct ps_vec3{
    union {double x, r, s;};
    union {double y, g, t;};
    union {double z, b, p;};
}ps_vec3;

typedef struct ps_vec4{
    union {double x, r, s;};
    union {double y, g, t;};
    union {double z, b, p;};
    union {double w, a, q;};
}ps_vec4;

typedef ps_vec4 ps_color;   

typedef long long ps_size_t;

typedef enum ps_event_id{
    E_WINDOW_CLOSE,
    E_WINDOW_KEY_PRESSED,
        P_WINDOW_KEY_PRESSED_KEYCODE,
    E_SHAPE_CHANGE,
        P_FROM_SHAPE,
        P_TO_SHAPE
}ps_event_id;

#endif
/**
 @author Abhikalp Unakal
 @date 05 june 2021
 @file ps_vector.h
 @brief ps_vector container implemented using macros 
*/

#ifndef VECTOR_H
#define VECTOR_H
#include "stdio.h"
#include "ps_types.h"


#define ps_vector_declare(type) struct ps_vector_##type {\
    type* data;\
    ps_size_t size;\
    ps_size_t capacity;\
};\
typedef struct ps_vector_##type ps_vector_##type

#define ps_vector_size(vec) vec.size

#define ps_vector_capacity(vec) vec.capacity

#define ps_vector_init(vec,cap,type) {\
    (vec).data = (type*)malloc(cap*sizeof(*(vec).data));\
    if(!(vec).data){\
        fprintf(stderr,"ps_vector init failed!\n"); \
        exit(-1);\
    }\
    (vec).size = 0;\
    (vec).capacity = cap;\
}

#define ps_vector_create(vec,type) ps_vector_init((vec),1,type)
#define ps_vector_at(vec,pos) (vec).data[pos]
#define ps_vector_front(vec) (vec).data[0]
#define ps_vector_back(vec) (vec).data[(vec).size-1]

#define ps_vector_push_back(vec,val,type) {\
    if((vec).size + 1 > (vec).capacity){\
        ps_size_t n = (vec).capacity*2;\
        type* m = (type*)realloc((vec).data,n*sizeof(*((vec).data)));\
        if(!m){\
            fprintf(stderr,"realloc failed");\
            exit(-1);\
        }\
        (vec).data = m;\
        (vec).capacity = n;\
    }\
    (vec).data[(vec).size] = (val);\
    (vec).size += 1;\
}

#define ps_vector_pop_back(vec,type){\
    if((vec).size > 0){\
        type* tmp = NULL;\
        tmp = (type*)realloc((vec).data,((vec).size-1)*sizeof((vec).data[0]));\
        (vec).data = (tmp)?tmp:NULL;\
        (vec).size -= 1;\
    }\
}

#define ps_vector_clear(vec){\
    (vec).size=0;\
    (vec).capacity=0;\
    free((vec).data);\
    (vec).data = NULL;\
}

#define ps_vector_destroy(vec,destroy_func){\
    destroy_func((vec));\
    (vec).size = 0;\
    (vec).capacity = 0;\
    free((vec).data);\
    (vec).data = NULL;\
}


#endif
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
#include "ps_types.h"

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
ps_vec2             ps_window_get_size(ps_window* window); // get current window w,h

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
