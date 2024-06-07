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
