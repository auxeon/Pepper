/**
 @author Abhikalp Unakal
 @date 13 oct 2021
 @file ps_math.c
 @brief math functions
*/

#ifndef PS_MATH_H
#define PS_MATH_H

#include "../ps_types.h"
#include "math.h"

float ps_vec2_magnitude(ps_vec2 v0) {
    return sqrt(v0.x*v0.x + v0.y*v0.y);
}

ps_vec2 ps_vec2_sub(const ps_vec2 v0, const ps_vec2 v1) {
    return (ps_vec2){.x=v0.x - v1.x, .y=v0.y - v1.y};
}

ps_vec2 ps_vec2_add(const ps_vec2 v0, const ps_vec2 v1) {
    return (ps_vec2){.x=v0.x + v1.x, .y=v0.y + v1.y};
}

ps_vec2 ps_vec2_scale(const ps_vec2 v0, const ps_vec2 s0) {
    return (ps_vec2){.x=v0.x*s0.x, .y=v0.y*s0.y};
}

#endif