/*
@author Abhikalp Unakal
@date   08 june 2021
@file   ps_chronon.c
@desc   wall clock time realted utilities
*/

#include "ps_chronon.h"
#include "../pch.h"
#include "sys/timeb.h"

ps_vector_declare(uint64_t);

struct ps_clock_data{
    ps_vector_uint64_t _timers_t0;
    uint64_t _dawn;
    bool _started;
    double _dt;
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

    ps_clock_data* _clock = (ps_clock_data*)malloc(sizeof(ps_clock_data));
    ps_vector_create(_clock->_timers_t0,uint64_t);
    _clock->_started = false;
    _clock->_dt = 0.0;
    return _clock;
}

void ps_clock_start(ps_clock_data* ps_clock) {
    if(!ps_clock->_started){
        ps_clock->_dawn = ps_clock_now();
        ps_clock->_started = true;
    }
    ps_vector_push_back(ps_clock->_timers_t0,ps_clock_now(),uint64_t);
}
void ps_clock_stop(ps_clock_data* ps_clock) {
    if(ps_vector_size(ps_clock->_timers_t0)>0){
        ps_clock->_dt = (double)(ps_clock_now() - ps_vector_back(ps_clock->_timers_t0))/1000; 
        ps_vector_pop_back(ps_clock->_timers_t0,uint64_t);
    }
}
double ps_clock_uptime(ps_clock_data* ps_clock) {
    return (double)(ps_clock_now() - ps_clock->_dawn)/1000;
}

double ps_clock_dt(ps_clock_data* ps_clock){
    return (double)(ps_clock_now() - ps_vector_back(ps_clock->_timers_t0))/1000; 
}

void ps_clock_reset(ps_clock_data* ps_clock){
    ps_vector_back(ps_clock->_timers_t0) = ps_clock_now(); 
}