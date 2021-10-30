/**
 @author Abhikalp Unakal
 @date 08 june 2021
 @file ps_logging.c
 @brief logging helpers
*/

#ifndef PS_LOGGING_H
#define PS_LOGGING_H

#include "stdio.h"

#define PS_ERROR(...) printf("[E] ");printf(__VA_ARGS__); printf("\n");
#define PS_WARN(...) printf("[W] ");printf(__VA_ARGS__); printf("\n");
#define PS_INFO(...) printf("[I] ");printf(__VA_ARGS__); printf("\n");

#endif