/*
@author Abhikalp Unakal
@date   08 june 2021
@file   ps_chronon.c
@desc   wall clock time realted utilities
*/

#include "ps_chronon.h"
#include "stdbool.h"
#include "stdlib.h"
#include "sys/timeb.h"
#include "../lib/ps_logging.h"
#include "../lib/ps_vector.h"

ps_vector_declare(uint64_t);

struct ps_clock_data{
    ps_vector_uint64_t timers_t0;
    uint64_t dawn;
    bool started;
    double dt;
};
/*
    accurate to milliseconds resolution
*/
uint64_t ps_clock_now() {
#if defined(_WIN32) || defined(_WIN64)
    struct _timeb timebuffer;
    _ftime(&timebuffer);
    return (uint64_t)(((timebuffer.time * 1000) + timebuffer.millitm));
#else
    struct timeb timebuffer;
    ftime(&timebuffer);
    return (uint64_t)(((timebuffer.time * 1000) + timebuffer.millitm));
#endif
}

ps_clock_data* ps_clock_get(){

    ps_clock_data* clock = (ps_clock_data*)malloc(sizeof(ps_clock_data));
    ps_vector_create(clock->timers_t0,uint64_t);
    clock->started = false;
    clock->dt = 0.0;
    return clock;
}

void ps_clock_start(ps_clock_data* ps_clock) {
    if(!ps_clock->started){
        ps_clock->dawn = ps_clock_now();
        ps_clock->started = true;
    }
    ps_vector_push_back(ps_clock->timers_t0,ps_clock_now(),uint64_t);
}
void ps_clock_stop(ps_clock_data* ps_clock) {
    if(ps_vector_size(ps_clock->timers_t0)>0){
        ps_clock->dt = (double)(ps_clock_now() - ps_vector_back(ps_clock->timers_t0))/1000; 
        ps_vector_pop_back(ps_clock->timers_t0,uint64_t);
    }
}
double ps_clock_uptime(ps_clock_data* ps_clock) {
    return (double)(ps_clock_now() - ps_clock->dawn)/1000;
}

void ps_clock_reset_uptime(ps_clock_data* ps_clock){
    ps_clock->dawn = ps_clock_now();
}

double ps_clock_dt(ps_clock_data* ps_clock){
    return (double)(ps_clock_now() - ps_vector_back(ps_clock->timers_t0))/1000; 
}

void ps_clock_reset(ps_clock_data* ps_clock){
    ps_vector_back(ps_clock->timers_t0) = ps_clock_now(); 
}

void ps_clock_fps_print(ps_clock_data* ps_clock){
    INFO("FPS : %lfs",1.0/ps_clock_dt(ps_clock));
    fflush(stdout);
}

void ps_clock_update(ps_clock_data* ps_clock, double fps){
    while(ps_clock_dt(ps_clock) < (double)1.0/fps){
            
    }
}

