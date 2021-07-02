/*
@author Abhikalp Unakal
@date   01 july 2021
@file   ps_events.c
@desc   pepper engine api for event systems
*/

#include "ps_events.h"

typedef struct ps_event{
    ps_event_id id;
    size_t sz;
}ps_event;