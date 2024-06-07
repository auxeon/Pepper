/**
 @author Abhikalp Unakal
 @date 06 june 2021
 @file mergesort.h
 @brief header file to sort stuff in a generic sense
*/
#ifndef PS_MERGESORT_H
#define PS_MERGESORT_H

#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "ps_types.h"

static void ps_merge(void* vec, ps_size_t s, ps_size_t e, ps_size_t d_sz, int cmp(const void* a, const void* b)){
    ps_size_t i=0,j=0,k=s;
    ps_size_t m = s + (e-s)/2;
    ps_size_t ln = m-s+1; 
    ps_size_t rn = e-m;
    char* l = (char*)malloc((ln+rn)*d_sz);
    char* r = l + ln*d_sz;
    if(!l){
        fprintf(stderr, "%s : %s : line %d : failed to allocate memory\n",__FILE__, __FUNCTION__, __LINE__);
        exit(-1);
    }
    memcpy(l, (char*)vec + d_sz*s, d_sz*(e-s+1));
    while(i<ln && j<rn){
        cmp(l + d_sz*i, r + d_sz*j)?
        memcpy((char*)vec + d_sz*(k++), l + d_sz*(i++), d_sz):
        memcpy((char*)vec + d_sz*(k++), r + d_sz*(j++), d_sz);
    }
    while(i<ln){
        memcpy((char*)vec + d_sz*(k++), l + d_sz*(i++), d_sz);
    }
    while(j<rn){
        memcpy((char*)vec + d_sz*(k++), r + d_sz*(j++), d_sz);
    }
    free(l);
}

static void ps_mergesort(void* vec, ps_size_t s, ps_size_t e, ps_size_t d_sz, int cmp(const void* a, const void* b)){
    if(s >= e){
        return;
    }
    ps_size_t m = s + (e - s)/2;
    ps_mergesort(vec, s, m, d_sz, cmp);
    ps_mergesort(vec, m+1, e, d_sz, cmp);
    ps_merge(vec,s,e,d_sz,cmp);
}

#endif
