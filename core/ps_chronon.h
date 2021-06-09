#ifndef PS_CHRONON_H
#define PS_CHRONON_H

typedef struct ps_clock_data ps_clock_data;
typedef unsigned long long uint64_t;

ps_clock_data* ps_clock_get();

uint64_t ps_clock_now();
void    ps_clock_start(ps_clock_data* ps_clock);
void    ps_clock_stop(ps_clock_data* ps_clock);
double  ps_clock_uptime(ps_clock_data* ps_clock);
double  ps_clock_dt(ps_clock_data* ps_clock);
void    ps_clock_reset(ps_clock_data* ps_clock);

#endif