/*
@author Abhikalp Unakal
@date   05 june 2021
@file   pch.h
@desc   precompiled header file 
*/

#ifdef __cplusplus
extern "C"{
#endif

#ifndef PCH_H
#define PCH_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "sys/timeb.h"

#endif 

#ifndef TYPES_H
#define TYPES_H

#define ps_count(x) sizeof(x)/sizeof(x[0])
typedef unsigned long size_t;
// typedef enum {false, true} bool;

#endif


#ifndef PS_LOGGING_H
#define PS_LOGGING_H

#define ERROR(...) printf("[E] ");printf(__VA_ARGS__); printf("\n");
#define WARN(...) printf("[W] ");printf(__VA_ARGS__); printf("\n");
#define INFO(...) printf("[I] ");printf(__VA_ARGS__); printf("\n");

#endif

/*
@author Abhikalp Unakal
@date   05 june 2021
@file   ps_vector.h
@desc   ps_vector container implemented using macros 
*/
#ifndef VECTOR_H
#define VECTOR_H

#define ps_vector_declare(type) struct ps_vector_##type {\
    type* data;\
    size_t size;\
    size_t capacity;\
};\
typedef struct ps_vector_##type ps_vector_##type

#define ps_vector_size(vec) vec.size

#define ps_vector_capacity(vec) vec.capacity

#define ps_vector_init(vec,cap,type) {\
    (vec).data = (type*)malloc(cap*sizeof(*(vec).data));\
    if(!(vec).data){\
        fprintf(stderr,"ps_vector init failed!\n"); \
        exit(-1);\
    }\
    (vec).size = 0;\
    (vec).capacity = cap;\
}

#define ps_vector_create(vec,type) ps_vector_init((vec),1,type)
#define ps_vector_at(vec,pos) (vec).data[pos]
#define ps_vector_front(vec) (vec).data[0]
#define ps_vector_back(vec) (vec).data[(vec).size-1]

#define ps_vector_push_back(vec,val,type) {\
    if((vec).size + 1 > vec.capacity){\
        size_t n = (vec).capacity*2;\
        type* m = (type*)realloc((vec).data,n*sizeof(*((vec).data)));\
        if(!m){\
            fprintf(stderr,"realloc failed");\
            exit(-1);\
        }\
        (vec).data = m;\
        (vec).capacity = n;\
    }\
    vec.data[vec.size] = val;\
    vec.size += 1;\
}

#define ps_vector_pop_back(vec,type){\
    if((vec).size > 0){\
        (vec).data = (type*)realloc((vec).data,((vec).size-1)*sizeof((vec).data[0]));\
        (vec).size -= 1;\
    }\
}

#define ps_vector_clear(vec){\
    (vec).size=0;\
    (vec).capacity=0;\
    free((vec).data);\
    (vec).data = NULL;\
}

#define ps_vector_destroy(vec,destroy_func){\
    wihle(size--){\
        destroy_func((vec).data[size-1]);\
    }\
    (vec).size = 0;\
    (vec).capacity = 0;\
    free((vec).data);\
    (vec).data = NULL;\
}


#endif

/*
@author Abhikalp Unakal
@date   06 june 2021
@file   mergesort.h
@desc   header file to sort stuff in a generic sense
*/
#ifndef PS_MERGESORT_H
#define PS_MERGESORT_H

typedef unsigned long size_t;

void ps_merge(
        void* vector,
        size_t start,
        size_t end,
        size_t data_size,
        int cmp(const void* a, const void* b, size_t data_size)
    ){

    size_t i=0,j=0,k=start;
    
    size_t mid = start + (end-start)/2;
    size_t ln = mid-start+1;
    size_t rn = end-mid;
    
    char* l = (char*)calloc(ln,data_size);
    char* r = (char*)calloc(rn,data_size);

    if(!l || !r){
        fprintf(stderr, "%s : %s : line %d : failed to allocate memory\n",__FILE__, __FUNCTION__, __LINE__);
        exit(-1);
    }

    memcpy(l, (char*)vector + data_size*start, data_size*ln);
    memcpy(r, (char*)vector + data_size*(mid + 1), data_size*rn);

    while(i<ln && j<rn){
        if(cmp(l + data_size*i, r + data_size*j, data_size)){
            memcpy((char*)vector + data_size*(k++), l + data_size*(i++), data_size*(1));
        }
        else{
            memcpy((char*)vector + data_size*(k++), r + data_size*(j++), data_size*(1));
        }
    }
    while(i<ln){
        memcpy((char*)vector + data_size*(k++), l + data_size*(i++), data_size*(1));
    }
    while(j<rn){
        memcpy((char*)vector + data_size*(k++), r + data_size*(j++), data_size*(1));
    }
    free(l);
    free(r);
}

void ps_mergesort(
        void* vector,
        size_t start,
        size_t end,
        size_t data_size,
        int cmp(const void* a, const void* b, size_t data_size)
    ){

    if(start >= end){
        return;
    }
    
    size_t mid = start + (end-start)/2;
    ps_mergesort(vector, start, mid, data_size, cmp);
    ps_mergesort(vector, mid+1, end, data_size, cmp);
    ps_merge(vector,start,end,data_size,cmp);
}

#endif

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
inline uint64_t ps_clock_now() {
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

inline ps_clock_data* ps_clock_get(){
    ps_clock_data* _clock = (ps_clock_data*)malloc(sizeof(ps_clock_data));
    ps_vector_create(_clock->_timers_t0,uint64_t);
    _clock->_started = false;
    _clock->_dt = 0.0;
    return _clock;
}

inline void ps_clock_start(ps_clock_data* ps_clock) {
    if(!ps_clock->_started){
        ps_clock->_dawn = ps_clock_now();
        ps_clock->_started = true;
    }
    ps_vector_push_back(ps_clock->_timers_t0,ps_clock_now(),uint64_t);
}
inline void ps_clock_stop(ps_clock_data* ps_clock) {
    if(ps_vector_size(ps_clock->_timers_t0)>0){
        ps_clock->_dt = (double)(ps_clock_now() - ps_vector_back(ps_clock->_timers_t0))/1000; 
        ps_vector_pop_back(ps_clock->_timers_t0,uint64_t);
    }
}
inline double ps_clock_uptime(ps_clock_data* ps_clock) {
    return (double)(ps_clock_now() - ps_clock->_dawn)/1000;
}

inline double ps_clock_dt(ps_clock_data* ps_clock){
    return (double)(ps_clock_now() - ps_vector_back(ps_clock->_timers_t0))/1000; 
}

inline void ps_clock_reset(ps_clock_data* ps_clock){
    ps_vector_back(ps_clock->_timers_t0) = ps_clock_now(); 
}

#endif

#ifdef __cplusplus
}
#endif