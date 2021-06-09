/*
@author Abhikalp Unakal
@date   06 june 2021
@file   mergesort.h
@desc   header file to sort stuff in a generic sense
*/
#ifndef PS_MERGESORT_H
#define PS_MERGESORT_H

#include "string.h"
#include "stdlib.h"
#include "stdio.h"

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