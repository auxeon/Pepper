/*
@author Abhikalp Unakal
@date   08 june 2021
@file   ps_types.h
@desc   custom types
*/

#ifndef TYPES_H
#define TYPES_H

// constants
#define APPNAME "Pepper"
#define PI 3.14159265359
#define PS_COLOR_EMERALD 46.0/256.0, 204/256.0, 113/256.0

// macros
#define ps_count(x) sizeof(x)/sizeof(x[0])
#define ps_deg2rad(x) PI/180.0f*x
#define ps_rad2deg(x) 180.0/PI*x

// colors
//typedef unsigned long size_t;

typedef struct ps_point{
    float x;
    float y;
    float z;
    float w;
}ps_point;

typedef struct ps_color{
    float r;
    float g;
    float b;
    float a;
}ps_color;


// events
typedef enum ps_event_id{
    E_WINDOW_CLOSE,
    E_WINDOW_KEY_PRESSED,
        P_WINDOW_KEY_PRESSED_KEYCODE,
    E_SHAPE_CHANGE,
        P_FROM_SHAPE,
        P_TO_SHAPE
}ps_event_id;

#endif