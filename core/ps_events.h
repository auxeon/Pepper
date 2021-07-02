/*
@author Abhikalp Unakal
@date   01 july 2021
@file   ps_events.h
@desc   pepper engine api for event systems
*/

#ifndef PS_EVENTS_H
#define PS_EVENTS_H

#include "core/ps_types.h"

typedef struct ps_event{
    ps_event_id id;
    size_t sz;
}ps_event;


#endif