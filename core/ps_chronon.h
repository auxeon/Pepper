/**
 @author Abhikalp Unakal
 @date 08 june 2021
 @file ps_chronon.h
 @brief wall clock time realted utilities
*/

#ifndef PS_CHRONON_H
#define PS_CHRONON_H

typedef struct ps_clock_data ps_clock_data;
typedef unsigned long long uint64_t;

ps_clock_data*  ps_clock_get();
uint64_t        ps_clock_now();
void            ps_clock_start(ps_clock_data* ps_clock);
void            ps_clock_stop(ps_clock_data* ps_clock);
double          ps_clock_uptime(ps_clock_data* ps_clock);
double          ps_clock_dt(ps_clock_data* ps_clock);
void            ps_clock_reset(ps_clock_data* ps_clock);
void            ps_clock_fps_print(ps_clock_data* ps_clock);
void            ps_clock_update(ps_clock_data* ps_clock, double fps);
void            ps_clock_reset_uptime(ps_clock_data* ps_clock);

#endif