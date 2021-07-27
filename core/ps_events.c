/**
 @author Abhikalp Unakal
 @date 01 july 2021
 @file ps_events.c
 @brief pepper engine api for event systems
*/

#include "ps_events.h"
#include "stdlib.h"
#include "../ps_types.h"

typedef struct ps_event{
    ps_event_id id;
    ps_size_t sz;
}ps_event;