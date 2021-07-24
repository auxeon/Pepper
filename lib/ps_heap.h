/**
 @author Abhikalp Unakal
 @date 06 july 2021
 @file ps_heap.h
 @brief ps_heap max and min heap 
*/

#include "stdlib.h"
#include "string.h"

#ifdef _WIN64
typedef long long size_t;
#endif

#define parent(i) (i-1)/2
#define left(i) 2*i + 1
#define right(i) left(i) + 1

void heapify(
    void* vec,
    size_t i,
    size_t n,
    size_t data_size,
    int cmp(const void* a, const void* b)
    ){
    void* temp = malloc(data_size);
    if (!temp) {
        return;
    }
    while (1) {
        size_t l = left(i);
        size_t r = right(i);
        size_t max_e = i;
        if (l < n && l >= 0 && cmp((char*)vec + (data_size*max_e), (char*)vec + (data_size*l))) {
            max_e = l;
        }
        if (r < n && r >= 0 && cmp((char*)vec + (data_size*max_e), (char*)vec + (data_size*r))) {
            max_e = r;
        }
        if (max_e != i) {
            memcpy((char*)temp, (char*)vec + (data_size * max_e), data_size);
            memcpy((char*)vec + (data_size * max_e), (char*)vec + (data_size * i), data_size);
            memcpy((char*)vec + (data_size * i), (char*)temp, data_size);
            i = max_e;
        }
        else {
            break;
        }
    }
    free(temp);
}

void make_heap(
    void* vec,
    size_t n,
    size_t data_size,
    int cmp(const void* a, const void* b)
    ){
    for (int i = n/2; i >= 0; --i) {
        heapify(vec, i, n, data_size, cmp);
    }
}
