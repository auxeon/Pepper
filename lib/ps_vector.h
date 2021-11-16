/**
 @author Abhikalp Unakal
 @date 05 june 2021
 @file ps_vector.h
 @brief ps_vector container implemented using macros 
*/

#ifndef VECTOR_H
#define VECTOR_H
#include "stdio.h"
#include "../ps_types.h"


#define ps_vector_declare(type) struct ps_vector_##type {\
    type* data;\
    ps_size_t size;\
    ps_size_t capacity;\
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
    if((vec).size + 1 > (vec).capacity){\
        ps_size_t n = (vec).capacity*2;\
        type* m = (type*)realloc((vec).data,n*sizeof(*((vec).data)));\
        if(!m){\
            fprintf(stderr,"realloc failed");\
            exit(-1);\
        }\
        (vec).data = m;\
        (vec).capacity = n;\
    }\
    (vec).data[(vec).size] = (val);\
    (vec).size += 1;\
}

#define ps_vector_pop_back(vec,type){\
    if((vec).size > 0){\
        type* tmp = NULL;\
        tmp = (type*)realloc((vec).data,((vec).size-1)*sizeof((vec).data[0]));\
        (vec).data = (tmp)?tmp:NULL;\
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
    destroy_func((vec));\
    (vec).size = 0;\
    (vec).capacity = 0;\
    free((vec).data);\
    (vec).data = NULL;\
}


#endif