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
