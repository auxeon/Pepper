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


// declarations

double      ps_lerp(double s, double e, double lo, double hi, double t);

bool        ps_vec2_equals(const ps_vec2 v0, const ps_vec2 v1);
double      ps_vec2_magnitude(const ps_vec2 v0);
ps_vec2     ps_vec2_sub(const ps_vec2 v0, const ps_vec2 v1);
ps_vec2     ps_vec2_add(const ps_vec2 v0, const ps_vec2 v1);
ps_vec2     ps_vec2_scale(const ps_vec2 v0, const ps_vec2 s0);
double      ps_vec2_dot(const ps_vec2 v0, const ps_vec2 v1);
ps_vec2     ps_vec2_normalize(const ps_vec2 v0);

bool        ps_vec3_equals(const ps_vec3 v0, const ps_vec3 v1);
double      ps_vec3_magnitude(const ps_vec3 v0);
ps_vec3     ps_vec3_sub(const ps_vec3 v0, const ps_vec3 v1);
ps_vec3     ps_vec3_add(const ps_vec3 v0, const ps_vec3 v1);
ps_vec3     ps_vec3_scale(const ps_vec3 v0, const ps_vec3 s0);
double      ps_vec3_dot(const ps_vec3 v0, const ps_vec3 v1);
ps_vec3     ps_vec3_cross(const ps_vec3 v0, const ps_vec3 v1);
ps_vec3     ps_vec3_normalize(const ps_vec3 v0);

bool        ps_vec4_equals(const ps_vec4 v0, const ps_vec4 v1);
double      ps_vec4_magnitude(const ps_vec4 v0);
ps_vec4     ps_vec4_sub(const ps_vec4 v0, const ps_vec4 v1);
ps_vec4     ps_vec4_add(const ps_vec4 v0, const ps_vec4 v1);
ps_vec4     ps_vec4_scale(const ps_vec4 v0, const ps_vec4 s0);
double      ps_vec4_dot(const ps_vec4 v0, const ps_vec4 v1);
ps_vec4     ps_vec4_normalize(const ps_vec4 v0);

ps_vec4     ps_vec4_quaternion_conjugate(const ps_vec4 v0);
ps_vec4     ps_vec4_quaternion_multiply(const ps_vec4 v0, const ps_vec4 v1);
ps_vec4     ps_vec4_quaternion_slerp(const ps_vec4 q0, const ps_vec4 q1, double t);



// definitions


/*
    [s,e] -> [lo,hi] @ t
*/
double ps_lerp(double s, double e, double lo, double hi, double t){

    double st = (t - s) / (e - s);
    return (1-st)*lo + (st)*hi;
}


bool ps_vec2_equals(ps_vec2 v0, ps_vec2 v1){
    return v0.x == v1.x && v0.y == v1.y;
}

double ps_vec2_magnitude(ps_vec2 v0) {
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

double ps_vec2_dot(const ps_vec2 v0, const ps_vec2 v1){
    return v0.x*v1.x + v0.y*v1.y;
}

ps_vec2 ps_vec2_normalize(const ps_vec2 v0){
    double magnitude = ps_vec2_magnitude(v0);
    if(magnitude - 0.0 < EPSILON){
        return (ps_vec2){.x=0.0, .y=0.0};
    }
    return (ps_vec2){.x=v0.x/magnitude, .y=v0.y/magnitude};
}


bool ps_vec3_equals(ps_vec3 v0, ps_vec3 v1){
    return v0.x == v1.x && v0.y == v1.y && v0.z == v1.z;
}

double ps_vec3_magnitude(ps_vec3 v0) {
    return sqrt(v0.x*v0.x + v0.y*v0.y + v0.z*v0.z);
}

ps_vec3 ps_vec3_sub(const ps_vec3 v0, const ps_vec3 v1) {
    return (ps_vec3){.x=v0.x - v1.x, .y=v0.y - v1.y, .z=v0.z - v1.z};
}

ps_vec3 ps_vec3_add(const ps_vec3 v0, const ps_vec3 v1) {
    return (ps_vec3){.x=v0.x + v1.x, .y=v0.y + v1.y, .z=v0.z+v1.z};
}

ps_vec3 ps_vec3_scale(const ps_vec3 v0, const ps_vec3 s0) {
    return (ps_vec3){.x=v0.x*s0.x, .y=v0.y*s0.y, .z=v0.z*s0.z};
}

double ps_vec3_dot(const ps_vec3 v0, const ps_vec3 v1){
    return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
}

ps_vec3 ps_vec3_cross(const ps_vec3 v0, const ps_vec3 v1){
    return (ps_vec3){.x=v0.y*v1.z - v0.z*v1.y, .y=v0.z*v1.x - v0.x*v1.z, .z=v0.x*v1.y - v0.y*v1.x};
}

ps_vec3 ps_vec3_normalize(const ps_vec3 v0){
    double magnitude = ps_vec3_magnitude(v0);
    if(magnitude - 0.0 < EPSILON){
        return (ps_vec3){.x=0.0, .y=0.0, .z=0.0};
    }
    return (ps_vec3){.x=v0.x/magnitude, .y=v0.y/magnitude, .z=v0.z/magnitude};
}


bool ps_vec4_equals(ps_vec4 v0, ps_vec4 v1){
    return v0.x == v1.x && v0.y == v1.y && v0.z == v1.z && v0.w == v1.w;
}

double ps_vec4_magnitude(ps_vec4 v0) {
    return sqrt(v0.x*v0.x + v0.y*v0.y + v0.z*v0.z + v0.w*v0.w);
}

ps_vec4 ps_vec4_sub(const ps_vec4 v0, const ps_vec4 v1) {
    return (ps_vec4){.x=v0.x - v1.x, .y=v0.y - v1.y, .z=v0.z - v1.z, .w=v0.w - v1.w};
}

ps_vec4 ps_vec4_add(const ps_vec4 v0, const ps_vec4 v1) {
    return (ps_vec4){.x=v0.x + v1.x, .y=v0.y + v1.y, .z=v0.z+v1.z, .w=v0.w+v1.w};
}

ps_vec4 ps_vec4_scale(const ps_vec4 v0, const ps_vec4 s0) {
    return (ps_vec4){.x=v0.x*s0.x, .y=v0.y*s0.y, .z=v0.z+s0.z, .w=v0.w*s0.w};
}

double ps_vec4_dot(const ps_vec4 v0, const ps_vec4 v1){
    return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z + v0.w*v1.w;
}

ps_vec4 ps_vec4_normalize(const ps_vec4 v0){
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
ps_vec4 ps_vec4_quaternion_conjugate(const ps_vec4 v0){
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
ps_vec4 ps_vec4_quaternion_multiply(const ps_vec4 v0, const ps_vec4 v1){

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
ps_vec4 ps_vec4_quaternion_slerp(const ps_vec4 q0, const ps_vec4 q1, double t){
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