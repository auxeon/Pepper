#include "stdio.h"
#include "stdbool.h"

bool progress( 
    int* total, 
    int* index, 
    int* nback,
    bool (*update)(int* t, int* i),
    int (*msgformat)(int* t, int* i)){
    for(int i=0;i<*nback;++i){
      fprintf(stderr,"\b");
      fflush(stderr);
    }
    bool res = update(total,index);
    *nback = msgformat(total,index);
    return res;
}
