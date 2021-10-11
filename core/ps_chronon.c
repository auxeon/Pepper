/**
 @author Abhikalp Unakal
 @date 08 june 2021
 @file ps_chronon.c
 @brief wall clock time realted utilities
*/

#if defined(_WIN32) || defined(_WIN64)
#define _CRT_SECURE_NO_WARNINGS
#include "Windows.h"
#endif

#include "ps_chronon.h"
#include "stdbool.h"
#include "stdlib.h"
#include "sys/timeb.h"
#include "../lib/ps_logging.h"
#include "../lib/ps_vector.h"

ps_vector_declare(uint64_t);

typedef struct ps_clock_data{
    ps_vector_uint64_t timers_t0;
    uint64_t dawn;
    bool started;
    double dt;
    uint64_t freq;
}ps_clock_data;
/*
    accurate to milliseconds resolution
*/
uint64_t ps_clock_now(ps_clock_data* c) {
#if defined(_WIN32) || defined(_WIN64)
    uint64_t ticks;
    /* prefer the QueryPerformance counter API if exists*/
    if(QueryPerformanceCounter((LARGE_INTEGER*)&ticks)){
        return ticks;
    }
    else{
        /* windows uses _timeb and _ftime instead of timeb and ftime */
        struct _timeb timebuffer;
        /*_timeb has a max resolution of 1ms*/
        _ftime(&timebuffer);
        return (uint64_t)(((timebuffer.time * c->freq) + timebuffer.millitm));
    }
#elif defined(__APPLE__)
    return mach_absolute_time();
#else
    struct timeb timebuffer;
    ftime(&timebuffer);
    return (uint64_t)(((timebuffer.time * c->freq) + timebuffer.millitm));
#endif
}

ps_clock_data* ps_clock_get(){
    ps_clock_data* clock = (ps_clock_data*)malloc(sizeof(ps_clock_data));
    if (clock) {
        ps_vector_create(clock->timers_t0, uint64_t);
        clock->started = false;
        clock->dt = 0.0;
#if defined(_WIN32) || defined(_WIN64)
        uint64_t freq;
        if(QueryPerformanceFrequency((LARGE_INTEGER*)&freq)){
            clock->freq = freq;
        }
#elif defined(__APPLE__)
        mach_timebase_info_data_t info_data;
        mach_timebase_info(&info_data);
        /*beware info_data is a struct of uint32_t numer, denom - division result will be double but casting it to uint64_t*/
        clock->freq = (uint64_t)((info.denom * 1e9) / info.numer);

#else
        clock->freq = (uint64_t)1000;
#endif
    }

    return clock;
}

void ps_clock_start(ps_clock_data* ps_clock) {
    if(!ps_clock->started){
        ps_clock->dawn = ps_clock_now(ps_clock);
        ps_clock->started = true;
    }
    ps_vector_push_back(ps_clock->timers_t0,ps_clock_now(ps_clock),uint64_t);
}
void ps_clock_stop(ps_clock_data* ps_clock) {
    if(ps_vector_size(ps_clock->timers_t0)>0){
        ps_clock->dt = (double)(ps_clock_now(ps_clock) - ps_vector_back(ps_clock->timers_t0))/ps_clock->freq; 
        ps_vector_pop_back(ps_clock->timers_t0,uint64_t);
    }
}
double ps_clock_uptime(ps_clock_data* ps_clock) {
    return (double)(ps_clock_now(ps_clock) - ps_clock->dawn)/ps_clock->freq;
}

void ps_clock_reset_uptime(ps_clock_data* ps_clock){
    ps_clock->dawn = ps_clock_now(ps_clock);
}

double ps_clock_dt(ps_clock_data* ps_clock){
    return (double)(ps_clock_now(ps_clock) - ps_vector_back(ps_clock->timers_t0))/ps_clock->freq; 
}

void ps_clock_dt_print(ps_clock_data* ps_clock){
    PS_INFO("%lf s",ps_clock_dt(ps_clock));
    fflush(stdout);
}

void ps_clock_reset(ps_clock_data* ps_clock){
    ps_vector_back(ps_clock->timers_t0) = ps_clock_now(ps_clock); 
}

void ps_clock_fps_print(ps_clock_data* ps_clock){
    PS_INFO("%lf FPS",1.0/ps_clock_dt(ps_clock));
    fflush(stdout);
}

double ps_clock_fps(ps_clock_data* ps_clock){
    return 1.0/ps_clock_dt(ps_clock);
}

void ps_clock_update(ps_clock_data* ps_clock, double fps){
    while(fps != 0.0 && ps_clock_dt(ps_clock) < (double)1.0/fps){
            
    }
}

