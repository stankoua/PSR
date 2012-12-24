#include <pthread.h>
#include "libthrd.h"

void lanceClientLeger(void* dialogue,void* (*action)(void*))
{
    pthread_t tid;
    pthread_create(&tid,NULL,action,dialogue);
    pthread_detach(tid);
}

